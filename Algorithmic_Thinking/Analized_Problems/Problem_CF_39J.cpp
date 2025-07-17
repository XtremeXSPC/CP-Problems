/**
 * @file Problem_CF_39J.cpp
 * @author Costantino Lombardi
 * @brief Implementation of the solution to Codeforces problem 39J using C++.
 * @version 0.1
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Funzione per calcolare la lunghezza del prefisso comune
int calculate_prefix_length(const std::string& s1, const std::string& s2) {
  int len   = 0;
  int n1    = s1.length();
  int n2    = s2.length();
  int limit = std::min(n1, n2); // Confronta fino alla lunghezza della stringa più corta
  while (len < limit && s1[len] == s2[len]) {
    len++;
  }
  return len;
}

// Funzione per calcolare la lunghezza del suffisso comune considerando lo shift
int calculate_suffix_length_shifted(const std::string& s1, const std::string& s2) {
  int n1 = s1.length();
  int n2 = s2.length(); // Sappiamo che n2 = n1 - 1

  // Caso base, se una stringa è vuota (non dovrebbe accadere con i vincoli del problema)
  if (n1 == 0 || n2 == 0) {
    return 0;
  }

  int len    = 0;
  int s1_idx = n1 - 1; // Ultimo carattere di s1
  int s2_idx = n2 - 1; // Ultimo carattere di s2

  // Confronta da destra a sinistra
  // s1[s1_idx] con s2[s2_idx]
  // s1[s1_idx-1] con s2[s2_idx-1]
  // ...
  // Finchè s1_idx >= 0 e s2_idx >=0
  // (corrisponde alla condizione s1[i] vs s2[i-1] dove i deve essere >=1 per s2)
  // e il prefisso che stiamo ignorando in s1 non è vuoto (s1_idx > 0)
  while (s1_idx >= 1 && s2_idx >= 0 && s1[s1_idx] == s2[s2_idx]) {
    len++;
    s1_idx--;
    s2_idx--;
  }
  return len;
}

int main() {
  std::ios_base::sync_with_stdio(false); // Ottimizzazione I/O C++
  std::cin.tie(NULL);                    // Ottimizzazione I/O C++

  std::string s1, s2;
  std::cin >> s1 >> s2;

  int n1 = s1.length();
  // int n2 = s2.length(); // n2 è sempre n1 - 1

  int l_prefix = 0;
  while (l_prefix < n1 - 1 && l_prefix < (int)s2.length() && s1[l_prefix] == s2[l_prefix]) {
    l_prefix++;
  }

  int l_suffix_shifted = 0;
  int s1_ptr           = n1 - 1;       // ultimo indice di s1
  int s2_ptr           = (n1 - 1) - 1; // ultimo indice di s2

  while (s1_ptr > 0 && s2_ptr >= 0 && s1[s1_ptr] == s2[s2_ptr]) {
    l_suffix_shifted++;
    s1_ptr--;
    s2_ptr--;
  }

  int pos_min_1_indexed = n1 - l_suffix_shifted;
  int pos_max_1_indexed = l_prefix + 1;

  std::vector<int> result_positions;
  if (pos_min_1_indexed <= pos_max_1_indexed) {
    for (int k = pos_min_1_indexed; k <= pos_max_1_indexed; ++k) {
      result_positions.push_back(k);
    }
  }

  if (result_positions.empty()) {
    std::cout << 0 << std::endl;
  } else {
    std::cout << result_positions.size() << std::endl;
    for (size_t i = 0; i < result_positions.size(); ++i) {
      std::cout << result_positions[i] << (i == result_positions.size() - 1 ? "" : " ");
    }
    std::cout << std::endl;
  }

  return 0;
}