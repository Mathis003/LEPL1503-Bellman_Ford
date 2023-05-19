#ifndef LEPL1503__STRUCT_FREE__
#define LEPL1503__STRUCT_FREE__

#include "unassociated/struct.h"

#include "display.h"
#include "files.h"


void free_outputGraph_t(outputGraph_t *outputGraph);

void free_graph_t(graph_t *input_graph);

uint8_t free_datas_threads(datas_threads_t *datas_threads);

void free_datas_threads_algorithm_t(int32_t *predecessor, int64_t *distance, outputGraph_t *output_graph, char *info_verbose, datas_threads_algorithm_t *datas_threads_algorithm);


#endif // LEPL1503__STRUCT_FREE__