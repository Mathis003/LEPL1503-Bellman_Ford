#ifndef LEPL1503__DISPLAY_TEST__
#define LEPL1503__DISPLAY_TEST__

#include "CUnit/Basic.h"

#include "../../include/display.h"
#include "../../include/files.h"

int init_suite_display(void);

int clean_suite_display(void);


void end_funct_test(char tempFile[], FILE *tmpfile_nber, FILE *stream, char expected_buffer[], const uint32_t size_buffer);

void printErrorMutexDestory(void);

void printNberThreadsNeg_Test(void);

void initialisationSucceed_Test(void);

void printNegCycleDetected_Test(void);

void printNegCycleNotDetected_Test(void);

void printVariableNULL_Test(void);

void printNeedEntryFile_Test(void);

void printSameSize_Test(void);

void printDifferentSize_Test(void);

void printTimer_Test(void);

void printPathWithExtralineOff_Test(void);

void printPathWithExtralineOn_Test(void);

void printInputGraphInfo_Test(void);

void printInputGraphLien_Test(void);

void printDecodingSection_Test(void);

void printAlgorithme_Encodage_Section_Test(void);

void printErrorWriting_Test(void);

void printCloseFileError_Test(void);

void printMemoryAllocationError_Test(void);

void printMethodFailed_Test(void);

void usage_Test(void);

void printDatas_Test(void);

int main_display();


#endif // LEPL1503__DISPLAY_TEST__