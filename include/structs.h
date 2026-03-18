#ifndef STRUCTS_H
#define STRUCTS_H

#include <pthread.h>

// Scan type enum
typedef enum { LEG, TCP, NUL } SCAN_TYPE;

// thread args
typedef struct {
  char *target;
  int port;
  int open_exclusive;
} Args;

// Scan config
typedef struct {
  char *target;
  int port_start;
  int port_end;
  int open_exclusive;
  SCAN_TYPE type;
} ScanArgs;

// Thread pool

typedef struct work_i {
  void (*fn)(void *); // This is scray but tutorial was doing this
  void *arg;
  struct work_i *next;
} work_i_t;

typedef struct {
  pthread_t *threads;
  int size;
  work_i_t *head;
  work_i_t *tail;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int shutdown;
} thread_pool_t;

void pool_init(thread_pool_t *pool, int size);
void pool_submit(thread_pool_t *pool, void (*fn)(void *), void *arg);
void pool_destroy(thread_pool_t *pool);

#endif
