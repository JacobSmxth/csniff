
// This is my block of rememberance. New concepts to me

// man 3 pthread_mutex_lock    - locks the mutex object; if unavailable, blocks
// until it becomes available. man 3 pthread_mutex_unlock  - releases the mutex
// object referenced by mutex. man 3 pthread_mutex_init    - initializes the
// mutex object with the specified attributes. man 3 pthread_mutex_destroy -
// destroys the mutex object, freeing any resources it might hold. man 3
// pthread_cond_wait     - atomically unlocks the mutex and blocks the calling
// thread on the condition variable. man 3 pthread_cond_signal   - unblocks at
// least one thread currently blocked on the condition variable. man 3
// pthread_cond_broadcast- unblocks all threads currently blocked on the
// condition variable. man 3 pthread_cond_init     - initializes the condition
// variable referenced by cond with the specified attributes. man 3
// pthread_cond_destroy  - destroys the condition variable referenced by cond.
// man 3 pthread_create        - starts a new thread in the calling process.
// man 3 pthread_join          - waits for the thread specified by thread to
// terminate.

#include "structs.h"
#include <inttypes.h>
#include <stdlib.h>

void *worker(void *arg) {
  thread_pool_t *pool = (thread_pool_t *)arg;
  while (1) {
    pthread_mutex_lock(
        &pool->lock); // This takes the mutex. which is a way to protect shared
                      // data. only one thread can modify or inspect at a time
    while (!pool->head && !pool->shutdown) {
      pthread_cond_wait(
          &pool->cond,
          &pool->lock); // This checks the queue. This is less CPU intensive
                        // than if I used a while loop that constantly checked.
                        // This just puts the thread to sleep
    }
    if (pool->shutdown &&
        !pool->head) { // Checks for a shutdown request or no more work
      pthread_mutex_unlock(&pool->lock);
      break;
    }
    work_i_t *item = pool->head;
    pool->head = item->next;
    if (!pool->head) {
      pool->tail = NULL;
    }

    pthread_mutex_unlock(&pool->lock);

    item->fn(item->arg);
    free(item);
  }
  return NULL;
}

void pool_init(thread_pool_t *pool, int size) {
  pool->size = size;
  pool->head = NULL;
  pool->tail = NULL;
  pool->shutdown = 0;
  pool->threads = malloc(sizeof(pthread_t) * size);
  pthread_mutex_init(&pool->lock, NULL);
  pthread_cond_init(&pool->cond, NULL);
  for (int i = 0; i < size; i++) {
    pthread_create(&pool->threads[i], NULL, worker, pool);
  }
}

void pool_submit(thread_pool_t *pool, void (*fn)(void *), void *arg) {
  work_i_t *item = malloc(sizeof(work_i_t));
  item->fn = fn;
  item->arg = arg;
  item->next = NULL;
  pthread_mutex_lock(&pool->lock);
  if (pool->tail) {
    pool->tail->next = item;
  } else {
    pool->head = item;
  }
  pool->tail = item;
  pthread_cond_signal(&pool->cond);
  pthread_mutex_unlock(&pool->lock);
}

void pool_destroy(thread_pool_t *pool) {
  pthread_mutex_lock(&pool->lock);
  pool->shutdown = 1;
  pthread_cond_broadcast(&pool->cond);
  pthread_mutex_unlock(&pool->lock);
  for (int i = 0; i < pool->size; i++) {
    pthread_join(pool->threads[i], NULL);
  }
  pthread_mutex_destroy(&pool->lock);
  pthread_cond_destroy(&pool->cond);
  free(pool->threads);
}
