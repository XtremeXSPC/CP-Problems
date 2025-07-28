# lldb_formatters.py
#
# A generic set of Data Formatters for LLDB, designed to work with custom C++
# implementations of common data structures.It adapts by searching for common
# member names instead of requiring specific ones.

try:
    import lldb  # type: ignore
except ImportError:
    # lldb module is only available when running within LLDB debugger.
    # This allows the file to be imported without errors in other contexts.
    lldb = None
import re
import shlex
import json
import base64
import os

# ----- Global Configuration Settings ----- #
# These can be changed at runtime using the 'formatter_config' command.
g_summary_max_items = 30
g_graph_max_neighbors = 10


# ----- ANSI Color Codes ----- #
# List of colors used in prints.
class Colors:
    RESET = "\x1b[0m"
    BOLD_CYAN = "\x1b[1;36m"
    YELLOW = "\x1b[33m"
    GREEN = "\x1b[32m"
    MAGENTA = "\x1b[35m"
    RED = "\x1b[31m"


# ----- Debug flag to control print statements ----- #
DEBUG_ENABLED = False  # Set to True to see detailed debug output in the LLDB console


def debug_print(message):
    """Prints a message only if debugging is enabled."""
    if DEBUG_ENABLED:
        print(f"[Formatter Debug] {message}")


# ----- Generic Helpers ----- #
def get_child_member_by_names(value, names):
    """
    Attempts to find and return the first valid child member from a list of possible names.
    """
    for name in names:
        child = value.GetChildMemberWithName(name)
        if child.IsValid():
            return child
    return None


def get_raw_pointer(value):
    """
    Extracts the raw pointer address from a raw pointer, unique_ptr, or shared_ptr.
    """
    if not value or not value.IsValid():
        return 0

    # If it's already a pointer, just get the value
    if value.GetType().IsPointerType():
        return value.GetValueAsUnsigned()

    # For smart pointers, try to get the internal pointer by common names
    ptr_member = get_child_member_by_names(value, ["_M_ptr", "__ptr_", "pointer"])
    if ptr_member and ptr_member.IsValid():
        return ptr_member.GetValueAsUnsigned()

    return value.GetValueAsUnsigned()  # Fallback for other smart pointer-like types


def type_has_field(type_obj, field_name):
    """
    Checks if an SBType has a field with the given name by iterating.
    """
    for i in range(type_obj.GetNumberOfFields()):
        if type_obj.GetFieldAtIndex(i).GetName() == field_name:
            return True
    return False


def get_value_summary(value_child):
    """
    Extracts a displayable string from a value SBValue, preferring GetSummary.
    """
    if not value_child or not value_child.IsValid():
        return f"{Colors.RED}[invalid]{Colors.RESET}"

    # GetSummary() often provides a better representation (e.g., for strings)
    summary = value_child.GetSummary()
    if summary:
        # Remove quotes for cleaner display inside our own formatting
        return summary.strip('"')

    # Fallback to GetValue() if no summary is available
    return value_child.GetValue()


def formatter_config_command(debugger, command, result, internal_dict):
    """
    Implements the 'formatter_config' command to view and change global settings.
    Usage:
      formatter_config                # View current settings and their descriptions.
      formatter_config <key> <value>  # Set a new value for a setting.
    """
    # We must declare that we intend to MODIFY the global variables.
    global g_summary_max_items, g_graph_max_neighbors

    args = command.split()

    # Case 1: No arguments. Print current settings and descriptions.
    if len(args) == 0:
        result.AppendMessage("Current Formatter Settings:")
        result.AppendMessage(
            f"  - summary_max_items: {g_summary_max_items} (Max items for list/tree summaries)"
        )
        result.AppendMessage(
            f"  - graph_max_neighbors: {g_graph_max_neighbors} (Max neighbors in graph node summaries)"
        )
        result.AppendMessage(
            "\nUse 'formatter_config <key> <value>' to change a setting."
        )
        return

    # Case 2: Wrong number of arguments.
    if len(args) != 2:
        result.SetError("Usage: formatter_config <setting_name> <value>")
        return

    # Case 3: Set a value.
    key = args[0]
    value_str = args[1]

    try:
        value = int(value_str)
    except ValueError:
        result.SetError(f"Invalid value. '{value_str}' is not a valid integer.")
        return

    if key == "summary_max_items":
        g_summary_max_items = value
        result.AppendMessage(f"Set summary_max_items -> {value}")
    elif key == "graph_max_neighbors":
        g_graph_max_neighbors = value
        result.AppendMessage(f"Set graph_max_neighbors -> {value}")
    else:
        result.SetError(
            f"Unknown setting '{key}'.\nAvailable settings are: 'summary_max_items', 'graph_max_neighbors'"
        )


def should_use_colors():
    """
    Returns True if the script is likely running in a terminal that
    supports ANSI color codes (like CodeLLDB's debug console or a real terminal).
    """
    return os.environ.get("TERM_PROGRAM") == "vscode"


