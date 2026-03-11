#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *target;
  int port_start;
  int port_end;
} ScanArgs;

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

void get_ports(ScanArgs *args, char *ports) {
  // Just making sure my pointer logic is correct
  printf("PORTS: %s\n", ports);
  args->port_end = 100;
  args->port_start = 11;
}

void process_parsings(int argc, char *argv[]) {
  ScanArgs *scan_args = malloc(sizeof(ScanArgs));
  if (!scan_args) {
    fprintf(stderr, "Malloc error\n");
    return;
  }

  char *target = NULL;
  for (int i = 1; i < argc; i++) {
    char argument = validate_argument(argv[i]);
    switch (argument) {
    case 't':
      printf("TARGET: %s\n", argv[++i]);
      scan_args->target = malloc(strlen(argv[i]) + 1);
      memcpy(scan_args->target, argv[i], strlen(argv[i]) + 1);
      break;
    case 'p':
      printf("PORT RANGE: %s\n", argv[++i]);
      get_ports(scan_args, argv[i]);
      break;
    case 'h':
      print_help();
      break;
    case 'm':
      printf("Hello darling :)\n");
      break;
    default:
      fprintf(stderr,
              "Did you seriously think that '%s' would work. Stop trying to "
              "make up things. im serious. ",
              argv[i]);
      argv[++i] ? printf("You even tried saying it was '%s'. LMAO", argv[i])
                : printf("");
      printf("\n");
      exit(EXIT_FAILURE);
    }

    // Making sure the values are being correctly changes
    printf("%s: %d - %d\n", scan_args->target, scan_args->port_start,
           scan_args->port_end);
  }
}

int parse_arguments(int argc, char *argv[]) {
  process_parsings(argc, argv);

  return 1;
}
