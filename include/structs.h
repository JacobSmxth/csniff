#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  char *target;
  int port;
  int open_exclusive;
} Args;

typedef enum { LEG, TCP } SCAN_TYPE;

typedef struct {
  char *target;
  int port_start;
  int port_end;
  int open_exclusive;
  SCAN_TYPE type;
} ScanArgs;

#endif
