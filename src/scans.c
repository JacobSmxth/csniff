#include "structs.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define POOL_SIZE 32

int scan(char *target, int port) {
  // socket() creates a new socket using IPv4 internet protocols, provides a
  // reliable two way connection
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in
      addr; // This describes an IPv4 internet domain socket address
  addr.sin_family = AF_INET;   // IPv4 internet protocols
  addr.sin_port = htons(port); // Converts the int into network byte order then
                               // assigns the sockets address to it
  inet_pton(AF_INET, target, &addr.sin_addr);
  int result = connect(sock, (struct sockaddr *)&addr, sizeof(addr)) ==
               0; // Connects to the port and returns if connected or not
  close(sock);    // Close the connection
  return result;  // function return if successful or not
}

void threaded_scan(void *args) {
  // Cast void* to a Args* struct
  Args *arguments = (Args *)args;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return;
  }

  // Basically a repeat of the scan function except pulling from a struct this
  // time
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(arguments->port);
  inet_pton(AF_INET, arguments->target, &addr.sin_addr);
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
    printf("Port %d open\n\n", arguments->port);
  } else {
    if (!arguments->open_exclusive) {
      printf("Port %d closed\n\n", arguments->port);
    }
  }
  close(sock);
  // Have to free the arguments this time
  free(args);
  return;
}

void legacy_scan(ScanArgs *arguments) {
  printf("========================\n");
  printf("Running legacy scan\n\n");
  int start = arguments->port_start;
  int end = arguments->port_end;
  int open_exclusive = arguments->open_exclusive;

  // Loops the scan over all the ports
  for (int port = start; port <= end; port++) {
    if (scan(arguments->target, port)) {
      printf("Port %d open\n\n", port);
    } else {
      if (!open_exclusive) { // Checks if user chose to hide closed ports
        printf("Port %d closed\n\n", port);
      }
    }
  }
  printf("========================\n");

  // Free the arguments
  free(arguments->target);
  free(arguments);
}

void tcp_scan(ScanArgs *arguments) {
  printf("========================\n");
  printf("Running TCP Scan\n\n");

  // Set variables so I don't have to pull from heap, quicker with stack

  int start = arguments->port_start;
  int end = arguments->port_end;

  thread_pool_t pool;
  pool_init(&pool, POOL_SIZE);

  for (int port = start; port <= end; port++) {
    Args *arg = malloc(sizeof(Args));
    arg->target = malloc(strlen(arguments->target) + 1);
    memcpy(arg->target, arguments->target, strlen(arguments->target) + 1);
    arg->port = port;
    arg->open_exclusive = arguments->open_exclusive;
    pool_submit(&pool, threaded_scan, arg);
  }

  pool_destroy(&pool);

  printf("========================\n");

  free(arguments->target);
  free(arguments);
}

void scan_target(ScanArgs *arguments) {
  switch (arguments->type) {
  case TCP:
    tcp_scan(arguments);
    break;
  case LEG:
    legacy_scan(arguments);
    break;
  case NUL:
    break;
  default:
    fprintf(stderr, "Invalid scan type selected\n");
    free(arguments->target);
    free(arguments);
    break;
  }
}
