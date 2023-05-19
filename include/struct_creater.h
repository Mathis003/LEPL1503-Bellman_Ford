#ifndef LEPL1503__STRUCT_CREATER__
#define LEPL1503__STRUCT_CREATER__

#include <getopt.h>

#include "unassociated/struct.h"

#include "display.h"
#include "struct_initializer.h"
#include "struct_free.h"
#include "threads.h"


/*
* Création des structures de graphe.
*/
uint8_t create_basic_InputGraph(datas_threads_t *datas_threads);

uint8_t createOutputGraph(outputGraph_t *outputGraph, uint32_t source, int64_t *distance, int32_t *predecessor);


/*
* Création des structures pour les threads.
*/
uint8_t create_Datas_Threads(datas_threads_t *datas_threads, int argc, char *argv[], bool *help_msg);

uint8_t create_Datas_Threads_Args(datas_threads_t *datas_threads, uint8_t argc, char *argv[], bool *help_msg);


#endif // LEPL1503__STRUCT_CREATER__