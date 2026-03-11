#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void menuPrompt() {
  while (1) {
    printf("This is the general menu layout!\n");
    printf("\nMenu Options\n");
    printf("================\n");
    printf("1. Option\n2. Option\n3. Option\n\n");
    printf("Make selection> ");
    char input[100];

    fgets(input, 100, stdin);
    printf("%s", input);
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "Jacob") == 0) {
      exit(EXIT_SUCCESS);
    }
  }
}