# ----- Formatter for Linear Data Structures (Lists, Stacks, Queues) ----- #
class LinearContainerProvider:
    """
    Provides a summary for linear structures that follow a 'next' pointer.
    E.g., List, Queue (list-based), Stack (list-based).
    """

    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.head_ptr = None
        self.next_ptr_name = None
        self.value_name = None
        self.size = 0
        debug_print(f"Provider created for object of type '{valobj.GetTypeName()}'")

    def update(self):
        """
        Initializes pointers and member names by searching through common conventions.
        """
        self.head_ptr = get_child_member_by_names(
            self.valobj, ["head", "m_head", "_head", "top"]
        )
        debug_print(
            f"Searching for head... Found: {'Yes' if self.head_ptr and self.head_ptr.IsValid() else 'No'}"
        )

        if self.head_ptr and get_raw_pointer(self.head_ptr) != 0:
            node_obj = self.head_ptr.Dereference()
            debug_print(f"Head pointer is valid. Dereferencing to get node object.")

            if node_obj and node_obj.IsValid():
                node_type = node_obj.GetType()

                for name in ["next", "m_next", "_next", "pNext"]:
                    if type_has_field(node_type, name):
                        self.next_ptr_name = name
                        break
                for name in ["value", "val", "data", "m_data", "key"]:
                    if type_has_field(node_type, name):
                        self.value_name = name
                        break
                debug_print(f"-> Found 'next' member: '{self.next_ptr_name}'")
                debug_print(f"-> Found 'value' member: '{self.value_name}'")
            else:
                debug_print("-> Failed to dereference head pointer.")

        size_member = get_child_member_by_names(
            self.valobj, ["size", "m_size", "_size", "count"]
        )
        if size_member:
            self.size = size_member.GetValueAsUnsigned()
        debug_print(
            f"Found size member: {'Yes' if size_member else 'No'}. Size is {self.size}"
        )

    def get_summary(self):
        self.update()

        if not self.head_ptr:
            return "Could not find head pointer"

        if get_raw_pointer(self.head_ptr) == 0:
            return f"{Colors.GREEN}size={self.size}{Colors.RESET}, []"

        if not self.next_ptr_name or not self.value_name:
            debug_print(
                "Bailing out: could not determine node structure (val/next names not found)."
            )
            return "Cannot determine node structure (val/next)"

        summary = []
        node = self.head_ptr
        count = 0
        max_items = g_summary_max_items
        visited = set()

        while get_raw_pointer(node) != 0 and count < max_items:
            debug_print(
                f"Loop iter {count+1}: node type='{node.GetTypeName()}', addr='{get_raw_pointer(node):#x}'"
            )

            node_addr = get_raw_pointer(node)
            if node_addr in visited:
                summary.append(f"{Colors.RED}[CYCLE DETECTED]{Colors.RESET}")
                break
            visited.add(node_addr)

            dereferenced_node = node.Dereference()
            if not dereferenced_node or not dereferenced_node.IsValid():
                debug_print("-> Node could not be dereferenced. Breaking loop.")
                break

            value_child = dereferenced_node.GetChildMemberWithName(self.value_name)
            current_val_str = get_value_summary(value_child)
            debug_print(f"-> Extracted value string: '{current_val_str}'")

            summary.append(f"{Colors.YELLOW}{current_val_str}{Colors.RESET}")

            node = dereferenced_node.GetChildMemberWithName(self.next_ptr_name)
            count += 1

        final_summary_str = f" {Colors.BOLD_CYAN}->{Colors.RESET} ".join(summary)

        if get_raw_pointer(node) != 0:
            final_summary_str += f" {Colors.BOLD_CYAN}->{Colors.RESET} ..."

        return f"{Colors.GREEN}size = {self.size}{Colors.RESET}, [{final_summary_str}]"


def LinearContainerSummary(valobj, internal_dict):
    """
    This function is registered with LLDB for Linear Structures.
    """
    provider = LinearContainerProvider(valobj, internal_dict)
    return provider.get_summary()


# ----- Formatter for Trees (Binary and N-ary) ----- #
class GenericTreeProvider:
    """
    Provides 'synthetic children' for tree nodes, allowing them to be expanded in the debugger.
    Handles both binary (left/right) and N-ary (children) trees.
    """

    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.children_member = None
        self.left_member = None
        self.right_member = None
        self.is_nary = False
        self.num_children_val = 0
        self.update()

    def update(self):
        self.children_member = get_child_member_by_names(
            self.valobj, ["children", "m_children"]
        )
        if (
            self.children_member
            and self.children_member.IsValid()
            and self.children_member.MightHaveChildren()
        ):
            self.is_nary = True
            self.num_children_val = self.children_member.GetNumChildren()
        else:
            self.is_nary = False
            self.left_member = get_child_member_by_names(
                self.valobj, ["left", "m_left", "_left"]
            )
            self.right_member = get_child_member_by_names(
                self.valobj, ["right", "m_right", "_right"]
            )

            count = 0
            if self.left_member and get_raw_pointer(self.left_member) != 0:
                count += 1
            if self.right_member and get_raw_pointer(self.right_member) != 0:
                count += 1
            self.num_children_val = count

    def num_children(self):
        return self.num_children_val

    def get_child_at_index(self, index):
        if self.is_nary:
            return (
                self.children_member.GetChildAtIndex(index)
                if self.children_member
                else None
            )
        else:
            current_index = 0
            if self.left_member and get_raw_pointer(self.left_member) != 0:
                if index == current_index:
                    return self.left_member
                current_index += 1
            if self.right_member and get_raw_pointer(self.right_member) != 0:
                if index == current_index:
                    return self.right_member
        return None


