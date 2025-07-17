/**
 * @file Problem_lkp18c2p1.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to DMOJ problem lkp18c2p1.
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

#define MAX_LINES 100

/* Function Prototipes */
int  shortest_line_index(int lines[], int n);
void solve(int lines[], int n, int m);

int shortest_line_index(int lines[], int n) {
  int j;
  int shortest = 0;
  for (j = 1; j < n; j++) {
    if (lines[j] < lines[shortest]) {
      shortest = j;
    }
  }
  return shortest;
}

void solve(int lines[], int n, int m) {
  int i, shortest;
  for (i = 0; i < m; i++) {
    shortest = shortest_line_index(lines, n);
    printf("%d\n", lines[shortest]);
    lines[shortest]++;
  }
}

/* Main function */
int main(void) {
  int lines[MAX_LINES];
  int n, m, i;
  scanf("%d%d", &n, &m);
  for (i = 0; i < n; i++) {
    scanf("%d", &lines[i]);
  }
  solve(lines, n, m);
  return 0;
}
