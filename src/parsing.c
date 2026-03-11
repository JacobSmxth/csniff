#include <stdio.h>

char validate_argument(char *argument) {
  if (argument[0] != '-') {
    return '0';
  }

  return argument[1];
}

void two_args(char *argv[]) {
  char choice = validate_argument(argv[1]);
  if (choice == '0') {
    fprintf(stderr, "unknown thingy\n");
    return;
  }

  printf("%c\n", choice);

  switch (choice) {
  case 'h':
    printf("HELP PAGE\nTHERE IS NO HELP YET\n");
    break;
  case 'm':
    printf("m is for 'mine'. This will evenutally scan your network for all "
           "open ports\n");
    break;
  default:
    printf("oh no\n");
    break;
  }
}

void process_parsings(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Error: must enter arguments (See -h for help)\n");
  } else if (argc == 2) {
    two_args(argv);
  }
}

int parse_arguments(int argc, char *argv[]) {
  process_parsings(argc, argv);

  return 1;
}
