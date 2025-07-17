/**
 * @file hash-example.c
 * @author Costantino Lombardi
 * @brief An example of a hash function.
 * @version 0.1
 * @date 2025-05-19
 *
 * @copyright Copyright (c) 2025
 *
 */

//===--------------------------------------------------------------------------===//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

/* Function prototypes */
unsigned long oaat(char* key, unsigned long len, unsigned long bits);

// oaat stands for "one-at-a-time"
unsigned long oaat(char* key, unsigned long len, unsigned long bits) {
  unsigned long hash, i;
  for (hash = 0, i = 0; i < len; i++) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash += (hash >> 11);
  hash ^= (hash << 15);

  return hash & hashmask(bits);
}

//===--------------------------------------------------------------------------===//

/* Main function */
int main(void) {
  // Sample call of oaat
  char word[] = "hello";
  // 2^17 is the smallest power of 2 that is at least 100000
  unsigned long code = oaat(word, strlen(word), 17);
  printf("%lu\n", code);

  return 0;
}
