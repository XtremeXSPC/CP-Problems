/* A. Boy or Girl
 * Time limit per test: 1 second
 * Memory limit per test: 256 megabytes
 * 
 * Those days, many boys use beautiful girls' photos as avatars in forums. So it is pretty hard to tell the gender
 * of a user at the first glance. Last year, our hero went to a forum and had a nice chat with a beauty (he thought so).
 * After that they talked very often and eventually they became a couple in the network.
 * 
 * But yesterday, he came to see "her" in the real world and found out "she" is actually a very strong man! Our hero is very
 * sad and he is too tired to love again now. So he came up with a way to recognize users' genders by their user names.
 * 
 * This is his method: if the number of distinct characters in one's user name is odd, then he is a male, otherwise she is
 * a female. You are given the string that denotes the user name, please help our hero to determine the gender of this user 
 * by his method.
 * 
 * Input
 * The first line contains a non-empty string, that contains only lowercase English letters — the user name. 
 * This string contains at most 100 letters.
 * 
 * Output
 * If it is a female by our hero's method, print "CHAT WITH HER!" (without the quotes), otherwise,
 * print "IGNORE HIM!" (without the quotes).
 * */

#include <iostream>
#include <set>

int main(void) {
    std::set<char> characters;
    std:: string username;
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