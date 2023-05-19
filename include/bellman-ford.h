#ifndef LEPL1503__BELLMAN_FORD__
#define LEPL1503__BELLMAN_FORD__


#include "unassociated/struct.h"
#include "unassociated/macros.h"
#include "display.h"


/*
* Algorithme de Bellman ford.
*/
void initializeAll(uint32_t nberNode, uint32_t source, int64_t *distance, int32_t *predecessor);

bool checkNegCycle(graph_t *input_graph, int64_t *distance);

void bellman_ford(datas_threads_t *datas_threads, uint32_t source, int64_t *distance, int32_t *predecessor, char *info_verbose);

/*
* Retrace le chemin allant du noeud source au noeud d'arrivée.
*/
void completePathList(outputGraph_t *output_graph, uint32_t *pathListAux, uint32_t source, int32_t *predecessor);

void reversePathList(outputGraph_t *output_graph, uint32_t *pathListAux);

uint8_t getPath(uint32_t source, outputGraph_t *output_graph, int32_t *predecessor);


#endif // LEPL1503__BELLMAN_FORD__