#include "../include/struct_free_test.h"


/*
* Note: C'est en réalité compliqué de tester ce module car il n'y a pas de fonction
*       qui permet de vérifier si un pointeur est bien libéré. Ce module est donc simplement
*       là pour vérifier que les fonctions de libération de mémoire ne plantent pas à l'exécution.
*       Nous n'avons pas trouvé de solution pour vérifier que la mémoire est bien libérée,
*       mais nous avons utilisé des outils qui permettent de vérifier les fuites de mémoire (Valgrind).
*       Nous avons donc vérifié que les fonctions de libération de mémoire ne créent pas de fuites.
*       En effet, nous n'avons aucune fuite de mémoire dans notre programme (dans le main et dans les tests).
*/


/*
* Initialise la suite de tests.
*/
int init_suite_struct_free(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_struct_free(void) {
    return EXIT_SUCCESS;
}


/*
* Teste la fonction free_outputGraph_t().
* Vérifie que la fonction ne plante pas à l'éxecution.
*/
void free_outputGraph_t_Test(void) {

    outputGraph_t *outputGraph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (outputGraph == NULL) {
        printVariableNULL("outputGraph");
        exit(EXIT_FAILURE);
    }
    outputGraph->nberNodes = 10;

    outputGraph->pathList = (uint32_t *) malloc(10 * sizeof(uint32_t));
    if (outputGraph->pathList == NULL) {
        printVariableNULL("outputGraph->pathList");
        exit(EXIT_FAILURE);
    }

    free_outputGraph_t(outputGraph);

    // Pour vérifier si la fonction ne plante pas.
    // La mémoire est libérée (vu avec Valgrind).
    CU_ASSERT_TRUE(true);
}



/*
* Teste la fonction free_graph_t().
* Vérifie que la fonction ne plante pas à l'éxecution.
*/
void free_graph_t_Test(void) {
    
    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMemoryAllocationError("datas_threads", errno);
        exit(EXIT_FAILURE);
    }
    datas_threads->verbose = false;
    
    if (initializeInputGraph(datas_threads, 10, 13) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    free_graph_t(datas_threads->input_graph);
    free(datas_threads);

    // Pour vérifier si la fonction ne plante pas à l'éxecution.
    // La mémoire est libérée (vu avec Valgrind)
    CU_ASSERT_TRUE(true);
}


/*
* Teste la fonction free_datas_threads().
* Vérifie que la fonction ne plante pas à l'éxecution.
*/
void free_datas_threads_Test(void) {

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    initializeDefaultArgs(datas_threads);

    datas_threads->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    if (datas_threads->mutex == NULL) {
        printVariableNULL("datas_threads->mutex");
        exit(EXIT_FAILURE);
    }
    
    datas_threads->verbose = false;
    
    if (initializeInputGraph(datas_threads, 10, 13) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    uint8_t result = free_datas_threads(datas_threads);

    // Pour vérifier si la fonction ne plante pas à l'éxecution.
    // La mémoire est libérée (vu avec Valgrind)
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
}


/*
* Teste la fonction free_datas_threads_algorithm_t().
* Vérifie que la fonction ne plante pas à l'éxecution.
*/
void free_datas_threads_algorithm_t_Test(void) {

    int32_t *predecessor = (int32_t *) malloc(10 * sizeof(int32_t));
    if (predecessor == NULL) {
        printVariableNULL("predecessor");
        exit(EXIT_FAILURE);
    }
    int64_t *distance = (int64_t *) malloc(10 * sizeof(int64_t));
    if (distance == NULL) {
        printVariableNULL("distance");
        exit(EXIT_FAILURE);
    }

    outputGraph_t *output_graph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (output_graph == NULL) {
        printVariableNULL("output_graph");
        exit(EXIT_FAILURE);
    }
    output_graph->nberNodes = 10;

    output_graph->pathList = (uint32_t *) malloc(10 * sizeof(uint32_t));
    if (output_graph->pathList == NULL) {
        printVariableNULL("output_graph->pathList");
        exit(EXIT_FAILURE);
    }
    char *info_verbose = calloc(100, sizeof(char));
    if (info_verbose == NULL) {
        printVariableNULL("info_verbose");
        exit(EXIT_FAILURE);
    }

    datas_threads_algorithm_t *datas_threads_algorithm = (datas_threads_algorithm_t *) malloc(sizeof(datas_threads_algorithm_t));
    if (datas_threads_algorithm == NULL) {
        printVariableNULL("datas_threads_algorithm");
        exit(EXIT_FAILURE);
    }


    free_datas_threads_algorithm_t(predecessor, distance, output_graph, info_verbose, datas_threads_algorithm);

    // Pour vérifier si la fonction ne plante pas à l'éxecution.
    // La mémoire est libérée (vu avec Valgrind)
    CU_ASSERT_TRUE(true);
}


/*
* Fonction principale lançant les tests du module struct_free.c.
* Cette fonction est appelée par le main du programme test.
*/
int main_struct_free_test() {

    /* Ajoute les tests au registre de tests */
    CU_pSuite pSuite = CU_add_suite("Test struct_free.c", init_suite_struct_free, clean_suite_struct_free);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute les tests à la suite de tests */
    if ((NULL == CU_add_test(pSuite, "free_outputGraph_t_Test", free_outputGraph_t_Test)) ||
        (NULL == CU_add_test(pSuite, "free_graph_t_Test", free_graph_t_Test)) ||
        (NULL == CU_add_test(pSuite, "free_datas_threads_Test", free_datas_threads_Test)) ||
        (NULL == CU_add_test(pSuite, "free_datas_threads_algorithm_t_Test", free_datas_threads_algorithm_t_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}