# ----- Summary function for Tree Root ----- #
def TreeSummary(valobj, internal_dict):
    """
    Provides a one-line summary for a tree root, showing an in-order traversal for binary trees.
    """
    use_colors = should_use_colors()

    # Conditionally define colors
    C_GREEN = Colors.GREEN if use_colors else ""
    C_YELLOW = Colors.YELLOW if use_colors else ""
    C_CYAN = Colors.BOLD_CYAN if use_colors else ""
    C_RESET = Colors.RESET if use_colors else ""

    root_node = get_child_member_by_names(valobj, ["root", "m_root", "_root"])
    if not root_node or get_raw_pointer(root_node) == 0:
        return "Tree is empty"

    summary_parts = []
    max_nodes = g_summary_max_items
    visited = set()

    # This inner function performs the recursive traversal
    def _in_order_traverse(node_ptr):
        if get_raw_pointer(node_ptr) == 0 or len(summary_parts) >= max_nodes:
            return

        node_addr = get_raw_pointer(node_ptr)
        if node_addr in visited:
            summary_parts.append(f"{Colors.RED}[CYCLE]{Colors.RESET}")
            return
        visited.add(node_addr)

        node = node_ptr.Dereference()
        if not node or not node.IsValid():
            return

        # Find members dynamically inside the node
        left = get_child_member_by_names(node, ["left", "m_left", "_left"])
        right = get_child_member_by_names(node, ["right", "m_right", "_right"])
        value = get_child_member_by_names(node, ["value", "val", "data", "key"])

        debug_print(
            f"-> In-order: traversing node with value '{get_value_summary(value)}'"
        )
        if left:
            _in_order_traverse(left)

        # Stop if we've collected enough nodes
        if len(summary_parts) >= max_nodes:
            return

        val_str = get_value_summary(value)
        summary_parts.append(f"{Colors.YELLOW}{val_str}{Colors.RESET}")

        if right:
            _in_order_traverse(right)

    _in_order_traverse(root_node)

    summary_str = f" {Colors.BOLD_CYAN}<->{Colors.RESET} ".join(summary_parts)
    if len(summary_parts) >= max_nodes:
        summary_str += " ..."

    size_member = get_child_member_by_names(valobj, ["size", "m_size", "count"])
    size_str = ""
    if size_member:
        size_str = (
            f"{Colors.GREEN}size = {size_member.GetValueAsUnsigned()}{Colors.RESET}, "
        )

    return f"{size_str}[{summary_str}]"


# ----- Tree Visualizer Provider ----- #
def tree_visualizer_provider(valobj, internal_dict):
    """
    Detects if it's running inside VS Code with CodeLLDB to provide the
    HTML visualizer. Otherwise, it falls back to the adaptive text summary.
    """
    if os.environ.get("TERM_PROGRAM") == "vscode":
        try:
            json_data = get_tree_json(valobj)
            json_string = json.dumps(json_data, separators=(",", ":"))
            b64_string = base64.b64encode(json_string.encode("utf-8")).decode("utf-8")
            return f"$$JSON_VISUALIZER$${b64_string}"
        except Exception as e:
            return f"Error creating JSON visualizer: {e}"
    else:
        # Fallback for the MS C/C++ extension's GUI and other consoles.
        return TreeSummary(valobj, internal_dict)


# ----- Helper to safely dereference a node pointer ----- #
def _safe_get_node_from_pointer(node_ptr):
    """
    Safely gets the underlying TreeNode struct from an SBValue that can be
    a raw pointer or a smart pointer, returning the SBValue for the struct.
    """
    if not node_ptr or not node_ptr.IsValid():
        return None

    # Try to handle it as a smart pointer first.
    internal_ptr = get_child_member_by_names(node_ptr, ["_M_ptr", "__ptr_", "pointer"])
    if internal_ptr and internal_ptr.IsValid():
        debug_print("   - Smart pointer detected, dereferencing internal ptr.")
        return internal_ptr.Dereference()

    # Fallback for raw pointers.
    debug_print("   - Assuming raw pointer, dereferencing directly.")
    return node_ptr.Dereference()


# ----- Helper functions to collect nodes in different orders ----- #
def _collect_nodes_inorder(node_ptr, nodes_list):
    """Collect nodes in in-order traversal."""
    if not node_ptr or get_raw_pointer(node_ptr) == 0:
        return

    node = _safe_get_node_from_pointer(node_ptr)
    if not node or not node.IsValid():
        return

    left = get_child_member_by_names(node, ["left", "m_left", "_left"])
    right = get_child_member_by_names(node, ["right", "m_right", "_right"])

    # In-order: left, root, right
    if left:
        _collect_nodes_inorder(left, nodes_list)
    nodes_list.append(node_ptr)
    if right:
        _collect_nodes_inorder(right, nodes_list)


