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

void print_help() {
  printf("HELP ISN't AVAILABLE... Trying API again in 5 seconds... (I'm joking "
         "just haven't coded anything\n");
}

void process_parsings(int argc, char *argv[]) {
  for (int i = 1; i < argc; i++) {
    char argument = validate_argument(argv[i]);
    switch (argument) {
    case 't':
      printf("TARGET: %s\n", argv[++i]);
      break;
    case 'p':
      printf("PORT RANGE: %s\n", argv[++i]);
      break;
    case 'h':
      print_help();
      break;
    }
  }
}

int parse_arguments(int argc, char *argv[]) {
  process_parsings(argc, argv);

  return 1;
}
