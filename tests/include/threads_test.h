#ifndef LEPL1503__THREADS_TEST__
#define LEPL1503__THREADS_TEST__

#include "CUnit/Basic.h"
#include <unistd.h>

#include "../../include/threads.h"
#include "helper/sort_bin_files.h"
#include "helper/verifyOutput.h"


int init_suite_threads(void);

int clean_suite_threads(void);

// Fonction d'aide //
void *funct_threads_waiting(void *datas_threads);


void wait_threads_Test(void);

void function_thread_algorithm_Test(void);

void launch_algorithm_threads_Test(void);

int main_threads_test();


#endif // LEPL1503__THREADS_TEST__