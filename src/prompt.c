#include "prompt.h"
#include "scans.h"
#include "structs.h"
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_selection_int(char *input) { // Takes input and turns in into integer
  int val;
  errno = 0; // Reset to detect errors
  val = (int)strtol(input, NULL, 0);

  // I never realized how nice man pages were
  if (errno == EINVAL || errno == ERANGE) {
    perror("strtol");
    exit(EXIT_FAILURE);
  }
  return val;
}

int prompt_for_int(char *prompt) { // Prompt to get integer
  printf("%s> ", prompt);
  char input[100];
  fgets(input, 100, stdin);
  input[strcspn(input, "\n")] = '\0';
  return get_selection_int(input);
}

char prompt_for_char(char *prompt) { // Prompt to get character
  printf("%s> ", prompt);
  char ch;
  scanf("%c", &ch);
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
  return ch;
}

char *prompt_for_ipv4(char *prompt) { // Prompt to get IPv4 in a string
  struct in_addr addr;
  while (1) {
    printf("%s> ", prompt);
    char input[100];
    fgets(input, 100, stdin);
    input[strcspn(input, "\n")] = '\0';
    if (inet_pton(AF_INET, input, &addr) ==
        1) { // Make sure its a valid IPv4 address
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

ScanArgs *get_scan_args() { // Prompting for scan_args
  ScanArgs *args = malloc(sizeof(ScanArgs));
  if (!args) {
    perror("args malloc");
    exit(EXIT_FAILURE);
  }

  switch (prompt_for_char("Do you want to show only open ports (y/n)")) {
  case 'y':
    args->open_exclusive = 1;
    break;
  case 'n':
    args->open_exclusive = 0;
    break;
  default:
    fprintf(stderr, "falling back to showing all results");
    args->open_exclusive = 0;
    break;
  }

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
  memcpy(args->target, target, strlen(target) + 1);
  free(target);

  return args;
}

void process_menu_option(int choice) {
  ScanArgs *args;
  switch (choice) {
  case 1:
    args = get_scan_args();
    break;
  case 2:
    args = get_scan_args();
    args->type = LEG;
    break;
  case 3:
    exit(EXIT_SUCCESS);
    return;
    break;
  default:
    break;
  }
  scan_target(args);
}

ScanArgs *menuPrompt() {
  while (1) {
    printf("\nMenu Options\n");
    printf("================\n");
    printf("1. Multi-threaded TCP Scan\n2. Single-threaded TCP Scan\n"
           "3. Exit\n");
    printf("Make selection> ");
    char input[100];
    fgets(input, 100, stdin);
    process_menu_option(get_selection_int(input));
    input[strcspn(input, "\n")] = '\0';
  }
}
