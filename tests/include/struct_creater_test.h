#ifndef LEPL1503__STRUCT_CREATER_TEST__
#define LEPL1503__STRUCT_CREATER_TEST__

#include "CUnit/Basic.h"

#include "../../include/unassociated/portable_endian.h"

#include "display_test.h"
#include "../../include/struct_creater.h"


int init_suite_struct_creater(void);

int clean_suite_struct_creater(void);


void create_basic_InputGraph_Test(void);

void createOutputGraph_Test(void);

void create_Datas_Threads_Normal_Test(void);

void create_Datas_Threads_Help_Test(void);

void create_Datas_Threads_Args_Test(void);

int main_struct_creater_test();


#endif // LEPL1503__STRUCT_CREATER_TEST__