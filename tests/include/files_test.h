#ifndef LEPL1503__FILES_TEST__
#define LEPL1503__FILES_TEST__


#include "CUnit/Basic.h"

#include "helper/verifyOutput.h"
#include "../../include/files.h"
#include "../../include/unassociated/portable_endian.h"
#include "../../include/unassociated/struct.h"


int init_suite_files(void);

int clean_suite_files(void);


void read_entry_file_Test(void);

void writeNberNodes_OutputFile_Test(void);

void writeData_OutputFile_Test(void);

int main_file_test();


#endif // LEPL1503__FILES_TEST__