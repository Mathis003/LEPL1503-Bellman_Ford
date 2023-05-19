#ifndef LEPL1503__VERIFY_OUTPUT_TEST__
#define LEPL1503__VERIFY_OUTPUT_TEST__

#include "CUnit/Basic.h"

#include "helper/verifyOutput.h"


int init_suite_verifyOutput(void);

int clean_suite_verifyOutput(void);


void check_same_size_True_Test(void);

void check_same_size_False_Test(void);

void check_same_contains_True_Test(void);

void check_same_contains_False_Test(void);

void compare_two_outputFile_True_Test(void);

void compare_two_outputFile_False_1_Test(void);

void compare_two_outputFile_False_2_Test(void);

int main_verifyOutput_test();


#endif // LEPL1503__VERIFY_OUTPUT_TEST__