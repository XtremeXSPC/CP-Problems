/**
 * @file Problem_cco18p1_Rec.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to problem DMOJ cco18p1.
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
#define SIZE 1000

//===------------------------ Functions Prototypes ----------------------------===//
int max(int v1, int v2);
int solve(char outcome1[], char outcome2[], int goals1[], int goals2[], int i, int j);

//===------------------------ Functions Definitions ---------------------------===//
int max(int v1, int v2) {
  if (v1 > v2)
    return v1;
  else
    return v2;
}

int solve(char outcome1[], char outcome2[], int goals1[], int goals2[], int i, int j) {
  int first, second, third, fourth;
  if (i == 0 || j == 0)
    return 0;
  if ((outcome1[i] == 'W' && outcome2[j] == 'L' && goals1[i] > goals2[j])
      || (outcome1[i] == 'L' && outcome2[j] == 'W' && goals1[i] < goals2[j]))
    first = solve(outcome1, outcome2, goals1, goals2, i - 1, j - 1) + goals1[i] + goals2[j];
  else
    first = 0;
  second = solve(outcome1, outcome2, goals1, goals2, i - 1, j - 1);
  third  = solve(outcome1, outcome2, goals1, goals2, i - 1, j);
  fourth = solve(outcome1, outcome2, goals1, goals2, i, j - 1);
  return max(first, max(second, max(third, fourth)));
}

//===------------------------ main() function ---------------------------------===//
int main(void) {
  int  i, n, result;
  char outcome1[SIZE + 1], outcome2[SIZE + 1];
  int  goals1[SIZE + 1], goals2[SIZE + 1];
  scanf("%d ", &n);
  for (i = 1; i <= n; i++)
    scanf("%c", &outcome1[i]);
  for (i = 1; i <= n; i++)
    scanf("%d ", &goals1[i]);
  for (i = 1; i <= n; i++)
    scanf("%c", &outcome2[i]);
  for (i = 1; i <= n; i++)
    scanf("%d", &goals2[i]);
  result = solve(outcome1, outcome2, goals1, goals2, n, n);
  printf("%d\n", result);
  return 0;
}
