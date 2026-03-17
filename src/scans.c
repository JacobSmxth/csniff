#include "structs.h"
#include <arpa/inet.h>
#include <pthread.h> // Need to set up multithreading
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void *threaded_scan(void *args) {
  Args *arguments = (Args *)args;

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    return NULL;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(arguments->port);
  inet_pton(AF_INET, arguments->target, &addr.sin_addr);
  if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
    printf("Port %d open\n", arguments->port);
  } else {
    if (!arguments->open_exclusive) {
      printf("Port %d closed\n", arguments->port);
    }
  }
  close(sock);
  free(args);
  return NULL;
}

void legacy_scan(ScanArgs *arguments) {
  printf("Running legacy scan\n");
  int start = arguments->port_start;
  int end = arguments->port_end;
  int open_exclusive = arguments->open_exclusive;

  for (int port = start; port <= end; port++) {
    if (scan(arguments->target, port)) {
      printf("Port %d open\n", port);
    } else {
      if (!open_exclusive) {
        printf("Port %d closed\n", port);
      }
    }
  }

  free(arguments->target);
  free(arguments);
}

void tcp_scan(ScanArgs *arguments) {
  printf("Running TCP Scan\n");
  int start = arguments->port_start;
  int end = arguments->port_end;
  int count = end - start + 1;

  if (count > 4500) {
    printf("Thread pools are not setup. Falling back to single-threaded tcp "
           "scan\n");
    ;
    legacy_scan(arguments);
    return;
  }

  pthread_t threads[count];

  for (int i = 0; i < count; i++) {
    Args *newArgs = malloc(sizeof(Args));
    newArgs->target = malloc(strlen(arguments->target) + 1);
    memcpy(newArgs->target, arguments->target, strlen(arguments->target) + 1);
    newArgs->port = start + i;
    newArgs->open_exclusive = arguments->open_exclusive;
    pthread_create(&threads[i], NULL, threaded_scan, newArgs);
  }

  for (int i = 0; i < count; i++) {
    pthread_join(threads[i], NULL);
  }

  free(arguments->target);
  free(arguments);
}

void syn_scan(ScanArgs *arguments) {

  int start = arguments->port_start;
  int end = arguments->port_end;
  int open_only = arguments->open_exclusive;
  if (open_only) {
    printf("Showing only open ports\n");
  } else {
    printf("Showing all port results\n");
  }

  printf("Falling back to default TCP Scan\n");
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
  case LEG:
    legacy_scan(arguments);
    break;
  default:
    fprintf(stderr, "invalid type\n");
    free(arguments->target);
    free(arguments);
    break;
  }
}
