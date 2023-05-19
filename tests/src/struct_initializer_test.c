#include "../include/struct_initializer_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_struct_initializer(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_struct_initializer(void) {
    return EXIT_SUCCESS;
}


/*
* Teste la fonction initializeInputGraph.
* Vérifie que le graphe d'entrée a bien été initialisé.
*/
void initializeInputGraph_Test(void) {

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }

    datas_threads->verbose = false;

    uint8_t result = initializeInputGraph(datas_threads, 10, 13);

    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
    CU_ASSERT_EQUAL(datas_threads->input_graph->nberNodes, 10);
    CU_ASSERT_EQUAL(datas_threads->input_graph->nberEdges, 13);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->input_graph->edges);

    if (result == EXIT_SUCCESS) {
        free_graph_t(datas_threads->input_graph);
        free(datas_threads);
    }
}


/*
* Teste la fonction initializeDefaultArgs.
* Vérifie que les arguments par défaut ont bien été initialisés.
*/
void initializeDefaultArgs_Test(void) {

    datas_threads_t args;
    initializeDefaultArgs(&args);

    CU_ASSERT_EQUAL(args.nb_threads, 4);
    CU_ASSERT_FALSE(args.verbose);
    CU_ASSERT_PTR_EQUAL(args.fp_output, stdout);
    CU_ASSERT_PTR_NULL(args.input_fp);
}


/*
* Teste la fonction initialize_Data_algorithm_threads().
* Créé une structure 'datas_threads_t' et vérifie que les données ont bien été modifiées.
*/
void initialize_Data_algorithm_threads_Test(void) {

    datas_threads_t datas_threads;
    datas_threads.nb_threads = 5;
    datas_threads.verbose = true;
    datas_threads.input_graph = (graph_t *) malloc(sizeof(graph_t));
    if (datas_threads.input_graph == NULL) {
        printMemoryAllocationError("datas_threads.input_graph", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t lastNode = 2;
    uint32_t nber_iteration = 5;
    datas_threads_algorithm_t *datas_threads_algo = (datas_threads_algorithm_t *) malloc(sizeof(datas_threads_algorithm_t));
    if (datas_threads_algo == NULL) {
        printMemoryAllocationError("datas_threads_algo", errno);
        exit(EXIT_FAILURE);
    }

    initialize_Data_algorithm_threads(&datas_threads, datas_threads_algo, lastNode, nber_iteration);

    CU_ASSERT_EQUAL(datas_threads_algo->datas_threads->nb_threads, 5);
    CU_ASSERT_TRUE(datas_threads_algo->datas_threads->verbose);
    CU_ASSERT_PTR_NOT_NULL(datas_threads_algo->datas_threads->input_graph);
    CU_ASSERT_EQUAL(datas_threads_algo->lastNode, 2);
    CU_ASSERT_EQUAL(datas_threads_algo->nber_iteration, 5);

    free(datas_threads_algo->datas_threads->input_graph);
    free(datas_threads_algo);
}


/*
* Fonction principale lançant les tests du module struct_initializer.c.
* Cette fonction est appelée par le main du programme test.
*/
int main_struct_initializer_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test struct_initializer.c", init_suite_struct_initializer, clean_suite_struct_initializer);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "initializeInputGraph_Test", initializeInputGraph_Test)) ||
        (NULL == CU_add_test(pSuite, "initializeDefaultArgs_Test", initializeDefaultArgs_Test)) ||
        (NULL == CU_add_test(pSuite, "initialize_Data_algorithm_threads_Test", initialize_Data_algorithm_threads_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}