def _collect_nodes_postorder(node_ptr, nodes_list):
    """Collect nodes in post-order traversal."""
    if not node_ptr or get_raw_pointer(node_ptr) == 0:
        return

    node = _safe_get_node_from_pointer(node_ptr)
    if not node or not node.IsValid():
        return

    left = get_child_member_by_names(node, ["left", "m_left", "_left"])
    right = get_child_member_by_names(node, ["right", "m_right", "_right"])

    # Post-order: left, right, root
    if left:
        _collect_nodes_postorder(left, nodes_list)
    if right:
        _collect_nodes_postorder(right, nodes_list)
    nodes_list.append(node_ptr)


# ----- Helper for the Pre-Order "drawing" view ----- #
def _recursive_preorder_print(node_ptr, prefix, is_last, result):
    """Helper function to recursively "draw" the tree in Pre-Order."""
    if not node_ptr or get_raw_pointer(node_ptr) == 0:
        return

    node = _safe_get_node_from_pointer(node_ptr)
    if not node or not node.IsValid():
        return

    value = get_child_member_by_names(node, ["value", "val", "data", "key"])
    value_summary = get_value_summary(value)

    # Process the node first
    result.AppendMessage(
        f"{prefix}{'└── ' if is_last else '├── '}{Colors.YELLOW}{value_summary}{Colors.RESET}"
    )

    # Then recurse on children
    left = get_child_member_by_names(node, ["left", "m_left", "_left"])
    right = get_child_member_by_names(node, ["right", "m_right", "_right"])

    children = []
    if left and get_raw_pointer(left) != 0:
        children.append(left)
    if right and get_raw_pointer(right) != 0:
        children.append(right)

    for i, child in enumerate(children):
        new_prefix = f"{prefix}{'    ' if is_last else '│   '}"
        _recursive_preorder_print(child, new_prefix, i == len(children) - 1, result)


# ----- Helper to print nodes in traversal order as tree structure ----- #
def _print_traversal_as_tree(nodes_list, result, max_children=2):
    """Print nodes in a tree-like structure based on their order in the list."""
    if not nodes_list:
        return

    def print_tree_recursive(start_idx, prefix, is_last, level):
        if start_idx >= len(nodes_list):
            return start_idx

        # Get current node
        node_ptr = nodes_list[start_idx]
        node = _safe_get_node_from_pointer(node_ptr)
        if not node or not node.IsValid():
            return start_idx + 1

        value = get_child_member_by_names(node, ["value", "val", "data", "key"])
        value_summary = get_value_summary(value)

        # Print current node
        result.AppendMessage(
            f"{prefix}{'└── ' if is_last else '├── '}{Colors.YELLOW}{value_summary}{Colors.RESET}"
        )

        # Calculate children
        next_idx = start_idx + 1
        children_count = 0

        # For binary tree, try to have at most 2 children
        # Left child would be next node, right child would be the one after
        children_indices = []

        # Simple strategy: next nodes in sequence become children
        # We'll limit to 2 children for binary tree appearance
        remaining_nodes = len(nodes_list) - next_idx
        if remaining_nodes > 0:
            # Calculate how many children this node should have
            if remaining_nodes >= 2 and start_idx < len(nodes_list) // 2:
                children_count = 2
            elif remaining_nodes >= 1:
                children_count = 1

            for i in range(children_count):
                if next_idx + i < len(nodes_list):
                    children_indices.append(next_idx + i)

        # Print children
        current_idx = next_idx
        for i, child_idx in enumerate(children_indices):
            new_prefix = f"{prefix}{'    ' if is_last else '│   '}"
            is_last_child = i == len(children_indices) - 1

            # For the first child, use the calculated index
            if i == 0:
                current_idx = print_tree_recursive(
                    child_idx,
                    new_prefix,
                    is_last_child and len(children_indices) == 1,
                    level + 1,
                )
            else:
                # For subsequent children, continue from where the previous child left off
                current_idx = print_tree_recursive(
                    current_idx, new_prefix, is_last_child, level + 1
                )

        # If we had children, return where we left off, otherwise just next node
        return current_idx if children_indices else next_idx

    # Start the recursive printing
    print_tree_recursive(0, "", True, 0)


