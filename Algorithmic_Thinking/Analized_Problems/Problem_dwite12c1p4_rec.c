/**
 * @file Problem_dwite12c1p4.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to DMOJ problem dwite12c1p4.
 * This is a recursive solution.
 * @version 0.1
 * @date 2025-05-20
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 255
#define TEST_CASE 5

// Macros

/* Types */
typedef struct node {
  int          candy;
  struct node *left, *right;
} node;

/* Function prototypes */
node* new_house(int candy);
node* new_nonhouse(node* left, node* right);

// Helpers
int max(int v1, int v2);

// Trees functions
int  tree_candy(node* tree);
int  tree_nodes(node* tree);
int  tree_leaves(node* tree);
int  tree_streets(node* tree);
int  tree_height(node* tree);
void tree_solve(node* tree);

node* read_tree(char* line);
node* read_tree_helper(char* line, int* pos);

/* Function Definitions */
node* new_house(int candy) {
  node* house = (node*)malloc(sizeof(node));
  if (house == NULL) {
    fprintf(stderr, "malloc() error!\n");
    exit(1);
  }
  house->candy = candy;
  house->left  = NULL;
  house->right = NULL;
  return house;
}

node* new_nonhouse(node* left, node* right) {
  node* nonhouse = (node*)malloc(sizeof(node));
  if (nonhouse == NULL) {
    fprintf(stderr, "malloc() error!\n");
    exit(1);
  }
  nonhouse->left  = left;
  nonhouse->right = right;
  return nonhouse;
}

int max(int v1, int v2) {
  if (v1 > v2)
    return v1;
  else
    return v2;
}

int tree_candy(node* tree) {
  if (!tree->left && !tree->right)
    return tree->candy;
  return tree_candy(tree->left) + tree_candy(tree->right);
}

int tree_nodes(node* tree) {
  if (!tree->left && !tree->right) {
    return 1;
  }
  return 1 + tree_nodes(tree->left) + tree_nodes(tree->right);
}

int tree_leaves(node* tree) {
  if (!tree->left && !tree->right) {
    return 1;
  }
  return tree_leaves(tree->left) + tree_leaves(tree->right);
}

int tree_streets(node* tree) {
  if (!tree->left && !tree->right) {
    return 0;
  }
  return tree_streets(tree->left) + tree_streets(tree->right) + 4;
}

int tree_height(node* tree) {
  if (!tree->left && !tree->right) {
    return 0;
  }
  return 1 + max(tree_height(tree->left), tree_height(tree->right));
}

void tree_solve(node* tree) {
  int candy       = tree_candy(tree);
  int height      = tree_height(tree);
  int num_streets = tree_streets(tree) - height;
  printf("%d %d\n", num_streets, candy);
}

/* Wrong approach!
node* read_tree(char* line) {
  node* tree;
  tree = (node*)malloc(sizeof(node));
  if (line[0] == '(') {
    tree->left  = reat_tree(&line[1]);
    // What is the argument here?
    // We need an helper function!
    tree->right = read_tree(???);
    return tree;
  }
}
*/

node* read_tree(char* line) {
  int pos = 0;
  return read_tree_helper(line, &pos);
}

node* read_tree_helper(char* line, int* pos) {
  node* tree;
  tree = (node*)malloc(sizeof(node));
  if (tree == NULL) {
    fprintf(stderr, "malloc() error!\n");
    exit(1);
  }
  if (line[*pos] == '(') {
    (*pos)++;
    tree->left = read_tree_helper(line, pos);
    (*pos)++;
    tree->right = read_tree_helper(line, pos);
    (*pos)++;
    return tree;
  } else {
    tree->left  = NULL;
    tree->right = NULL;
    tree->candy = line[*pos] - '0';
    (*pos)++;
    if (line[*pos] != ')' && line[*pos] != ' ' && line[*pos] != '\0') {
      tree->candy = (tree->candy * 10) + line[*pos] - '0';
      (*pos)++;
    }
    return tree;
  }
}

/* Main function */
int main(void) {
  /* Testing the solution */
  int   i;
  char  line[SIZE + 1];
  node* tree;

  // Get input from stdin
  // gets(line); DEPRECATED

  for (i = 0; i < TEST_CASE; i++) {
    fgets(line, SIZE + 1, stdin);
    line[strcspn(line, "\n")] = '\0';

    // Solver
    tree = read_tree(line);
    tree_solve(tree);
  }
  return 0;
}

/* main() for Debugging
int main(void) {
  // Test manual tree creation
  printf("=== Testing manual tree creation ===\n");

  // Create a simple tree: (1 2)
  node* house1 = new_house(1);
  node* house2 = new_house(2);
  node* tree1  = new_nonhouse(house1, house2);

  printf("Tree1 stats:\n");
  printf("Candy: %d\n", tree_candy(tree1));
  printf("Nodes: %d\n", tree_nodes(tree1));
  printf("Leaves: %d\n", tree_leaves(tree1));
  printf("Height: %d\n", tree_height(tree1));
  printf("Streets: %d\n", tree_streets(tree1));
  printf("Solution: ");
  tree_solve(tree1);

  // Test string parsing
  printf("\n=== Testing tree parsing from strings ===\n");

  char* test_cases[] = {"(1 2)", "(3 4)", "((1 2) 3)", "(1 (2 3))", "((1 2) (3 4))"};

  for (int i = 0; i < 5; i++) {
    printf("\nParsing: %s\n", test_cases[i]);
    node* parsed_tree = read_tree(test_cases[i]);
    printf("Tree stats:\n");
    printf("Candy: %d\n", tree_candy(parsed_tree));
    printf("Nodes: %d\n", tree_nodes(parsed_tree));
    printf("Leaves: %d\n", tree_leaves(parsed_tree));
    printf("Height: %d\n", tree_height(parsed_tree));
    printf("Streets: %d\n", tree_streets(parsed_tree));
    printf("Solution: ");
    tree_solve(parsed_tree);

    // Free memory for parsed tree (ideally should have a free_tree function)
  }

  // Free memory for manually created tree (ideally should have a free_tree function)

  return 0;

  */