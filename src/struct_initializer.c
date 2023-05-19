#include "../include/struct_initializer.h"


/*
* Initialise le graphe d'entrée en y ajoutant le nombre de noeuds ainsi que le nombre d'arêtes.
* Alloue également de la mémoire pour les pointeurs de tableaux 'from', 'to' et 'weight'.
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' contenant les données communes aux threads.
* @param nberNodes: nombre entier représentant le nombre de noeuds du graphe d'entrée.
* @param nberEdges: nombre entier représentant le nombre de liens du graphe d'entrée.
*
* @return: EXIT_SUCCESS si tout s'est correctement déroulé. EXIT_FAILURE sinon.
*/
uint8_t initializeInputGraph(datas_threads_t *datas_threads, uint32_t nberNodes, uint32_t nberEdges) {

    // Initialise le graphe d'entrée.
    datas_threads->input_graph = (graph_t *) malloc(sizeof(graph_t));
    if (datas_threads->input_graph == NULL) {
        free(datas_threads);
        printMethodFailed("malloc", errno);
        return EXIT_FAILURE;
    }

    // Update le nombre de noeuds et d'arêtes du graphe d'entrée.
    datas_threads->input_graph->nberNodes = nberNodes;
    datas_threads->input_graph->nberEdges = nberEdges;

    // Affiche les informations du graphe d'entrée.
    if (datas_threads->verbose) {
        printInputGraphInfo(nberNodes, nberEdges);
    }

    // Alloue de la mémoire pour le tableau de structure 'edge_t'.
    datas_threads->input_graph->edges = (edge_t *) malloc(datas_threads->input_graph->nberEdges * sizeof(edge_t));
    if (datas_threads->input_graph->edges == NULL) {
        free(datas_threads->input_graph);
        printMemoryAllocationError("datas_threads->input_graph->edges", errno);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/*
* Initialise les valeurs par défaut de la partie arguments de la structure 'datas_threads_t'.
*
* @param datas_threads : pointeur vers la structure 'datas_threads_t' à initialiser.
*
* @return: void.
*/
void initializeDefaultArgs(datas_threads_t *datas_threads) {
    datas_threads->nb_threads = 4;
    datas_threads->verbose = false;
    datas_threads->fp_output = stdout;
    datas_threads->input_fp = NULL;
}


/*
* Initialise la structure 'data_thread_algorithm_t' qui représente les informations nécessaires pour les threads.
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' contenant les données communes aux différents threads.
* @params data_threads_algo : pointeur vers la structure 'datas_threads_algorithm_t' utilisé par les threads dans la phase algorithmique du programme.
* @param lastNode: nombre entier non signé (sur 32 bits) représentant le dernier noeud du graphe (là où commence le thread).
* @param nber_iteration: nombre entier non signé (sur 32 bits) représentant le nombre d'itérations de l'algorithme que doit effectuer le thread.
*
* @return: void.
*/
void initialize_Data_algorithm_threads(datas_threads_t *datas_threads, datas_threads_algorithm_t * datas_threads_algo, uint32_t lastNode, uint32_t nber_iteration) {

    // Initialise les valeurs de la structure 'data_thread_algorithm_t'
    datas_threads_algo->datas_threads = datas_threads;
    datas_threads_algo->lastNode = lastNode;
    datas_threads_algo->nber_iteration = nber_iteration;
}