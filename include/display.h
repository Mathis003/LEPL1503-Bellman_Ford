#ifndef LEPL1503__DISPLAY__
#define LEPL1503__DISPLAY__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>

#include "unassociated/macros.h"
#include "unassociated/portable_endian.h"


// ===== Error ===== //

void printErrorMutexDestory(void);

void printNberThreadsNeg(void);

void printThreadError(uint32_t id);

void printNeedEntryFile(void);

void printErrorWriting(const char *name_error, int32_t errnum);

void printOpenFileError(int32_t errnum);

void printCloseFileError(int32_t errnum);

void printMemoryAllocationError(const char *name, int32_t errnum);

void printGraphNotInitialized(int32_t errnum);

void printMethodFailed(const char *name_method, int32_t errnum);

void printErrorNoFile(int32_t errnum);

void printVariableNULL(const char *nameVariable);

void printErrorEmptyFile(void);


// ===== Bellman-ford ===== //

void initialisationSucceed(char *info_verbose, uint32_t source);

void NegCycleNotDetected(char *info_verbose);

void NegCycleDetected(char *info_verbose, uint32_t nodeFrom, uint32_t nodeTo, int32_t cost, int64_t *distance);


// ===== Section ===== //

void printDecodingSection(void);

void printAlgorithme_Encodage_Section(void);


// ===== Struct_modifier ===== //

void printInputGraphInfo(uint32_t nberNodes, uint32_t nberEdges);

void printInputGraphLien(uint32_t index, uint32_t from, uint32_t to, int32_t weight);


// ===== Files ===== //

void printDatas(uint32_t nberNodes, uint32_t sourceNode, uint32_t destinationNode, uint32_t maxLength, int64_t totalCost, int64_t *distance);

void printPath(uint32_t pathNode, bool extraLine);


// ===== Sp ===== //

void printTimer(double time_programme);

void usage(char *prog_name);


// ===== VerifyOutput ===== //

void printDifferentSize(size_t size1, size_t size2);

void printSameSize(size_t size);


#endif // LEPL1503__DISPLAY__