# ----- Sequential tree structure with proper binary tree layout ----- #
def _print_sequential_tree(nodes_list, result):
    """Print nodes in a binary tree structure based on their traversal order."""
    if not nodes_list:
        return

    def print_node_recursive(index, prefix, is_last):
        if index >= len(nodes_list):
            return

        node_ptr = nodes_list[index]
        node = _safe_get_node_from_pointer(node_ptr)
        if not node or not node.IsValid():
            return

        value = get_child_member_by_names(node, ["value", "val", "data", "key"])
        value_summary = get_value_summary(value)

        # Print current node
        result.AppendMessage(
            f"{prefix}{'└── ' if is_last else '├── '}{Colors.YELLOW}{value_summary}{Colors.RESET}"
        )

        # Calculate children indices (standard binary tree: left = 2*i+1, right = 2*i+2)
        left_child_idx = 2 * index + 1
        right_child_idx = 2 * index + 2

        children = []
        if left_child_idx < len(nodes_list):
            children.append(left_child_idx)
        if right_child_idx < len(nodes_list):
            children.append(right_child_idx)

        # Print children
        for i, child_idx in enumerate(children):
            new_prefix = f"{prefix}{'    ' if is_last else '│   '}"
            is_last_child = i == len(children) - 1
            print_node_recursive(child_idx, new_prefix, is_last_child)

    # Start with root (index 0)
    print_node_recursive(0, "", True)


# ----- Central dispatcher to handle all 'pptree' commands ----- #
def _pptree_command_dispatcher(debugger, command, result, internal_dict, order):
    """
    A single function to handle the logic for all traversal commands.
    'order' can be 'preorder', 'inorder', or 'postorder'.
    """
    args = command.split()
    if not args:
        result.SetError(f"Usage: pptree_{order} <variable_name>")
        return

    frame = (
        debugger.GetSelectedTarget().GetProcess().GetSelectedThread().GetSelectedFrame()
    )
    if not frame.IsValid():
        result.SetError("Cannot execute command: invalid execution context.")
        return

    tree_val = frame.FindVariable(args[0])
    if not tree_val or not tree_val.IsValid():
        result.SetError(f"Could not find variable '{args[0]}'.")
        return

    root_node_ptr = get_child_member_by_names(tree_val, ["root", "m_root", "_root"])
    if not root_node_ptr or get_raw_pointer(root_node_ptr) == 0:
        result.AppendMessage("Tree is empty.")
        return

    # Select the correct approach based on the order
    if order == "preorder":
        result.AppendMessage(
            f"{tree_val.GetTypeName()} at {tree_val.GetAddress()} (Pre-Order):"
        )
        _recursive_preorder_print(root_node_ptr, "", True, result)
    elif order == "inorder":
        result.AppendMessage(
            f"{tree_val.GetTypeName()} at {tree_val.GetAddress()} (In-Order):"
        )
        # Collect nodes in in-order, then display as tree structure
        nodes_list = []
        _collect_nodes_inorder(root_node_ptr, nodes_list)
        _print_sequential_tree(nodes_list, result)
    elif order == "postorder":
        result.AppendMessage(
            f"{tree_val.GetTypeName()} at {tree_val.GetAddress()} (Post-Order):"
        )
        # Collect nodes in post-order, then display as tree structure
        nodes_list = []
        _collect_nodes_postorder(root_node_ptr, nodes_list)
        _print_sequential_tree(nodes_list, result)


# --- The user-facing command functions are now simple one-liners ---
def pptree_preorder_command(debugger, command, result, internal_dict):
    """Implements the 'pptree_preorder' command."""
    _pptree_command_dispatcher(debugger, command, result, internal_dict, "preorder")


def pptree_inorder_command(debugger, command, result, internal_dict):
    """Implements the 'pptree_inorder' command."""
    _pptree_command_dispatcher(debugger, command, result, internal_dict, "inorder")


def pptree_postorder_command(debugger, command, result, internal_dict):
    """Implements the 'pptree_postorder' command."""
    _pptree_command_dispatcher(debugger, command, result, internal_dict, "postorder")


# ----- JSON Tree Visualizer ----- #
def _build_json_tree_node(node_ptr):
    """
    Recursive helper to build a Python dictionary representing the tree node
    and its descendants, ready to be serialized to JSON.
    """
    if not node_ptr or get_raw_pointer(node_ptr) == 0:
        return None

    node = node_ptr.Dereference()
    if not node or not node.IsValid():
        return None

    # Get node members
    value = get_child_member_by_names(node, ["value", "val", "data", "key"])
    left = get_child_member_by_names(node, ["left", "m_left", "_left"])
    right = get_child_member_by_names(node, ["right", "m_right", "_right"])

    val_summary = get_value_summary(value)

    # Build the JSON object for this node
    json_node = {"name": val_summary}
    children = []

    # Recurse for children
    left_child = _build_json_tree_node(left)
    if left_child:
        children.append(left_child)

    right_child = _build_json_tree_node(right)
    if right_child:
        children.append(right_child)

    if children:
        json_node["children"] = children

    return json_node


# ----- Main JSON Provider Function ----- #
def get_tree_json(valobj):
    """
    This is the main JSON provider function that LLDB's front-end will call.
    It orchestrates the JSON creation for a Tree object.
    """
    root_node_ptr = get_child_member_by_names(valobj, ["root", "m_root", "_root"])
    if not root_node_ptr or get_raw_pointer(root_node_ptr) == 0:
        return {"kind": {"tree": True}, "root": {"name": "[Empty Tree]"}}

    json_root = _build_json_tree_node(root_node_ptr)

    # The final JSON payload expected by CodeLLDB's tree visualizer
    return {"kind": {"tree": True}, "root": json_root}


