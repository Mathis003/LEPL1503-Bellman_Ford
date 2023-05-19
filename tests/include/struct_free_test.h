#ifndef LEPL1503__STRUCT_FREE_TEST__
#define LEPL1503__STRUCT_FREE_TEST__

#include "CUnit/Basic.h"

#include "../../include/struct_initializer.h"
#include "../../include/struct_free.h"


int init_suite_struct_free(void);

int clean_suite_struct_free(void);


void free_outputGraph_t_Test(void);

void free_graph_t_Test(void);

void free_datas_threads_Test(void);

void free_datas_threads_algorithm_t_Test(void);

int main_struct_free_test();


#endif // LEPL1503__STRUCT_FREE_TEST__