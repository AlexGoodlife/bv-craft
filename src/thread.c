#include <stdio.h>
#include "thread.h"
#include <stdlib.h>
#include <string.h>

Threadpool* threadpool_init(int n_threads, int n_tasks){
  Threadpool* result = malloc(sizeof(Threadpool));
  pthread_mutex_init(&result->lock, NULL);
  pthread_cond_init(&result->cond, NULL);
  result->n_threads = n_threads;
  result->n_tasks = n_tasks;
  result->num_tasks = 0;
  result->task_idx = 0;
  result->tail = 0;
  result->quit = false;
  result->threads = malloc(sizeof(pthread_t)*n_threads);
  result->tasks = malloc(sizeof(Task) * n_tasks);
  for (int i = 0; i < n_threads; i++) {
    pthread_create(result->threads + i, NULL, threadpool_init_func, result);
  }
  return result;
}

void threadpool_destroy(Threadpool** pool){
  (*pool)->quit = true;
  pthread_mutex_lock(&(*pool)->lock);
  pthread_cond_broadcast(&(*pool)->cond);
  pthread_mutex_unlock(&(*pool)->lock);
  for(int i = 0; i < (*pool)->n_threads;i++){
    pthread_join((*pool)->threads[i], NULL);
  }
  pthread_mutex_destroy(&(*pool)->lock);
  pthread_cond_destroy(&(*pool)->cond);
  free((*pool)->tasks);
  free((*pool)->threads);
  free(*pool);
  *pool = NULL;
}

void threadpool_add_task(Threadpool *pool, void (*func)(void *), void *arg) {
  pthread_mutex_lock(&pool->lock);
  while (pool->num_tasks == pool->n_tasks) {
    pthread_cond_wait(&pool->cond, &pool->lock);
  }
  pool->tasks[pool->num_tasks].func = func;
  pool->tasks[pool->num_tasks].arg = arg;
  pool->tail++;
  pool->tail %= pool->n_tasks;
  pool->num_tasks++;
  pthread_cond_signal(&pool->cond);
  pthread_mutex_unlock(&pool->lock);
}

// First argument should be pointer to Threadpool itself
void *threadpool_init_func(void *arg) {
  Threadpool* pool = arg;
  while (!pool->quit) {
    pthread_mutex_lock(&pool->lock);
    while (!pool->quit && pool->num_tasks == 0) {
      pthread_cond_wait(&pool->cond, &pool->lock);
    }
    if(pool->quit){
      pthread_mutex_unlock(&pool->lock);
      break;
    }
    Task task = pool->tasks[pool->task_idx];
    pool->task_idx = (pool->task_idx + 1) % pool->n_tasks;
    //pthread_cond_signal(&pool->cond);
    pthread_mutex_unlock(&pool->lock);

    task.func(task.arg);
    
    pthread_mutex_lock(&pool->lock);
    pool->num_tasks--;
    // printf("I CALL: ACTIVE %d\n", pool->num_tasks);
    if (pool->num_tasks == 0) {
      printf("I CALL IT TO STOP WAITING\n");
      pthread_cond_signal(&pool->cond);
    }
    pthread_mutex_unlock(&pool->lock);
  }
  return NULL;
}

void threadpool_wait(Threadpool *pool) {
  pthread_mutex_lock(&pool->lock);
  while (pool->num_tasks > 0) {
    printf("I ACTUALLY WAIT\n");
    pthread_cond_wait(&pool->cond, &pool->lock);
  }
  pthread_mutex_unlock(&pool->lock);
}
