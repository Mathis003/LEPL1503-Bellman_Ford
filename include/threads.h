#ifndef LEPL1503__THREADS__
#define LEPL1503__THREADS__

#include "unassociated/portable_semaphore.h"
#include "unassociated/struct.h"

#include "files.h"
#include "bellman-ford.h"


uint8_t wait_threads(pthread_t *threads, datas_threads_t *datas_threads);

uint8_t launch_algorithm_threads(pthread_t *threads, datas_threads_t *datas_threads);

void *function_thread_algorithm(void *args_struct);


#endif // LEPL1503__THREADS__