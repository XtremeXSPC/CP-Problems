/**
 * @file Problem_UVa_10465_Rec.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to problem UVa 10465.
 * In this case "recursion" was used.
 * @version 0.1
 * @date 2025-05-21
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//===------------------------ Types and Costants ------------------------------===//
unsigned long long total_calls;

//===------------------------ Functions Prototypes ----------------------------===//
int  max(int first, int second);
int  solve_t(int m, int n, int t);
void solve(int m, int n, int t);

//===------------------------ Functions Definitions ---------------------------===//
int max(int v1, int v2) {
  if (v1 > v2)
    return v1;
  else
    return v2;
}

int solve_t(int m, int n, int t) {
  int first, second;
  total_calls++;
  if (t == 0)
    return 0;
  if (t >= m)
    first = solve_t(m, n, t - m);
  else
    first = -1;
  if (t >= n)
    second = solve_t(m, n, t - n);
  else
    second = -1;
  if (first == -1 && second == -1)
    return -1;
  else
    return max(first, second) + 1;
}

void solve(int m, int n, int t) {
  int result, i;
  total_calls = 0;
  result      = solve_t(m, n, t);
  if (result >= 0)
    printf("%d\n", result);
  else {
    i      = t - 1;
    result = solve_t(m, n, i);
    while (result == -1) {
      i--;
      result = solve_t(m, n, i);
    }
    printf("%d %d\n", result, t - i);
  }
  printf("Total calls to solve_t: %llu\n", total_calls);
}

//===------------------------ main() function ---------------------------------===//

int main(void) {
  int m, n, t;
  while (scanf("%d%d%d", &m, &n, &t) != -1)
    solve(m, n, t);
  return 0;
}
