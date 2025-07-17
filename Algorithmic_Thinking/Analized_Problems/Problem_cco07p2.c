/**
 * @file Problem_cco07p2.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to DMOJ problem cco07p2.
 * @version 0.1
 * @date 2025-05-18
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100000

typedef struct snowflake_node {
  int                    snowflake[6];
  struct snowflake_node* next;
} snowflake_node;

/* Function Prototipes */
// void identify_identical(int values[], int n);
// void identify_identical(int snowflakes[][6], int n);
void identify_identical(snowflake_node* snowflakes[]);

int identical_right(int snow1[], int snow2[], int start);
int identical_left(int snow1[], int snow2[], int start);
int are_identical(int snow1[], int snow2[]);

// int compare(const void* first, const void* second);
int code(int snowflake[]);

/* We need to adapt this function to correctly chek the
 * equality of the arrays.
 *
void identify_identical(int values[], int n) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (values[i] == values[j]) {
        printf("%s", "Twin integers found.\n");
        return;
      }
    }
  }
  printf("%s", "No two integers are alike.\n");
}
 */

/* We need to adapt the function "identify_identical"
 * to work properly with linked lists that are accessed
 * using a pointer stored in a one dimensional array.
 *
void identify_identical(int snowflakes[][6], int n) {
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (are_identical(snowflakes[i], snowflakes[j])) {
        printf("%s", "Twin snowflakes found.\n");
        return;
      }
    }
  }
  printf("%s", "No two snowflakes are alike.\n");
}
 */

void identify_identical(snowflake_node* snowflakes[]) {
  snowflake_node *node1, *node2;
  int             i;
  for (i = 0; i < SIZE; i++) {
    node1 = snowflakes[i];
    while (node1 != NULL) {
      node2 = node1->next;
      while (node2 != NULL) {
        if (are_identical(node1->snowflake, node2->snowflake)) {
          printf("%s", "Twin snowflakes found.\n");
          return;
        }
        node2 = node2->next;
      }
      node1->next;
    }
  }
  printf("%s", "No two snowflakes are alike.\n");
}

/* As explaind in the reference book, this solution won't work.
 * The problem here is the bad index for snow2.
 *
int identical_right(int snow1[], int snow2[], int start) {
  int offset;
  for (offset = 0; offset < 6; offset++) {
    if (snow1[offset] != snow2[start + offset]) {
      return 0;
    }
  }
  return 1;
}
 */

/* Another good improvement we can make is just simply
 * use the % operator to get the correct index for
 * snow2 array.
 *
int identical_right(int snow1[], int snow2[], int start) {
  int offset, snow2_index;
  for (offset = 0; offset < 6; offset++) {
    snow2_index = start + offset;
    if (snow2_index >= 6) {
      snow2_index = snow2_index - 6;
      if (snow1[offset] != snow2[start + offset])
        return 0;
    }
  }
  return 1;
}
 */

int identical_right(int snow1[], int snow2[], int start) {
  int offset;
  for (offset = 0; offset < 6; offset++) {
    if (snow1[offset] != snow2[(start + offset) % 6]) {
      return 0;
    }
  }
  return 1;
}

int identical_left(int snow1[], int snow2[], int start) {
  int offset, snow2_index;
  for (offset = 0; offset < 6; offset++) {
    snow2_index = start - offset;
    if (snow2_index < 0) {
      snow2_index = snow2_index + 6;
      if (snow1[offset] != snow2[snow2_index])
        return 0;
    }
  }
  return 1;
}

int are_identical(int snow1[], int snow2[]) {
  int start;
  for (start = 0; start < 6; start++) {
    if (identical_right(snow1, snow2, start))
      return 1;
    if (identical_left(snow1, snow2, start))
      return 1;
  }
  return 0;
}

/* This function unfortunately does not solve our problem.
 * Here we need to define an aribitrary rule that can tell us
 * which snowflake is less than or greather than the other
 * (which is not so siple), so we have to try something else.
 *
int compare(const void* first, const void* second) {
  int        i;
  const int* snowflake1 = first;
  const int* snowflake2 = second;
  if (are_identical(snowflake1, snowflake2))
    return 0;
  for (i = 0; i < 6; i++) {
    if (snowflake1[i] < snowflake2[i])
      return -1;
  }
  return 1;
}
 */

int code(int snoflake[]) {
  return (snoflake[0] + snoflake[1] + snoflake[2] + snoflake[3] + snoflake[4] + snoflake[5]) % SIZE;
}

/* Main function */
int main(void) {
  // static int snowflakes[SIZE][6];
  static snowflake_node* snowflakes[SIZE] = {NULL};
  snowflake_node*        snow;
  int                    n, i, j, snowflake_code;
  scanf("%d", &n);

  for (i = 0; i < n; i++) {
    snow = malloc(sizeof(snowflake_node));
    if (snow == NULL) {
      fprintf(stderr, "malloc() error!\n");
    }
    for (j = 0; j < n; j++)
      scanf("%d", &snow->snowflake[j]);
    snowflake_code             = code(snow->snowflake);
    snow->next                 = snowflakes[snowflake_code];
    snowflakes[snowflake_code] = snow;
  }

  identify_identical(snowflakes);

  /*
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      scanf("%d", &snowflakes[i][j]);
    }
    identify_identical(snowflakes, n);
  }
  */

  return 0;
}
