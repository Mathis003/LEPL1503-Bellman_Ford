#ifndef LEPL1503__STRUCT_INITIALIZER_TEST__
#define LEPL1503__STRUCT_INITIALIZER_TEST__

#include "CUnit/Basic.h"

#include "../../include/unassociated/portable_endian.h"

#include "../../include/struct_initializer.h"
#include "../../include/struct_free.h"


int init_suite_struct_initializer(void);

int clean_suite_struct_initializer(void);


void initializeInputGraph_Test(void);

void initializeDefaultArgs_Test(void);

void initialize_Data_algorithm_threads_Test(void);

int main_struct_initializer_test();


#endif // LEPL1503__STRUCT_INITIALIZER_TEST__