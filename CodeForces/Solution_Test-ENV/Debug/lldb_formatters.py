# lldb_formatters.py
#
# A generic set of Data Formatters for LLDB, designed to work with custom C++ 
# implementations of common data structures.It adapts by searching for common
# member names instead of requiring specific ones.

try:
    import lldb # type: ignore
except ImportError:
    # lldb module is only available when running within LLDB debugger
    # This allows the file to be imported without errors in other contexts
    lldb = None
import re

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
    
    if value.GetType().IsPointerType():
        return value.GetValueAsUnsigned()
    
    # For smart pointers like shared_ptr or unique_ptr (common names for the internal pointer).
    if value.GetChildMemberWithName("pointer").IsValid():
        return value.GetChildMemberWithName("pointer").GetValueAsUnsigned()

    # Fallback to common internal names
    # libc++ (macOS/Clang) uses a member like '__m_p' inside a tuple.
    ptr_member = get_child_member_by_names(value, ['__m_p','_M_ptr', '__ptr_', 'pointer'])
    if ptr_member and ptr_member.IsValid():
        return ptr_member.GetValueAsUnsigned()
        
    return value.GetValueAsUnsigned() # Fallback

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

    def update(self):
        """
        Initializes pointers and member names by searching through common conventions.
        """
        self.head_ptr = get_child_member_by_names(self.valobj, ['head', 'm_head', '_head', 'top'])
        
        # Determine the node's member names from the type of the first node
        if self.head_ptr and get_raw_pointer(self.head_ptr) != 0:
            # Get the actual type pointed to, handling pointers and smart pointers
            node_type = self.head_ptr.GetType()
            if node_type.IsPointerType() or node_type.IsReferenceType():
                node_type = node_type.GetPointeeType()

            # Dereference smart pointers to get to the actual Node type
            if not (node_type.IsPointerType() or node_type.IsReferenceType()):
                 if node_type.GetNumberOfTemplateArguments() > 0:
                     node_type = node_type.GetTemplateArgumentType(0)
            
            # Look for the 'next' pointer
            for name in ['next', 'm_next', '_next', 'pNext']:
                if node_type.GetFieldWithName(name):
                    self.next_ptr_name = name
                    break
            
            # Look for the 'value' member
            for name in ['value', 'val', 'data', 'm_data', 'key']:
                if node_type.GetFieldWithName(name):
                    self.value_name = name
                    break
        
        # Look for a size member
        size_member = get_child_member_by_names(self.valobj, ['size', 'm_size', '_size', 'count'])
        if size_member:
            self.size = size_member.GetValueAsUnsigned()

    def get_summary(self):
        self.update()
        
        if not self.head_ptr:
            return "Could not find head pointer"
        
        if get_raw_pointer(self.head_ptr) == 0:
            return f"size={self.size}, []"

        if not self.next_ptr_name or not self.value_name:
            return "Cannot determine node structure (val/next)"

        summary = ""
        node = self.head_ptr
        count = 0
        max_items = 15 # Limit to avoid excessively long output
        
        # Set for cycle detection
        visited = set()

        while get_raw_pointer(node) != 0 and count < max_items:
            node_addr = get_raw_pointer(node)
            if node_addr in visited:
                summary += " ... [CYCLE DETECTED]"
                break
            visited.add(node_addr)

            value_child = node.GetChildMemberWithName(self.value_name)
            summary += str(value_child.GetSummary() or value_child.GetValue())
            
            node = node.GetChildMemberWithName(self.next_ptr_name)
            if get_raw_pointer(node) != 0:
                summary += " -> "
            count += 1
        
        if count == max_items and get_raw_pointer(node) != 0:
            summary += " ..."

        return f"size={self.size}, [{summary}]"

