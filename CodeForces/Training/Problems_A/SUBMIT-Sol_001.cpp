#include <iostream>

int main(void) {
    int n, solution = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int petya, vasya, tonya;
        std::cin >> petya >> vasya >> tonya;
        
        int sum = petya + vasya + tonya;
        if (petya + vasya + tonya >= 2) {
            solution++;
        }
    }

    std::cout << solution << std::endl;
    return 0;
}