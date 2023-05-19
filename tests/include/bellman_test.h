#ifndef LEPL1503__BELLMAN_FORD_TEST__
#define LEPL1503__BELLMAN_FORD_TEST__

#include "CUnit/Basic.h"

#include "../../include/bellman-ford.h"
#include "../../include/struct_initializer.h"
#include "../../include/struct_free.h"
#include "../../include/unassociated/struct.h"
#include "../../include/unassociated/portable_endian.h"


int init_suite_bellman(void);

int clean_suite_bellman(void);


void initializeAll_Test(void);

void checkNegCycle_False_Test(void);

void checkNegCycle_True_Test(void);

void bellman_ford_Test(void);

void completePathList_Test(void);

void reversePathList_Test(void);

void getPath_Test(void);

int main_bellman_test();


#endif // LEPL1503__BELLMAN_FORD_TEST__