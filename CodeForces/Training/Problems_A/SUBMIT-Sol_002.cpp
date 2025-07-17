#include <iostream>
#include <set>

int main(void) {
    std::set<int> colors;
    int s1, s2, s3, s4;
    std::cin >> s1 >> s2 >> s3 >> s4;

    colors.insert(s1);
    colors.insert(s2);
    colors.insert(s3);
    colors.insert(s4);

    std::cout << 4 - colors.size() << std::endl;
    return 0;
}