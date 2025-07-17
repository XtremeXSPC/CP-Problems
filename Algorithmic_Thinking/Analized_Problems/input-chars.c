/**
 * @file input-chars.c
 * @author Costantino Lombardi
 * @brief An example of chars (from stdin) to integer conversion.
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

#define SIZE 255

/* Main function */
int main(void) {

  char line[SIZE + 1];
  // The "candy" variable cames from
  // "Halloween Houl" problem that I'm solving.
  int digit1, digit2, candy;

  // Get input from "stdin"
  // gets(line); Deprecated

  printf("%s", "Insert a number: (-1 to exit)\n");
  do {
    fgets(line, SIZE + 1, stdin);
    line[strcspn(line, "\n")] = '\0';

    if (strcmp(line, "-1") == 0) {
      break;
    }

    digit1 = line[0] - '0';
    if (strlen(line) == 1) {
      candy = digit1;
    } else {
      digit2 = line[1] - '0';
      candy  = 10 * digit1 + digit2;
    }
    printf("%d\n", candy);
    printf("%s", "Insert a number: (-1 to exit)\n");
  } while (1);

  /* This code works only with a single character
  char line[SIZE + 1];
  int  candy;
  gets(line);
  candy = line[0] - '0';
  printf("%d\n", candy);
  */

  return 0;
}
