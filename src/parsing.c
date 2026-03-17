#include "prompt.h"
#include "structs.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char validate_argument(char *argument) {
  if (argument[0] != '-') {
    return '0';
  }

  return argument[1];
}

void print_help() {
  printf("command format: ./build/port_sniffer -t <target> -p <port/range> "
         "|options|\n");
  printf("options:\n");
  printf(
      "-o  Hides any closed ports\n-n  Legacy scan mode\n-s  Syn scan mode\n");
}

int *validate_port_range(char *ports) {
  int start, end;
  int matched = sscanf(ports, "%d-%d", &start, &end);

  int *port = malloc(sizeof(int) * 2);
  if (matched == 2) {
    port[0] = start;
    port[1] = end;
  } else {
    matched = sscanf(ports, "%d", &start);
    if (matched == 1) {
      port[0] = start;
      port[1] = start;
    } else {
      free(port);
      port = NULL;
    }
  }
  return port;
}

int get_ports(ScanArgs *args, char *ports) {
  // Just making sure my pointer logic is correct

  int changed = 0;
  int *thePorts = validate_port_range(ports);
  if (thePorts) {
    if (thePorts[0] == thePorts[1]) {
      args->port_start = thePorts[0];
      args->port_end = thePorts[0];
    } else {

      if (thePorts[0] <= 0) {
        fprintf(stderr,
                "%d is an invalid port. Can't have a port lower than 1. "
                "Setting to 1.\n",
                thePorts[0]);
        args->port_start = 1;
        changed = 1;
      } else {

        args->port_start = thePorts[0];
      }
      if (thePorts[1] > 65535) {
        fprintf(
            stderr,
            "%d is an invalid port. Highest port is 65535. Setting to 65535.\n",
            thePorts[1]);
        args->port_end = 65535;
        changed = 1;
      } else {

        args->port_end = thePorts[1];
      }
      if (changed)
        sleep(4);
    }
    free(thePorts);
  } else {
    fprintf(stderr, "Invalid\n");
    free(thePorts);
    return 0;
  }
  return 1;
}

int get_target(ScanArgs *scan_args, char *target) {
  struct in_addr addr;
  if (!scan_args) {
    fprintf(stderr, "invalid target\n");
    return 0;
  }

  if (inet_pton(AF_INET, target, &addr) == 1) {
    size_t len = strlen(target) + 1;
    scan_args->target = malloc(len);
    memcpy(scan_args->target, target, len);
  } else {
    fprintf(stderr, "This is an invalid target address\n");
    return 0;
  }

  return 1;
}

ScanArgs *parse_arguments(int argc, char *argv[]) {

  if (argc == 1) { // If no arguments entered, open interactive selection
    return menuPrompt();
  }

  ScanArgs *scan_args = malloc(sizeof(ScanArgs));
  if (!scan_args) {
    perror("malloc");
    return NULL;
  }
  scan_args->open_exclusive = 0;
  scan_args->type = TCP;
  scan_args->target = NULL;

  for (int i = 1; i < argc; i++) {
    char argument = validate_argument(argv[i]);
    switch (argument) {
    case 't':
      get_target(scan_args, argv[++i]);
      break;
    case 'p':
      if (!scan_args->target) {
        printf("No target specified, falling back to 192.168.1.10\n");
        char *target = "192.168.1.10";
        scan_args->target = malloc(strlen(target) + 1);
        memcpy(scan_args->target, target, strlen(target) + 1);
      }
      get_ports(scan_args, argv[++i]);
      break;
    case 'o':
      scan_args->open_exclusive = 1;
      break;
    case 's':
      if (scan_args->type != TCP) {
        fprintf(stderr, "Can't set multiple scan types. Falling back to TCP");
        scan_args->type = TCP;
        break;
      }
      scan_args->type = SYN;
      break;
    case 'n':
      if (scan_args->type != TCP) {
        fprintf(stderr, "Can't set multiple scan types. Falling back to TCP");
        scan_args->type = TCP;
        break;
      }
      scan_args->type = LEG;
      break;
    case 'h':
      print_help();
      break;
    case 'm':
      printf("Local host scan is not functional yet.\n");
      break;
    default:
      fprintf(stderr, "%s doesn't exist.\n", argv[i]);
      printf("\n");
      exit(EXIT_FAILURE);
    }
  }

  // TESTING and DEBUGS:
  // if (!scan_args->target) {
  //   scan_args->target = malloc(strlen("192.168.1.0") + 1);
  //   scan_args->target = "192.168.1.0";
  // }
  // if (!scan_args->port_start) {
  //   scan_args->port_start = 1;
  // }
  // if (!scan_args->port_end) {
  //   scan_args->port_end = 1024;
  // }
  // printf("FINAL CHECK: %s: %d, %d\n", scan_args->target,
  // scan_args->port_start,
  //        scan_args->port_end);
  // END OF TESTING and DEBUGS

  return scan_args;
}
