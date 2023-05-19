#ifndef LEPL1503__FINAL_TEST__
#define LEPL1503__FINAL_TEST__

#include "CUnit/Basic.h"

#include "helper/verifyOutput.h"
#include "helper/sort_bin_files.h"
#include "../../include/sp.h"


int init_suite_all(void);

int clean_suite_all(void);

uint8_t main_toCreate_binaryFile(char input_name[], char output_name[], uint8_t nb_thread);

void compareBinaryFiles_ALL_LIMITS_CASES_MonoThread_Test(void);

void compareBinaryFiles_ALL_LIMITS_CASES_BasicThread_Test(void);

void compareBinaryFiles_ALL_LIMITS_CASES_LotOfThread_Test(void);

void compareBinaryFiles_RandomsGraph_MonoThread_Test(void);

void compareBinaryFiles_RandomsGraph_Basic_Thread_Test(void);

void compareBinaryFiles_RandomsGraph_LofOfThread_Test(void);

int main_final_test();


#endif // LEPL1503__FINAL_TEST__