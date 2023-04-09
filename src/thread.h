#ifndef BV_THREAD_H
#define BV_THREAD_H

#include <pthread.h>
#include <stdbool.h>
#include "../lib/cutility/include/queue.h"


typedef struct {
  void (*func)(void *);
  void *arg;
} Task;

typedef struct Threadpool{
  Queue q;
  pthread_t *threads;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  pthread_cond_t wait_cond;
  int n_threads;
  int active_threads;
  bool quit;
}Threadpool;

void threadpool_add_task(Threadpool *pool,Task* task); 
Task* task_create( void (*func)(void *), void *arg); 
Threadpool* threadpool_init(int n_threads);
void threadpool_destroy(Threadpool** pool);
void threadpool_wait(Threadpool *pool);
#endif