def _build_dot_for_tree(smart_ptr_sbvalue, dot_lines, visited_nodes):
    """
    Recursive helper to traverse a tree and generate Graphviz .dot content.
    """
    node_addr = get_raw_pointer(smart_ptr_sbvalue)

    if node_addr == 0 or node_addr in visited_nodes:
        return
    visited_nodes.add(node_addr)

    # Find the internal pointer member of the smart pointer.
    internal_ptr = get_child_member_by_names(
        smart_ptr_sbvalue, ["_M_ptr", "__ptr_", "pointer"]
    )
    if not internal_ptr or not internal_ptr.IsValid():
        return  # If we can't find the internal pointer, we can't proceed.

    # Dereference the internal pointer to get the actual TreeNode structure.
    node_struct = internal_ptr.Dereference()
    if not node_struct or not node_struct.IsValid():
        return

    # Get node members
    value = get_child_member_by_names(node_struct, ["value", "val", "data", "key"])
    left = get_child_member_by_names(node_struct, ["left", "m_left", "_left"])
    right = get_child_member_by_names(node_struct, ["right", "m_right", "_right"])

    val_summary = get_value_summary(value).replace('"', '\\"')

    # Define the current node
    dot_lines.append(f'  Node_{node_addr} [label="{val_summary}"];')

    # Define edge to the left child and recurse
    if left and get_raw_pointer(left) != 0:
        left_child_addr = get_raw_pointer(left)
        dot_lines.append(f"  Node_{node_addr} -> Node_{left_child_addr};")
        _build_dot_for_tree(left, dot_lines, visited_nodes)

    # Define edge to the right child and recurse
    if right and get_raw_pointer(right) != 0:
        right_child_addr = get_raw_pointer(right)
        dot_lines.append(f"  Node_{node_addr} -> Node_{right_child_addr};")
        _build_dot_for_tree(right, dot_lines, visited_nodes)


# ----- LLDB Command to Export Tree as Graphviz .dot File ----- #
def export_tree_command(debugger, command, result, internal_dict):
    """
    Implements the 'export_tree' command. It traverses a tree structure
    and writes a Graphviz .dot file to disk.
    Usage: (lldb) export_tree <variable_name> [output_file.dot]
    """
    args = command.split()
    if not args:
        result.SetError("Usage: export_tree <variable_name> [output_file.dot]")
        return

    var_name = args[0]
    output_filename = args[1] if len(args) > 1 else "tree.dot"

    frame = (
        debugger.GetSelectedTarget().GetProcess().GetSelectedThread().GetSelectedFrame()
    )
    if not frame.IsValid():
        result.SetError("Cannot execute 'export_tree': invalid execution context.")
        return

    tree_val = frame.FindVariable(var_name)
    if not tree_val or not tree_val.IsValid():
        result.SetError(f"Could not find a variable named '{var_name}'.")
        return

    # Find the root node, not a 'nodes' container
    root_node_ptr = get_child_member_by_names(tree_val, ["root", "m_root", "_root"])
    if not root_node_ptr or get_raw_pointer(root_node_ptr) == 0:
        result.AppendMessage("Tree is empty or root pointer not found.")
        return

    # Build the .dot file content recursively
    dot_lines = [
        "digraph Tree {",
        "  node [shape=circle, style=filled, fillcolor=lightgoldenrodyellow];",
    ]
    visited_nodes = set()
    _build_dot_for_tree(root_node_ptr, dot_lines, visited_nodes)
    dot_lines.append("}")
    dot_content = "\n".join(dot_lines)

    # Write the content to the output file
    try:
        with open(output_filename, "w") as f:
            f.write(dot_content)
        result.AppendMessage(f"Successfully exported tree to '{output_filename}'.")
        result.AppendMessage(
            f"Run this command to generate the image: {Colors.BOLD_CYAN}dot -Tpng {output_filename} -o tree.png{Colors.RESET}"
        )
    except IOError as e:
        result.SetError(f"Failed to write to file '{output_filename}': {e}")


# ----- Formatter for Graphs ----- #
class GraphProvider:
    """
    Provides a summary and synthetic children for an entire Graph structure.
    Displays the graph's nodes as children.
    """

    def __init__(self, valobj, internal_dict):
        self.valobj = valobj
        self.nodes_container = None
        self.update()

    def update(self):
        self.nodes_container = get_child_member_by_names(
            self.valobj, ["nodes", "m_nodes", "adj", "adjacency_list"]
        )

    def num_children(self):
        if self.nodes_container and self.nodes_container.IsValid():
            return self.nodes_container.GetNumChildren()
        return 0

    def get_child_at_index(self, index):
        if self.nodes_container:
            return self.nodes_container.GetChildAtIndex(index)
        return None

    def get_summary(self):
        num_nodes_member = get_child_member_by_names(
            self.valobj, ["num_nodes", "V", "node_count"]
        )
        num_edges_member = get_child_member_by_names(
            self.valobj, ["num_edges", "E", "edge_count"]
        )

        summary = f"{Colors.MAGENTA}Graph{Colors.RESET}"
        if num_nodes_member:
            summary += f" | V = {num_nodes_member.GetValueAsUnsigned()}"
        if num_edges_member:
            summary += f" | E = {num_edges_member.GetValueAsUnsigned()}"
        return summary


