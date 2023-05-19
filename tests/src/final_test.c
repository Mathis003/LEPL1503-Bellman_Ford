#include "../include/final_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_all(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_all(void) {
    return EXIT_SUCCESS;
}


/*
* Fonction principale du programme permettant de créer le fichier binaire de sortie avec tous
* les résultats à l'intérieur en fonction d'un graphe d'entrée donné.
*
* Cette fonction est un "presque" copier/coller de la fonction main du programme C.
*
* @params input_name: tableau de caractère (= pointeur) représentant le nom du fichier d'entrée.
* @params output_name: tableau de caractère (= pointeur) représentant le nom du fichier de sortie.
* @params nb_thread: nombre entier non signé (sur 8 bits) représentant le nombre de threads à utiliser pour le calcul des chemins.
*
* @return: nombre entier => EXIT_SUCCESS si le programme s'est exécuté correctement, EXIT_FAILURE sinon
*/
uint8_t main_toCreate_binaryFile(char input_name[], char output_name[], uint8_t nb_thread) {

    char buffer_nb_threads[100];
    if (sprintf(buffer_nb_threads, "%d", nb_thread) < 0) {
        printMethodFailed("sprintf", errno);
        return EXIT_FAILURE;
    }
    
    int argc = 6;
    char *argv[] = {"./main", "-n", buffer_nb_threads, "-f", output_name, input_name};

    bool help_msg = false;

    // Création de la structure datas_threads_t.
    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMethodFailed("malloc", errno);
        return EXIT_FAILURE;
    }

    if (create_Datas_Threads(datas_threads, argc, argv, &help_msg) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    // Création du tableau de threads.
    pthread_t threads[datas_threads->nb_threads];

    // Lis le fichier d'entrée et stocke les données dans la structure 'inputGraph_t' contenu dans datas_threads->input_graph.
    // Ferme aussi le fichier d'entrée.
    if (read_entry_file(datas_threads) == EXIT_FAILURE) {
        return EXIT_FAILURE; // Tous les free(), destruction du mutex et fermeture des fichiers sont fait dans la fonction.
    }

    // Si le nombre de threads est supérieur au nombre de noeuds du graphe.
    if (datas_threads->input_graph->nberNodes < datas_threads->nb_threads) {
        if (datas_threads->input_graph->nberNodes != 0) { // Pour ne pas avoir un nbre de threads égal à 0.
            datas_threads->nb_threads = datas_threads->input_graph->nberNodes; // Un thread par noeud.
        }
    }
    
    // Ecrit le nombre de noeuds du graphe dans le fichier de sortie.
    if (writeNberNodes_OutputFile(datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Cas où le graphe est vide.
    if (datas_threads->input_graph->nberNodes == 0) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_SUCCESS;
    }

    if (datas_threads->verbose) {
        printAlgorithme_Encodage_Section();
    }

    // Lance les threads pour effectuer l'algorithme de Bellman-Ford + stocker les résultats dans le fichier binaire de sortie.
    if (launch_algorithm_threads(threads, datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Attend que tous les threads aient fini d'effectuer leur travail.
    if (wait_threads(threads, datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Détruit le mutex qui n'est plus utile.
    if (pthread_mutex_destroy(datas_threads->mutex) != 0) {
        printErrorMutexDestory();
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Libère la mémoire allouée restante.
    free_datas_threads(datas_threads);

    return EXIT_SUCCESS;
}



/*
* Fonction d'aide.
*/
void compareBinaryFiles_ALL_LIMITS_CASES(uint8_t nb_threads) {

    char *name_input[13] = {"Graph_Bidirectionnel_Edges",
                         "Graph_Cost_Null",
                         "Graph_EMPTY",
                         "Graph_MoreThanTwoEdges_BetweenNodes",
                         "Graph_NO_EDGES",
                         "Graph_One_Edge",
                         "Graph_WithIsolateNodes",
                         "Graph_WithMaxValue",
                         "Graph_WithMinValue",
                         "Graph_WithMultiplePath",
                         "Graph_WithMultipleWaysWithSameCost",
                         "Graph_WithNegCycle",
                         "Graph_WithSameEdges10Times"};

    uint8_t result_progr;
    char input_path[100];
    char output_c_path[100];
    char output_python_path[100];

    for (uint8_t i = 0; i < 13; i++) {
        snprintf(input_path, sizeof(input_path), "binary_files/limits_case/input/%s", name_input[i]);
        snprintf(output_c_path, sizeof(output_c_path), "binary_files/limits_case/output_C/Output_%s", name_input[i]);
        snprintf(output_python_path, sizeof(output_python_path), "binary_files/limits_case/output_Python/Output_%s", name_input[i]);

        result_progr = main_toCreate_binaryFile(input_path, output_c_path, nb_threads);
        CU_ASSERT_EQUAL(result_progr, EXIT_SUCCESS);
        if (nb_threads > 1) {
            create_sorted_bin_file(output_c_path);
        }
        CU_ASSERT_TRUE(compare_two_outputFile(output_c_path, output_python_path, false));
    }
}

/*
* Teste, sur tous les graphes "cas limites", la création du fichier binaire de sortie avec un seul thread.
* Vérifie ensuite que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_ALL_LIMITS_CASES_MonoThread_Test(void) {
    compareBinaryFiles_ALL_LIMITS_CASES(1);
}


/*
* Teste, sur tous les graphes "cas limites", la création du fichier binaire de sortie avec un seul thread.
* Trie ensuite le fichier binaire de sortie (en créant un autre (avec le même nom)).
* Vérifie au final que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_ALL_LIMITS_CASES_BasicThread_Test(void) {
    compareBinaryFiles_ALL_LIMITS_CASES(4);
}


/*
* Teste, sur tous les graphes "cas limites", la création du fichier binaire de sortie avec un seul thread.
* Trie ensuite le fichier binaire de sortie (en créant un autre (avec le même nom)).
* Vérifie au final que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_ALL_LIMITS_CASES_LotOfThread_Test(void) {
    compareBinaryFiles_ALL_LIMITS_CASES(80);
}



/*
* Fonction d'aide.
*/
void compareBinaryFiles_RandomsGraph(uint8_t nb_threads) {

    char *name_input[5] = {"SmallGraph",
                           "NormalGraph",
                           "MediumGraph",
                           "BigGraph",
                           "HugeGraph"};

    uint8_t result_progr;
    char input_path[100];
    char output_c_path[100];
    char output_python_path[100];

    for (uint8_t i = 0; i < 5; i++) {
        snprintf(input_path, sizeof(input_path), "binary_files/general_case/input/%s", name_input[i]);
        snprintf(output_c_path, sizeof(output_c_path), "binary_files/general_case/output_C/Output_%s", name_input[i]);
        snprintf(output_python_path, sizeof(output_python_path), "binary_files/general_case/output_Python/Output_%s", name_input[i]);

        result_progr = main_toCreate_binaryFile(input_path, output_c_path, nb_threads);
        CU_ASSERT_EQUAL(result_progr, EXIT_SUCCESS);
        if (nb_threads > 1) {
            create_sorted_bin_file(output_c_path);
        }
        CU_ASSERT_TRUE(compare_two_outputFile(output_c_path, output_python_path, false));
    }
}


/*
* Teste, sur tous les graphes "random généraux", la création du fichier binaire de sortie avec un seul thread.
* Vérifie ensuite que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_RandomsGraph_MonoThread_Test(void) {
    compareBinaryFiles_RandomsGraph(1);
}


/*
* Teste, sur tous les graphes "random généraux", la création du fichier binaire de sortie avec un seul thread.
* Trie ensuite le fichier binaire de sortie (en créant un autre (avec le même nom)).
* Vérifie au final que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_RandomsGraph_BasicThread_Test(void) {
    compareBinaryFiles_RandomsGraph(4);
}


/*
* Teste, sur tous les graphes "random généraux", la création du fichier binaire de sortie avec un seul thread.
* Trie ensuite le fichier binaire de sortie (en créant un autre (avec le même nom)).
* Vérifie au final que le fichier binaire de sortie est identique à celui créé par le programme python.
*/
void compareBinaryFiles_RandomsGraph_LofOfThread_Test(void) {
    compareBinaryFiles_RandomsGraph(80);
}


/*
* Fonction principale lançant les tests finaux => reprenant tout le programme.
* Cette fonction est appelée par le main du programme test.
*/
int main_final_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test FINAL", init_suite_all, clean_suite_all);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "compareBinaryFiles_ALL_LIMITS_CASES_MonoThread_Test", compareBinaryFiles_ALL_LIMITS_CASES_MonoThread_Test)) ||
        (NULL == CU_add_test(pSuite, "compareBinaryFiles_ALL_LIMITS_CASES_BasicThread_Test", compareBinaryFiles_ALL_LIMITS_CASES_BasicThread_Test)) ||
        (NULL == CU_add_test(pSuite, "compareBinaryFiles_ALL_LIMITS_CASES_LotOfThread_Test", compareBinaryFiles_ALL_LIMITS_CASES_LotOfThread_Test)) ||
        (NULL == CU_add_test(pSuite, "compareBinaryFiles_RandomsGraph_MonoThread_Test", compareBinaryFiles_RandomsGraph_MonoThread_Test)) ||
        (NULL == CU_add_test(pSuite, "compareBinaryFiles_RandomsGraph_BasicThread_Test", compareBinaryFiles_RandomsGraph_BasicThread_Test)) ||
        (NULL == CU_add_test(pSuite, "compareBinaryFiles_RandomsGraph_LofOfThread_Test", compareBinaryFiles_RandomsGraph_LofOfThread_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}