/* A. System of Equations
 * Time limit per test: 2 seconds
 * Memory limit per test: 256 megabytes
 * 
 * Furik loves math lessons very much, so he doesn't attend them, unlike Rubik. But now Furik wants to get a good 
 * mark for math. For that Ms. Ivanova, his math teacher, gave him a new task. Furik solved the task immediately. Can you?
 * 
 * You are given a system of equations:
 *      {a^2 + b = n
 *      {a + b^2 = m
 * 
 * You should count, how many there are pairs of integers (a, b) (0 ≤ a,b) which satisfy the system.
 * 
 * Input
 * A single line contains two integers n, m (1 ≤ n, m ≤ 1000) — the parameters of the system.
 * The numbers on the line are separated by a space.
 * 
 * Output
 * On a single line print the answer to the problem.
 * */

#include <iostream>

int main(void) {
    int n, m, count = 0;
    std::cin >> n >> m;

    for (int a = 0; a * a <= n; a++) {
        for (int b = 0; b * b <= m; b++) {
            if ((a * a + b == n) && (a + b * b == m)) {
                count++;
            }
        }
    }

    std::cout << count << std::endl;
    return 0;

}