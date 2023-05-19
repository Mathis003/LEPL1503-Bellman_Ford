#include "../include/struct_free.h"


/*
* Libère toute la mémoire liée à la structure 'outputGraph_t'.
*
* @param outputGraph: pointeur vers la structure 'outputGraph_t' à désallouer.
*
* @return void.
*/
void free_outputGraph_t(outputGraph_t *outputGraph) {
    free(outputGraph->pathList);
    free(outputGraph);
}


/*
* Libère toute la mémoire liée à la structure 'graph_t'.
*
* @param input_graph: pointeur vers la structure' graph_t' à désallouer.
*
* @return: void.
*/
void free_graph_t(graph_t *input_graph) {
    free(input_graph->edges);
    free(input_graph);
}


/*
* Libère toute la mémoire liée à la structure 'datas_threads_t'.
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' à désallouer.
*
* @return: EXIT_SUCCESS si la libération de mémoire est réussie, EXIT_FAILURE sinon.
*/
uint8_t free_datas_threads(datas_threads_t *datas_threads) {

    uint8_t returnValue = EXIT_SUCCESS;
    free(datas_threads->mutex);
    free_graph_t(datas_threads->input_graph);
    if (datas_threads->fp_output != stdout) {
        if (closeFile(datas_threads->fp_output, true) == EXIT_FAILURE) {
            returnValue = EXIT_FAILURE;
        }
    }
    free(datas_threads);
    return returnValue;
}


/*
* Libère toute la mémoire liée à la structure 'data_thread_algorithm_t'.
*
* @param datas_threads_algorithm: pointeur vers la structure 'data_thread_algorithm_t' à désallouer.
* @param predecessor: pointeur vers le tableau de prédécesseurs à désallouer.
* @param distance: pointeur vers le tableau de distance à désallouer.
* @param output_graph: pointeur vers la structure 'outputGraph_t' à désallouer.
* @param info_verbose: pointeur vers la chaîne de caractères à désallouer.
*
* @return: void.
*/
void free_datas_threads_algorithm_t(int32_t *predecessor, int64_t *distance, outputGraph_t *output_graph, char *info_verbose, datas_threads_algorithm_t *datas_threads_algorithm) {
    free(predecessor);
    free(distance);
    free(info_verbose);
    free_outputGraph_t(output_graph);
    free(datas_threads_algorithm);
}