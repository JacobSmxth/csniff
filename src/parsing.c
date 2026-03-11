#include <stdio.h>
int parse_arguments(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    printf("%d: %s\n", i, argv[i]);
  }

  return argc;
}
