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
  printf("HELP ISN't AVAILABLE... Trying API again in 5 seconds... (I'm joking "
         "just haven't coded anything)\n");
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
        fprintf(stderr, "Dawg. I swear. There isn't a port 0. I set it to 1 "
                        "instead. Cause I think we want this to run\n");
        args->port_start = 1;
        changed = 1;
      } else {

        args->port_start = thePorts[0];
      }
      if (thePorts[1] > 65335) {
        fprintf(stderr, "You should know ports only go up to 65535. I have set "
                        "it to that just so this will function.\n");
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
    fprintf(stderr, "ScanArgs doesn't exist\n");
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

  ScanArgs *scan_args = malloc(sizeof(ScanArgs));
  if (!scan_args) {
    fprintf(stderr, "Malloc error\n");
    return NULL;
  }
  scan_args->open_exclusive = 0;
  scan_args->type = TCP;

  for (int i = 1; i < argc; i++) {
    char argument = validate_argument(argv[i]);
    switch (argument) {
    case 't':
      get_target(scan_args, argv[++i]);
      break;
    case 'p':
      if (!scan_args->target) {
        printf("No target specified, falling back to default\n");
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
        fprintf(stderr, "Already set a scan type. You're confusing me. Falling "
                        "back to TCP\n");
        scan_args->type = TCP;
        break;
      }
      scan_args->type = SYN;
      break;
    case 'n':
      if (scan_args->type != TCP) {
        fprintf(stderr, "Already set a scan type. You're confusing me. Falling "
                        "back to TCP\n");
        scan_args->type = TCP;
        break;
      }
      scan_args->type = LEG;
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
                : printf(" ");
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
