#include "../include/threads_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_threads(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_threads(void) {
    return EXIT_SUCCESS;
}


// Fonction d'aide pour les tests.
void *funct_threads_waiting(void *datas_threads) {
    sleep(1);

    // Retourne un pointeur (void *) différent de NULL car la fonction n'échoue pas.
    return (void *) 42;
}


/*
* Teste la fonction wait_threads.
* Vérifie que les threads ont bien été attendus 1sec.
*/
void wait_threads_Test(void) {

    pthread_t threads[4];
    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    datas_threads->nb_threads = 4;

    for (uint8_t i = 0; i < datas_threads->nb_threads; i++) {
        
        if (pthread_create(&threads[i], NULL, funct_threads_waiting, NULL) != 0) {
            printThreadError(i);
            exit(EXIT_FAILURE);
        }
    }
    
    uint8_t result = wait_threads(threads, datas_threads);
    
    CU_ASSERT_EQUAL(result, EXIT_SUCCESS);

    free(datas_threads);
}


/*
* Teste la fonction function_thread_algorithm.
* Lit le fichier binaire 'SmallGraph' et vérifie que le graphe d'entrée a bien été modifié.
*/
void function_thread_algorithm_Test(void) {

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    datas_threads->nb_threads = 3;
    datas_threads->verbose = false;

    if ((datas_threads->input_fp = fopen("binary_files/general_case/input/SmallGraph", "rb")) == 0) {
        free(datas_threads);
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    
    read_entry_file(datas_threads);

    datas_threads->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    if (datas_threads->mutex == NULL) {
        free_graph_t(datas_threads->input_graph);
        free(datas_threads);
        printVariableNULL("datas_threads->mutex");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(datas_threads->mutex, NULL) != 0) {
        free_graph_t(datas_threads->input_graph);
        free(datas_threads->mutex);
        free(datas_threads);
        printMethodFailed("pthread_mutex_init", errno);
        exit(EXIT_FAILURE);
    }

    if ((datas_threads->fp_output = fopen("output", "wb")) == 0) {
        free_graph_t(datas_threads->input_graph);
        pthread_mutex_destroy(datas_threads->mutex);
        free(datas_threads->mutex);
        free(datas_threads);
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    
    datas_threads_algorithm_t *datas_threads_algo1 = (datas_threads_algorithm_t *) malloc(sizeof(datas_threads_algorithm_t));
    if (datas_threads_algo1 == NULL) {
        free_graph_t(datas_threads->input_graph);
        pthread_mutex_destroy(datas_threads->mutex);
        free(datas_threads->mutex);
        free(datas_threads);
        printVariableNULL("datas_threads_algo1");
        exit(EXIT_FAILURE);
    }
    initialize_Data_algorithm_threads(datas_threads, datas_threads_algo1, 1, 2);

    void *datas_threads_algo = (void *) datas_threads_algo1;

    function_thread_algorithm(datas_threads_algo);

    if (closeFile(datas_threads->fp_output, true) == EXIT_FAILURE) {
        free_graph_t(datas_threads->input_graph);
        pthread_mutex_destroy(datas_threads->mutex);
        free(datas_threads->mutex);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_destroy(datas_threads->mutex) != 0) {
        free_graph_t(datas_threads->input_graph);
        free(datas_threads->mutex);
        free(datas_threads);
        printMethodFailed("pthread_mutex_destroy", errno);
        exit(EXIT_FAILURE);
    }

    free_graph_t(datas_threads->input_graph);
    free(datas_threads->mutex);
    free(datas_threads);


    // Lit le fichier output et vérifie que les résultats sont corrects.
    FILE *fp_output;
    if ((fp_output = fopen("output", "rb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    uint32_t expected_sourceNode[] = {1, 2};
    uint32_t expected_destinationNode[] = {0, 1};
    int64_t expected_totalCost[] = {6, 5};
    uint32_t expected_pathLength[] = {4, 4};
    uint32_t expected_nodePath[2][4] = {
        {1, 2, 3, 0},
        {2, 3, 0, 1}
    };

    uint32_t sourceNode;
    uint32_t destinationNode;
    int64_t totalCost;
    uint32_t pathLength;
    uint32_t nodePath;


    for (uint8_t i = 0; i < 2; i++) {

        if (fread(&sourceNode, sizeof(uint32_t), 1, fp_output) != 1) {
            printMethodFailed("fread", errno);
            exit(EXIT_FAILURE);
        }
        if (fread(&destinationNode, sizeof(uint32_t), 1, fp_output) != 1) {
            printMethodFailed("fread", errno);
            exit(EXIT_FAILURE);
        }
        if (fread(&totalCost, sizeof(int64_t), 1, fp_output) != 1) {
            printMethodFailed("fread", errno);
            exit(EXIT_FAILURE);
        }
        if (fread(&pathLength, sizeof(uint32_t), 1, fp_output) != 1) {
            printMethodFailed("fread", errno);
            exit(EXIT_FAILURE);
        }

        CU_ASSERT_EQUAL(be32toh(sourceNode), expected_sourceNode[i]);
        CU_ASSERT_EQUAL(be32toh(destinationNode), expected_destinationNode[i]);
        CU_ASSERT_EQUAL(be64toh(totalCost), expected_totalCost[i]);
        CU_ASSERT_EQUAL(be32toh(pathLength), expected_pathLength[i]);

        for (uint32_t j= 0; j < be32toh(pathLength); j++) {

            if (fread(&nodePath, sizeof(uint32_t), 1, fp_output) != 1) {
                printMethodFailed("fread", errno);
                exit(EXIT_FAILURE);
            }

            CU_ASSERT_EQUAL(be32toh(nodePath), expected_nodePath[i][j]);
        }
        
    }

    if (closeFile(fp_output, true) == EXIT_FAILURE) {
        remove("output");
        exit(EXIT_FAILURE);
    }

    remove("output");
}


/*
* Teste la fonction launch_algorithm_threads.
* Vérifie que les résultats sont corrects.
*/
void launch_algorithm_threads_Test(void) {

    // Création de la structure datas_threads_t.
    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMemoryAllocationError("datas_threads", errno);
        exit(EXIT_FAILURE);
    }

    datas_threads->nb_threads = 3;
    datas_threads->verbose = false;

    datas_threads->mutex = (pthread_mutex_t *) malloc(sizeof(pthread_mutex_t));
    if (datas_threads->mutex == NULL) {
        printMemoryAllocationError("datas_threads->mutex", errno);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(datas_threads->mutex, NULL) != 0) {
        printMethodFailed("pthread_mutex_init", errno);
        free(datas_threads->mutex);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    if ((datas_threads->input_fp = fopen("binary_files/general_case/input/SmallGraph", "rb")) == NULL) {
        printMethodFailed("fopen", errno);
        free(datas_threads->mutex);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    if ((datas_threads->fp_output = fopen("output", "wb")) == NULL) {
        printMethodFailed("fopen", errno);
        free(datas_threads->mutex);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    // Création du tableau de threads.
    pthread_t threads[datas_threads->nb_threads];

    // Lis le fichier d'entrée et stocke les données dans la structure 'inputGraph_t' contenu dans datas_threads->input_graph.
    // Ferme aussi le fichier d'entrée.
    if (read_entry_file(datas_threads) == EXIT_FAILURE) {
        CU_ASSERT_FALSE(true);
        exit(EXIT_FAILURE); // Tous les free(), destruction du mutex et fermeture des fichiers sont faits dans la fonction.
    }
    
    // Ecrit le nombre de noeuds du graphe dans le fichier de sortie.
    if (writeNberNodes_OutputFile(datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        exit(EXIT_FAILURE);
    }

    // Lance les threads pour effectuer l'algorithme de Bellman-Ford + stocker les résultats dans le fichier binaire de sortie.
    if (launch_algorithm_threads(threads, datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        exit(EXIT_FAILURE);
    }

    // Attend que tous les threads aient fini d'effectuer leur travail.
    if (wait_threads(threads, datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        exit(EXIT_FAILURE);
    }

    // Détruit le mutex qui n'est plus utile.
    if (pthread_mutex_destroy(datas_threads->mutex) != 0) {
        printErrorMutexDestory();
        free_datas_threads(datas_threads);
        exit(EXIT_FAILURE);
    }

    free_datas_threads(datas_threads);


    // Recrée le fichier dans l'ordre !
    uint8_t sorted_bin_result = create_sorted_bin_file("output");
    CU_ASSERT_EQUAL(sorted_bin_result, EXIT_SUCCESS);

    if (sorted_bin_result == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    // Vérifie que le fichier de sortie est correct.
    CU_ASSERT_TRUE(compare_two_outputFile("output", "binary_files/general_case/output_Python/Output_SmallGraph", false));

    // Supprime le fichier de sortie.
    remove("output");
}



/*
* Fonction principale lançant les tests du module threads.c.
* Cette fonction est appelée par le main du programme test.
*/
int main_threads_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test threads.c", init_suite_threads, clean_suite_threads);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "wait_threads_Test", wait_threads_Test)) ||
        (NULL == CU_add_test(pSuite, "function_thread_algorithm_Test", function_thread_algorithm_Test)) ||
        (NULL == CU_add_test(pSuite, "launch_algorithm_threads_Test", launch_algorithm_threads_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}