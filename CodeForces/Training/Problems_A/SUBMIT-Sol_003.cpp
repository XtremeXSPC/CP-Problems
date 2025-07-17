#include <iostream>
#include <set>

int main(void) {
    std::set<char> characters;
    std::string username;
    std::cin >> username;

    for (int i = 0; i < username.size(); i++) {
        characters.insert(username[i]);
    }

    if (characters.size() % 2 == 0) {
        std::cout << "CHAT WITH HER!" << std::endl;
    } else {
        std::cout << "IGNORE HIM!" << std::endl;
    }

    return 0;
}