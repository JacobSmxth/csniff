#include "parsing.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void test_scan(ScanArgs *arguments) {
  int start = arguments->port_start;
  int end = arguments->port_end;
  for (int port = start; port <= end; port++) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("socket");
      continue;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, arguments->target, &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
      printf("Port %d: OPEN\n", port);
    } else {
      printf("Port %d: CLOSED\n", port);
    }

    close(sock);
  }
  free(arguments->target);
  free(arguments);
}

int main(int argc, char *argv[]) {
  ScanArgs *arg = parse_arguments(argc, argv);
  test_scan(arg);

  return 0;
}