def LinearContainerSummary(valobj, internal_dict):
    """
    This function is registered with LLDB. It creates an instance of our
    provider class and calls its get_summary() method.
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
        # First, check if it's an N-ary tree (has a 'children' container)
        self.children_member = get_child_member_by_names(self.valobj, ['children', 'm_children'])
        if self.children_member and self.children_member.IsValid() and self.children_member.MightHaveChildren():
            self.is_nary = True
            self.num_children_val = self.children_member.GetNumChildren()
        else:
            # Otherwise, check if it's a binary tree
            self.is_nary = False
            self.left_member = get_child_member_by_names(self.valobj, ['left', 'm_left', '_left'])
            self.right_member = get_child_member_by_names(self.valobj, ['right', 'm_right', '_right'])
            
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
            if self.children_member is not None:
                return self.children_member.GetChildAtIndex(index)
            else:
                return None    
        else: # Binary
            current_index = 0
            if self.left_member and get_raw_pointer(self.left_member) != 0:
                if index == current_index:
                    return self.left_member
                current_index += 1
            if self.right_member and get_raw_pointer(self.right_member) != 0:
                if index == current_index:
                    # If left is null, the first child is the right one
                    return self.right_member
        return None

    def get_summary(self):
        # Provides a summary for the node itself
        value_member = get_child_member_by_names(self.valobj, ['value', 'val', 'data', 'key'])
        if value_member and value_member.IsValid():
            return f"value = {value_member.GetSummary() or value_member.GetValue()}"
        return "Node"

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
        # Looks for a container of nodes (e.g., a vector or an adjacency list)
        self.nodes_container = get_child_member_by_names(self.valobj, ['nodes', 'adj', 'adjacency_list'])
    
    def num_children(self):
        if self.nodes_container and self.nodes_container.IsValid():
            return self.nodes_container.GetNumChildren()
        return 0

    def get_child_at_index(self, index):
        if self.nodes_container:
            return self.nodes_container.GetChildAtIndex(index)
        return None

    def get_summary(self):
        num_nodes_member = get_child_member_by_names(self.valobj, ['num_nodes', 'V'])
        num_edges_member = get_child_member_by_names(self.valobj, ['num_edges', 'E'])
        
        summary = "Graph"
        if num_nodes_member:
            summary += f" | V={num_nodes_member.GetValueAsUnsigned()}"
        if num_edges_member:
            summary += f" | E={num_edges_member.GetValueAsUnsigned()}"
        return summary

# ----- Formatter Registration ----- #

def __lldb_init_module(debugger, internal_dict):
    """
    Entry point function that LLDB calls when this script is imported.
    """
    if lldb is None:
        print("LLDB module not available - Skipping formatter registration")
        return
        
    print("Loading custom LLDB formatters...")

    # Categories to organize the formatters
    category_name = "CustomFormatters"
    category = debugger.GetCategory(category_name)
    if not category.IsValid():
        category = debugger.CreateCategory(category_name)
    
    category.SetEnabled(True)

    # Regular expressions to match type names
    # This allows matching "LinkedList", "MyLinkedList", "CustomList", etc.
    list_regex      = r"^(Custom|My)?(Linked)?List<.*>( &&)?$"
    stack_regex     = r"^(Custom|My)?Stack<.*>( &&)?$"
    queue_regex     = r"^(Custom|My)?Queue<.*>( &&)?$"
    tree_node_regex = r"^(Custom|My|Bin|Binary)?(Tree)?Node<.*>( &&)?$"
    graph_regex     = r"^(Custom|My)?Graph<.*>( &&)?$"

    # Register providers for the various structures
    # A Summary Provider gives a one-line summary.
    # A Synthetic Children Provider allows expanding the object.
    
    # Linear Structures
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(list_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName("lldb_formatters.LinearContainerSummary")
    )
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(stack_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName("lldb_formatters.LinearContainerSummary")
    )
    category.AddTypeSummary(
        lldb.SBTypeNameSpecifier(queue_regex, True),
        lldb.SBTypeSummary.CreateWithFunctionName("lldb_formatters.LinearContainerSummary")
    )

    # Tree Nodes (and their children)
    category.AddTypeSynthetic(
        lldb.SBTypeNameSpecifier(tree_node_regex, True),
        lldb.SBTypeSynthetic.CreateWithClassName("lldb_formatters.GenericTreeProvider")
    )

    # Graph (to display the node list)
    category.AddTypeSynthetic(
        lldb.SBTypeNameSpecifier(graph_regex, True),
        lldb.SBTypeSynthetic.CreateWithClassName("lldb_formatters.GraphProvider")
    )

    print(f"Formatters registered in category '{category_name}'.")

