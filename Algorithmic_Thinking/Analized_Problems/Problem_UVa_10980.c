/**
 * @file Problem_UVa_10980_Rec.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to problem UVa 10980.
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
#define SIZE 200
#define MAX_SCHEMES 20

//===------------------------ Functions Prototypes ----------------------------===//
double min(double v1, double v2);
double solve_k(int num[], double price[], int num_schemes, double unit_price, int num_apples);
double solve(int num[], double price[], int num_schemes, double unit_price, int num_apples);
int    get_number(int* num);

//===------------------------ Functions Definitions ---------------------------===//
double min(double v1, double v2) {
  if (v1 < v2)
    return v1;
  else
    return v2;
}

double solve_k(int num[], double price[], int num_schemes, double unit_price, int num_apples) {
  double best, result;
  int    i;
  if (num_apples == 0)
    return 0;
  else {
    result = solve_k(num, price, num_schemes, unit_price, num_apples - 1);
    best   = result + unit_price;
    for (i = 0; i < num_schemes; i++)
      if (num_apples - num[i] >= 0) {
        result = solve_k(num, price, num_schemes, unit_price, num_apples - num[i]);
        best   = min(best, result + price[i]);
      }
    return best;
  }
}

double solve(int num[], double price[], int num_schemes, double unit_price, int num_apples) {
  double best;
  int    i;
  best = solve_k(num, price, num_schemes, unit_price, num_apples);
  for (i = num_apples + 1; i < SIZE; i++) {
    best = min(best, solve_k(num, price, num_schemes, unit_price, i));
  }
  return best;
}

int get_number(int* num) {
  int ch;
  int ret = 0;
  ch      = getchar();
  if (ch == EOF) {
    *num = ret;
    return 0;
  }
  while (ch != ' ' && ch != '\n' && ch != EOF) {
    ret = ret * 10 + ch - '0';
    ch  = getchar();
  }
  *num = ret;
  return ch == ' ';
}

//===------------------------ main() function ---------------------------------===//
int main(void) {
  int    test_case, num_schemes, num_apples, more, i;
  double unit_price, result;
  int    num[MAX_SCHEMES];
  double price[MAX_SCHEMES];
  test_case = 0;
  while (scanf("%lf%d ", &unit_price, &num_schemes) != -1) {
    test_case++;
    for (i = 0; i < num_schemes; i++)
      scanf("%d%lf ", &num[i], &price[i]);
    printf("Case %d:\n", test_case);
    more = get_number(&num_apples);
    while (more) {
      result = solve(num, price, num_schemes, unit_price, num_apples);
      printf("Buy %d for $%.2f\n", num_apples, result);
      more = get_number(&num_apples);
    }
    result = solve(num, price, num_schemes, unit_price, num_apples);
    printf("Buy %d for $%.2f\n", num_apples, result);
  }
  return 0;
}