# ----- Function for Graph Nodes ----- #
def GraphNodeSummary(valobj, internal_dict):
    """
    Provides a summary for a single Graph Node, showing its value and neighbors.
    """
    # valobj is the node itself
    debug_print(f"GraphNodeSummary called for '{valobj.GetTypeName()}'")
    node_value = get_child_member_by_names(valobj, ["value", "val", "data", "key"])
    neighbors = get_child_member_by_names(valobj, ["neighbors", "adj", "edges"])

    val_str = get_value_summary(node_value)
    summary = f"{Colors.YELLOW}{val_str}{Colors.RESET}"

    if neighbors and neighbors.IsValid() and neighbors.MightHaveChildren():
        neighbor_summaries = []
        max_neighbors = g_graph_max_neighbors
        num_neighbors = neighbors.GetNumChildren()

        debug_print(
            f"-> Node '{val_str}' has {num_neighbors} neighbors. Displaying up to {max_neighbors}."
        )
        for i in range(min(neighbors.GetNumChildren(), max_neighbors)):
            neighbor_node = neighbors.GetChildAtIndex(i)

            # The neighbor might be a pointer to a node, or a node itself
            if neighbor_node.GetType().IsPointerType():
                neighbor_node = neighbor_node.Dereference()

            if neighbor_node and neighbor_node.IsValid():
                neighbor_val = get_child_member_by_names(
                    neighbor_node, ["value", "val", "data", "key"]
                )
                neighbor_summaries.append(get_value_summary(neighbor_val))

        if neighbor_summaries:
            summary += f" -> [{', '.join(neighbor_summaries)}]"
        if neighbors.GetNumChildren() > max_neighbors:
            summary += " ..."

    return summary


# ----- Custom LLDB command 'export_graph' ----- #
def export_graph_command(debugger, command, result, internal_dict):
    """
    Implements the 'export_graph' command. It traverses a graph structure
    and writes a Graphviz .dot file to disk.
    Usage: (lldb) export_graph <variable_name> [output_file.dot]
    """
    debug_print("=" * 20 + " 'export_graph' Command Start " + "=" * 20)

    # 1. Parse arguments
    args = command.split()
    if not args:
        result.SetError("Usage: export_graph <variable_name> [output_file.dot]")
        debug_print("Command failed: No variable name provided.")
        return

    var_name = args[0]
    output_filename = args[1] if len(args) > 1 else "graph.dot"
    debug_print(f"Variable: '{var_name}', Output file: '{output_filename}'")

    # 2. Get the variable from the current frame
    frame = (
        debugger.GetSelectedTarget().GetProcess().GetSelectedThread().GetSelectedFrame()
    )
    if not frame.IsValid():
        result.SetError("Cannot execute 'export_graph': invalid execution context.")
        debug_print("Command failed: Invalid frame.")
        return

    graph_val = frame.FindVariable(var_name)
    if not graph_val or not graph_val.IsValid():
        result.SetError(f"Could not find a variable named '{var_name}'.")
        debug_print(f"Command failed: Variable '{var_name}' not found.")
        return

    # 3. Access the container of nodes within the graph object
    nodes_container = get_child_member_by_names(
        graph_val, ["nodes", "m_nodes", "adj", "adjacency_list"]
    )
    if (
        not nodes_container
        or not nodes_container.IsValid()
        or not nodes_container.MightHaveChildren()
    ):
        result.AppendMessage("Graph is empty or nodes container not found.")
        debug_print("Nodes container not found or is empty.")
        return

    # 4. Traverse the graph and build the .dot file content
    dot_lines = [
        "digraph G {",
        '  rankdir="LR";',
        "  node [shape=circle, style=filled, fillcolor=lightblue];",
    ]
    edge_lines = set()  # Use a set to avoid duplicate edges
    visited_nodes = set()

    num_nodes = nodes_container.GetNumChildren()
    debug_print(f"Found {num_nodes} nodes in the container. Starting traversal...")

    for i in range(num_nodes):
        node = nodes_container.GetChildAtIndex(i)

        # If the container holds pointers, dereference them
        if node.GetType().IsPointerType():
            node = node.Dereference()

        if not node or not node.IsValid():
            continue

        node_addr = get_raw_pointer(node)

        # Define the node if not already seen
        if node_addr not in visited_nodes:
            visited_nodes.add(node_addr)
            node_value = get_child_member_by_names(
                node, ["value", "val", "data", "key"]
            )
            val_summary = get_value_summary(node_value).replace(
                '"', '\\"'
            )  # Escape quotes
            dot_lines.append(f'  Node_{node_addr} [label="{val_summary}"];')
            debug_print(f"  - Defined node Node_{node_addr} with label '{val_summary}'")

        # Find its neighbors and define edges
        neighbors = get_child_member_by_names(node, ["neighbors", "adj", "edges"])
        if neighbors and neighbors.IsValid() and neighbors.MightHaveChildren():
            for j in range(neighbors.GetNumChildren()):
                neighbor = neighbors.GetChildAtIndex(j)

                if neighbor.GetType().IsPointerType():
                    neighbor = neighbor.Dereference()

                if not neighbor or not neighbor.IsValid():
                    continue

                neighbor_addr = get_raw_pointer(neighbor)
                edge_str = f"  Node_{node_addr} -> Node_{neighbor_addr};"

                if edge_str not in edge_lines:
                    edge_lines.add(edge_str)
                    debug_print(
                        f"    - Found edge: Node_{node_addr} -> Node_{neighbor_addr}"
                    )

    dot_lines.extend(list(edge_lines))
    dot_lines.append("}")
    dot_content = "\n".join(dot_lines)

    # 5. Write the content to the output file
    try:
        with open(output_filename, "w") as f:
            f.write(dot_content)
        result.AppendMessage(f"Successfully exported graph to '{output_filename}'.")
        result.AppendMessage(
            f"Run this command to generate the image: {Colors.BOLD_CYAN}dot -Tpng {output_filename} -o graph.png{Colors.RESET}"
        )
    except IOError as e:
        result.SetError(f"Failed to write to file '{output_filename}': {e}")
        debug_print(f"File I/O Error: {e}")

    debug_print("=" * 20 + " 'export_graph' Command End " + "=" * 20)


