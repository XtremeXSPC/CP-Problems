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