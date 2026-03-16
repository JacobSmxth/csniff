#include "structs.h"
#include <arpa/inet.h>
#include <pthread.h> // Need to set up multithreading
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int scan(char *target, int port) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, target, &addr.sin_addr);
  int result = connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0;
  close(sock);
  return result;
}

void tcp_scan(ScanArgs *arguments) {
  printf("Running TCP Scan\n");
  int start = arguments->port_start;
  int end = arguments->port_end;
  for (int port = start; port <= end; port++) {
    if (scan(arguments->target, port)) {
      printf("Port %d OPEN\n", port);
    } else {
      if (!arguments->open_exclusive) {
        printf("Port %d CLOSED\n", port);
      }
    }
  }
  free(arguments->target);
  free(arguments);
}

void syn_scan(ScanArgs *arguments) {

  int start = arguments->port_start;
  int end = arguments->port_end;
  int open_only = arguments->open_exclusive;
  printf("SYN SCAN. Scan function not complete.\n");
  printf("Starting at port %d, ending at port %d. Syn Scan.\n", start, end);
  if (open_only) {
    printf("Showing only open ports\n");
  } else {
    printf("Showing all port results\n");
  }

  printf("Falling back to default TCP Scan\nSYN SCAN NOT COMPLETE OR READY TO "
         "BE USED.\n");
  tcp_scan(arguments);
}

void scan_target(ScanArgs *arguments) {
  switch (arguments->type) {
  case TCP:
    tcp_scan(arguments);
    break;
  case SYN:
    syn_scan(arguments);
    break;
  default:
    fprintf(stderr, "invalid type\n");
    break;
  }
}
