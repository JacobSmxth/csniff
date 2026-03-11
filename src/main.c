#include "parsing.h"
#include "prompt.h"

int main(int argc, char *argv[]) {
  parse_arguments(argc, argv);
  menuPrompt();
  return 0;
}
