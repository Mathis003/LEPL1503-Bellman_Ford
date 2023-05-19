#ifndef LEPL1503__STRUCT_INITIALIZER__
#define LEPL1503__STRUCT_INITIALIZER__

#include "unassociated/struct.h"

#include "display.h"


uint8_t initializeInputGraph(datas_threads_t *datas_threads, uint32_t nberNodes, uint32_t nberEdges);

void initializeDefaultArgs(datas_threads_t *datas_threads);

void initialize_Data_algorithm_threads(datas_threads_t *datas_threads, datas_threads_algorithm_t * datas_threads_algo, uint32_t lastNode, uint32_t nber_iteration);

#endif // LEPL1503__STRUCT_INITIALIZER__