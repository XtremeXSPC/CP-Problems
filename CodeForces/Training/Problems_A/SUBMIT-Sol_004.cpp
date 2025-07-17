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