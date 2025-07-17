/**
 * @file Problem_coci17c1p3hard.c
 * @author Costantino Lombardi
 * @brief Implementation of the solution to DMOJ problem coci17c1p3hard.
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

#define MAX_USERS 100000
#define MAX_PASSWORD 10
#define NUM_BITS 20

// Macros
#define hashsize(n) ((unsigned long)1 << (n))
#define hashmask(n) (hashsize(n) - 1)

/* Types */
typedef struct password_node {
  char                  password[MAX_PASSWORD + 1];
  int                   total;
  struct password_node* next;
} password_node;

/* Function prototypes */
unsigned long  oaat(char* key, unsigned long len, unsigned long bits);
password_node* in_hash_table(password_node* hash_table[], char* find);
void           add_to_hash_table(password_node* hash_table[], char* find);
int            already_added(char all_substrings[][MAX_PASSWORD + 1], int total_substring, char* find);

/* Functions definitions */
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

password_node* in_hash_table(password_node* hash_table[], char* find) {
  unsigned       password_code;
  password_node* password_ptr;
  password_code = oaat(find, strlen(find), NUM_BITS);
  password_ptr  = hash_table[password_code];
  while (password_ptr) {
    if (strcmp(password_ptr->password, find) == 0)
      return password_ptr;
    password_ptr = password_ptr->next;
  }
  return NULL;
}

void add_to_hash_table(password_node* hash_table[], char* find) {
  unsigned       password_code;
  password_node* password_ptr;
  password_ptr = in_hash_table(hash_table, find);
  if (!password_ptr) {
    password_code = oaat(find, strlen(find), NUM_BITS);
    password_ptr  = malloc(sizeof(password_node));
    if (password_ptr == NULL) {
      fprintf(stderr, "malloc() error!\n");
      exit(1);
    }
    strcpy(password_ptr->password, find);
    password_ptr->total       = 0;
    password_ptr->next        = hash_table[password_code];
    hash_table[password_code] = password_ptr;
  }
  password_ptr->total++;
}

int already_added(char all_substrings[][MAX_PASSWORD + 1], int total_substring, char* find) {
  int i;
  for (i = 0; i < total_substring; i++)
    if (strcmp(all_substrings[i], find) == 0)
      return 1;
  return 0;
}

/* Main function */
int main(void) {
  static password_node* hash_table[1 << NUM_BITS] = {NULL};
  int                   num_ops, op, op_type, i, j;
  char                  password[MAX_PASSWORD + 1], substring[MAX_PASSWORD + 1];
  password_node*        password_prt;

  // This fix the substring counting bug
  int  total_substrings;
  char all_substrings[MAX_PASSWORD * MAX_PASSWORD][MAX_PASSWORD + 1];
  scanf("%d", &num_ops);

  for (op = 0; op < num_ops; op++) {
    scanf("%d%s", &op_type, password);

    if (op_type == 1) {
      total_substrings = 0; // Fix counting
      for (i = 0; i < (int)strlen(password); i++)
        for (j = i; j < (int)strlen(password); j++) {
          // strncpy(substring, &password[i], j - i + 1);
          // snprintf resolve compiler warning about truncation
          snprintf(substring, MAX_PASSWORD + 1, "%.*s", j - i + 1, &password[i]);
          substring[j - i + 1] = '\0';
          // This section is needed to check if we've seen
          // the current substring
          if (!already_added(all_substrings, total_substrings, substring)) {
            add_to_hash_table(hash_table, substring);
            strcpy(all_substrings[total_substrings], substring);
            total_substrings++;
          }
        }
    } else {
      password_prt = in_hash_table(hash_table, password);
      if (!password_prt)
        printf("0\n");
      else
        printf("%d\n", password_prt->total);
    }
  }
  return 0;
}

//=---------------------- Observations ----------------------==//

/* This initial solution to the proposed problem led
 * to a slow search for the password we needed to find
 * among those stored in the password array.
 * Therefore, searching for the password implies
 * a computational complexity of O(n^2),
 * which is too high for the imposed limit.
 */

/*
 static char users[MAX_USERS][MAX_PASSWORD + 1];
 int         num_ops, op, op_type, total, j;
 char        password[MAX_PASSWORD + 1];
 int         num_users = 0;
 scanf("%d", &num_ops);
 for (op = 0; op < num_ops; op++) {
   scanf("%d%s", &op_type, password);

   if (op_type == 1) {
     strcpy(users[num_users], password);
     num_users++;
   } else {
     total = 0;
     for (j = 0; j < num_users; j++)
       if (strstr(users[j], password))
         total++;
     printf("%d\n", total);
   }
 }
 */