# ----- Formatter Registration ----- #
def __lldb_init_module(debugger, internal_dict):
    """
    Entry point function that LLDB calls when this script is imported.
    """
    if lldb is None:
        print("LLDB module not available - Skipping formatter registration")
        return

    print("Loading custom LLDB formatters...")

    category_name = "CustomFormatters"
    category = debugger.GetCategory(category_name)
    if not category.IsValid():
        category = debugger.CreateCategory(category_name)

    category.SetEnabled(True)

    # Regular expressions to match type names
    list_regex = r"^(Custom|My)?(Linked)?List<.*>( &&)?$"
    stack_regex = r"^(Custom|My)?Stack<.*>( &&)?$"
    queue_regex = r"^(Custom|My)?Queue<.*>( &&)?$"
    tree_regex = r"^(Custom|My)?(Binary)?Tree<.*>( &&)?$"
    tree_node_regex = r"^(Custom|My|Bin|Binary)?(Tree)?Node<.*>( &&)?$"
    graph_regex = r"^(Custom|My)?Graph<.*>( &&)?$"
    graph_node_regex = r"^(Custom|My)?(Graph)?Node<.*>( &&)?$"

    # Register Linear Structures (Summary Provider)
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(list_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName(
            "lldb_formatters.LinearContainerSummary"
        ),
    )
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(stack_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName(
            "lldb_formatters.LinearContainerSummary"
        ),
    )
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(queue_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName(
            "lldb_formatters.LinearContainerSummary"
        ),
    )

    # ----- Tree Registration (with HTML Visualizer) - The Base64 Protocol ----- #

    # Synthetic children provider for the nodes to make them expandable
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(tree_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName(
            "lldb_formatters.tree_visualizer_provider"
        ),
    )

    # ----- Register Graph Container and Nodes ----- #
    # Synthetic children provider for the graph (to show nodes) and a basic summary
    category.AddTypeSynthetic(
        lldb.SBTypeNameSpecifier(graph_regex, True),
        lldb.SBTypeSynthetic.CreateWithClassName("lldb_formatters.GraphProvider"),
    )

    # A detailed summary for each individual graph node
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(graph_node_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName("lldb_formatters.GraphNodeSummary"),
    )

    # ----- Custom Command Registration ----- #
    # Register the specific traversal commands
    debugger.HandleCommand(
        "command script add -f lldb_formatters.pptree_preorder_command pptree_preorder"
    )
    debugger.HandleCommand(
        "command script add -f lldb_formatters.pptree_inorder_command pptree_inorder"
    )
    debugger.HandleCommand(
        "command script add -f lldb_formatters.pptree_postorder_command pptree_postorder"
    )

    # Register a convenient alias: 'pptree' will run the default Pre-Order view
    debugger.HandleCommand("command alias pptree pptree_preorder")

    # Register other commands
    debugger.HandleCommand(
        "command script add -f lldb_formatters.export_graph_command export_graph"
    )
    debugger.HandleCommand(
        "command script add -f lldb_formatters.export_tree_command export_tree"
    )
    debugger.HandleCommand(
        "command script add -f lldb_formatters.formatter_config_command formatter_config"
    )

    # ----- Final Output Message -----
    print(f"Formatters registered in category '{category_name}'.")
    print(
        "Custom commands 'pptree' (pre-order), 'pptree_inorder', 'pptree_postorder', 'export_tree', and 'formatter_config' are now available."
    )
