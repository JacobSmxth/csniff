#ifndef PARSING_H
#define PARSING_H

typedef struct {
  char *target;
  int port_start;
  int port_end;
} ScanArgs;

ScanArgs *parse_arguments(int argc, char *argv[]);

#endif
