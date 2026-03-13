#include "parsing.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

void test_scan(char *target, int start_port, int end_port) {
  for (int port = start_port; port <= end_port; port++) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("socket");
      continue;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, target, &addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == 0) {
      printf("Port %d: OPEN\n", port);
    } else {
      printf("Port %d: CLOSED\n", port);
    }

    close(sock);
  }
}

int main(int argc, char *argv[]) {
  ScanArgs *arg = parse_arguments(argc, argv);
  test_scan(arg->target, arg->port_start, arg->port_end);
  return 0;
}
