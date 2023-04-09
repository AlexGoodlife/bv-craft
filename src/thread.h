#ifndef BV_THREAD_H
#define BV_THREAD_H

#include <pthread.h>
#include <stdbool.h>


typedef struct {
  void (*func)(void *);
  void *arg;
} Task;

typedef struct Threadpool{
  Task *tasks;
  pthread_t *threads;
  int num_tasks;
  int task_idx;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  int n_threads;
  int n_tasks;
  int tail;
  bool quit;
}Threadpool;

void *threadpool_init_func(void *arg);
void threadpool_add_task(Threadpool *pool, void (*func)(void *), void *arg); 
Threadpool* threadpool_init(int n_threads, int n_tasks);
void threadpool_destroy(Threadpool** pool);
void threadpool_wait(Threadpool *pool);
#endif
