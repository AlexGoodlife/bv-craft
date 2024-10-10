#include <stdio.h>
#include "thread.h"
#include "common.h"
#include <stdlib.h>
#include <string.h>

Task* task_create( void (*func)(void *), void *arg) {
  Task* result = malloc(sizeof(Task));
  result->func = func;
  result->arg = arg;
  return result;
}

void* threadpool_func(void*);

Threadpool* threadpool_init(int n_threads){
  Threadpool* result = malloc(sizeof(Threadpool));
  pthread_mutex_init(&result->lock, NULL);
  pthread_cond_init(&result->cond, NULL);
  pthread_cond_init(&result->wait_cond, NULL);
  result->n_threads = n_threads;
  result->q = queue_create();
  result->active_threads = 0;
  result->quit = false;
  result->threads = malloc(sizeof(pthread_t)*n_threads);
  for (int i = 0; i < n_threads; i++) {
    pthread_create(result->threads + i, NULL, &threadpool_func, result);
  }
  return result;
}

void threadpool_destroy(Threadpool** pool){
  if (*pool == NULL) {
    ERROR("Threadpool pointer is null, memory leaked");
    return;
  }
  (*pool)->quit = true;
  pthread_mutex_lock(&(*pool)->lock);
  pthread_cond_broadcast(&(*pool)->cond);
  pthread_mutex_unlock(&(*pool)->lock);
  for(int i = 0; i < (*pool)->n_threads;i++){
    pthread_join((*pool)->threads[i], NULL);
  }
  pthread_mutex_destroy(&(*pool)->lock);
  pthread_cond_destroy(&(*pool)->cond);
  queue_destroy(&(*pool)->q);
  free((*pool)->threads);
  free(*pool);
  *pool = NULL;
}

void threadpool_add_task(Threadpool *pool,Task* task) {
  pthread_mutex_lock(&pool->lock);

  while (pool->active_threads == pool->n_threads) {
    pthread_cond_wait(&pool->cond, &pool->lock);
  }

  queue_enqueue(pool->q, task);
  pool->active_threads++;

  pthread_cond_signal(&pool->cond);
  pthread_mutex_unlock(&pool->lock);
}

// First argument should be pointer to Threadpool itself
void *threadpool_func(void *arg) {
  Threadpool* pool = arg;
  while (1) {
    pthread_mutex_lock(&pool->lock);
    while (!pool->quit &&  queue_getSize(pool->q) == 0) {
      pthread_cond_wait(&pool->cond, &pool->lock);
    }
    if(pool->quit){
      pthread_mutex_unlock(&pool->lock);
      break;
    }
    Task *task = queue_dequeue(pool->q);
    
    pthread_mutex_unlock(&pool->lock);

    task->func(task->arg);

    pthread_mutex_lock(&pool->lock);
    pool->active_threads--;
    pthread_cond_signal(&pool->wait_cond);
    pthread_mutex_unlock(&pool->lock);
  }
  return NULL;
}

void threadpool_wait(Threadpool *pool) {
  pthread_mutex_lock(&pool->lock);
  while (!pool->quit && pool->active_threads > 0) {
    pthread_cond_wait(&pool->wait_cond, &pool->lock);
  }
  pthread_mutex_unlock(&pool->lock);
}
