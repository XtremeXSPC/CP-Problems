/* A. Perfect Permutation
 * Time limit per test: 2 seconds
 * Memory limit per test: 256 megabytes
 * A permutation is a sequence of integers p-1, p-2, ..., p-n, consisting of n distinct positive integers, each of them doesn't 
 * exceed n. Let's denote the i-th element of permutation p as p-i. We'll call number n the size of permutation p-1, p-2, ..., p-n.
 * 
 * Nickolas adores permutations. He likes some permutations more than the others. He calls such permutations perfect.
 * A perfect permutation is such permutation p that for any i (1 ≤  i ≤ n) (n is the permutation size) the following 
 * equations hold p_p-i = i and p-i ≠ i. Nickolas asks you to print any perfect permutation of size n for the given n.
 * 
 * Input
 * A single line contains a single integer n (1 ≤ n ≤ 100) — the permutation size.
 * 
 * Output
 * If a perfect permutation of size n doesn't exist, print a single integer -1. 
 * Otherwise print n distinct integers from 1 to n, p-1, p-2, ..., p-n, permutation p, that is perfect. 
 * Separate printed numbers by whitespaces.
 * */

#include <iostream>
#include <vector>

int main(void) {
    int n;
    std::cin >> n;

    if (n % 2 != 0) {
        std::cout << -1 << std::endl;
    } else {
        std::vector<int> permutation(n);
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                permutation[i] = i + 2;
            } else {
                permutation[i] = i;
            }
        }

        for (int i = 0; i < n; i++) {
            std::cout << permutation[i] << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}