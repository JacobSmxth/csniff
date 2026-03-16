#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum { TCP, SYN } SCAN_TYPE;

typedef struct {
  char *target;
  int port_start;
  int port_end;
  int open_exclusive;
  SCAN_TYPE type;
} ScanArgs;

#endif
