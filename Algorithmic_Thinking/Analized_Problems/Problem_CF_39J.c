/**
 * @file Problem_CF_39J.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to Codeforces problem 39J.
 * @version 0.1
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 1000000

// Macros

/* Types */

/* Function prototypes */
int prefix_length(char s1[], char s2[]);
int suffix_length(char s1[], char s2[], int len);

/* Functions definitions */
int prefix_length(char s1[], char s2[]) {
  int i = 1;
  while (s1[i] == s2[i])
    i++;
  return i - 1;
}

int suffix_length(char s1[], char s2[], int len) {
  int i = len;
  while (i >= 2 && s1[i] == s2[i - 1])
    i--;
  return len - i;
}

/* Main function */
int main(void) {

  static char s1[SIZE + 2], s2[SIZE + 2];
  int         len, prefix, suffix, total;

  /* Deprecated gets() */
  // gets(&s1[1]);
  // gets(&s2[1]);

  /* Using either gets() or fgets() leads to a
   * Wrong Answer (WA) when submitted to the tester.
   * The issue likely lies in how the this funciton
   * interacts with string inputs.
   */
  fgets(&s1[1], sizeof(s1) - 1, stdin);
  fgets(&s2[1], sizeof(s2) - 1, stdin);

  char* newline = strchr(&s1[1], '\n');
  if (newline)
    *newline = '\0';

  newline = strchr(&s2[1], '\n');
  if (newline)
    *newline = '\0';

  len    = strlen(&s1[1]);
  prefix = prefix_length(s1, s2);
  suffix = suffix_length(s1, s2, len);
  total  = (prefix + 1) - (len - suffix) + 1;
  if (total < 0)
    total = 0;

  printf("%d\n", total);
  for (int i = 0; i < total; i++) {
    printf("%d", i + len - suffix);
    if (i < total - 1)
      printf("%s", " ");
    else
      printf("%s", "\n");
  }

  /* Here is some code that shows how the prefix1 array can be built:

  unsigned long long prefix1[1000001];
  prefix1[0] = 0;
  for (int i = 1; i <= strlen(first_string); i++)
    prefix1[i] = prefix1[i - 1] * 39 + first_string[i];

 */

  return 0;
}
