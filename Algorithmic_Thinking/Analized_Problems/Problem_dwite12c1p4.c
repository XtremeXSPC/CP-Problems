/**
 * @file Problem_dwite12c1p4.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to DMOJ problem dwite12c1p4.
 * This is an iterative solution that involve stacks.
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

// Macros

/* Types */
typedef struct node {
  int          candy;
  struct node *left, *right;
} node;

typedef struct stack {
  node* values[SIZE];
  int   highest_used;
} stack;

/* Function prototypes */
node* new_house(int candy);
node* new_nonhouse(node* left, node* right);

stack* new_stack(void);
void   push_stack(stack* s, node* value);
node*  pop_stack(stack* s);
int    is_empty_stack(stack* s);

// Helpers
int tree_candy(node* tree);

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

stack* new_stack(void) {
  stack* s = (stack*)malloc(sizeof(stack));
  if (s == NULL) {
    fprintf(stderr, "malloc() error!\n");
    exit(1);
  }
  s->highest_used = -1;
  return s;
}

void push_stack(stack* s, node* value) {
  s->highest_used++;
  s->values[s->highest_used] = value;
}

node* pop_stack(stack* s) {
  node* ret = s->values[s->highest_used];
  s->highest_used--;
  return ret;
}

int is_empty_stack(stack* s) {
  return s->highest_used == -1;
}

int tree_candy(node* tree) {
  int    total = 0;
  stack* s     = new_stack();
  while (tree != NULL) {
    if (tree->left && tree->right) {
      push_stack(s, tree->left);
      tree = tree->right;
    }
    total = total + tree->candy;

    if (is_empty_stack(s))
      tree = NULL;
    else
      tree = pop_stack(s);
  }
  return total;
}

/* Main function */
int main(void) {

  /* Example of using the Stack */
  stack* s;
  s = new_stack();
  node *n, *n1, *n2, *n3;
  n1 = new_house(20);
  n2 = new_house(30);
  n3 = new_house(10);
  push_stack(s, n1);
  push_stack(s, n2);
  push_stack(s, n3);
  while (!is_empty_stack(s)) {
    n = pop_stack(s);
    printf("%d\n", n->candy);
  }

  /* Use of helper functions
  node* four    = new_house(4);
  node* nine    = new_house(9);
  node* B       = new_nonhouse(four, nine);
  node* fifteen = new_house(15);
  node* C       = new_nonhouse(B, fifteen);
  */

  /* Example of how construct House Nodes
  node* four  = malloc(sizeof(node));
  four->candy = 4;
  four->left  = NULL;
  four->right = NULL;

  node* nine  = malloc(sizeof(node));
  four->candy = 9;
  four->left  = NULL;
  four->right = NULL;

  node* B  = malloc(sizeof(node));
  B->left  = four;
  B->right = nine;
 */

  return 0;
}
