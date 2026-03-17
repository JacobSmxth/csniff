#include "prompt.h"
#include "scans.h"
#include "structs.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_selection_int(char *input) {
  int val;
  errno = 0;
  val = (int)strtol(input, NULL, 0);

  // I never realized how nice man pages were
  if (errno == EINVAL || errno == ERANGE) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  return val;
}

int prompt_for_int(char *prompt) {
  while (1) {
    printf("%s> ", prompt);
    char input[100];
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
    return get_selection_int(input);
  }
}

char *prompt_for_ipv4(char *prompt) {
  struct in_addr addr;
  while (1) {
    printf("%s> ", prompt);
    char input[100];
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
    if (inet_pton(AF_INET, input, &addr) == 1) {
      size_t len = strlen(input) + 1;
      char *address = malloc(len);
      memcpy(address, input, len);
      return address;
    } else {
      fprintf(stderr, "This is an invalid target address\n");
      continue;
    }
  }
}

ScanArgs *get_scan_args() {
  ScanArgs *args = malloc(sizeof(ScanArgs));
  if (!args) {
    perror("args malloc");
    exit(EXIT_FAILURE);
  }

  args->open_exclusive = 1;
  args->type = TCP;
  int start, end;

  switch (prompt_for_int("Do you want to search range or single?\n1. Range\n2. "
                         "Single\nEnter choice")) {
  case 1:
    start = prompt_for_int("Enter starting port");
    end = prompt_for_int("Enter ending port");
    break;
  case 2:
    start = prompt_for_int("Enter port");
    end = start;
    break;
  default:
    exit(EXIT_FAILURE);
  }

  args->port_start = start;
  args->port_end = end;
  char *target = prompt_for_ipv4("Enter target ipv4");
  args->target = malloc(strlen(target) + 1);
  memcpy(args->target, target, strlen(target));

  return args;
}

void process_menu_option(int choice) {
  ScanArgs *args = get_scan_args();
  switch (choice) {
  case 1:
    break;
  case 2:
    args->type = LEG;
    break;
  case 3:
    args->type = SYN;
    break;
  default:
    break;
  }
  scan_target(args);
}

ScanArgs *menuPrompt() {
  while (1) {
    printf("This is the general menu layout!\n");
    printf("\nMenu Options\n");
    printf("================\n");
    printf("1. Multi-threaded TCP Scan\n2. Single-threaded TCP Scan\n3. SYN "
           "Scan\n\n");
    printf("Make selection> ");
    char input[100];
    fgets(input, 100, stdin);
    process_menu_option(get_selection_int(input));
    input[strcspn(input, "\n")] = '\0';
    if (strcmp(input, "Jacob") == 0) {
      exit(EXIT_SUCCESS);
    }
  }
}
