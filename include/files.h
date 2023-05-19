#ifndef LEPL1503__FILES__
#define LEPL1503__FILES__

#include "unassociated/portable_endian.h"
#include "unassociated/macros.h"
#include "unassociated/struct.h"

#include "struct_creater.h"
#include "display.h"


uint8_t closeFile(FILE *fp, bool verbose);

/*
* Lecture du fichier binaire d'entré.
*/
uint8_t read_entry_file(datas_threads_t *datas_threads);

/*
* Ecriture du fichier binaire de sortie.
*/
uint8_t writeNberNodes_OutputFile(datas_threads_t *datas_threads);

uint8_t writeData_OutputFile(datas_threads_t *datas_threads, outputGraph_t *outputGraph, int64_t *distance);


#endif // LEPL1503__FILES__