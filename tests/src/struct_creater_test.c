#include "../include/struct_creater_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_struct_creater(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_struct_creater(void) {
    return EXIT_SUCCESS;
}


/*
* Teste la fonction create_basic_InputGraph.
* Lit le fichier binaire 'SmallGraph' et vérifie que le graphe d'entrée a bien été modifié.
*/
void create_basic_InputGraph_Test(void) {

    datas_threads_t *datas_threads = malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }

    if ((datas_threads->input_fp = fopen("binary_files/general_case/input/SmallGraph", "rb")) == NULL) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    datas_threads->verbose = false;

    create_basic_InputGraph(datas_threads);

    CU_ASSERT_EQUAL(datas_threads->input_graph->nberNodes, 4);
    CU_ASSERT_EQUAL(datas_threads->input_graph->nberEdges, 5);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->input_graph->edges);

    free_graph_t(datas_threads->input_graph);

    if (closeFile(datas_threads->input_fp, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    free(datas_threads);
}


/*
* Teste la fonction createOutputGraph.
* Crée un graphe de sortie et vérifie que les données ont bien été modifiées.
*/
void createOutputGraph_Test(void) {
    
    uint32_t source = 1;
    outputGraph_t *outputGraph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (outputGraph == NULL) {
        printVariableNULL("outputGraph");
        exit(EXIT_FAILURE);
    }
    outputGraph->nberNodes = 4;

    int64_t *distance = (int64_t *) malloc(4 * sizeof(int64_t));
    if (distance == NULL) {
        free(outputGraph);
        printVariableNULL("distance");
        exit(EXIT_FAILURE);
    }
    memcpy(distance, (int64_t[]) {INFINITY_VALUE, 0, 9, -11}, 4 * sizeof(int64_t));

    int32_t *predecessor = (int32_t *) malloc(4 * sizeof(int32_t));
    if (predecessor == NULL) {
        free(distance);
        free(outputGraph);
        printVariableNULL("predecessor");
        exit(EXIT_FAILURE);
    }
    memcpy(predecessor, (int32_t[]) {1, 0, 0, 3}, 4 * sizeof(int32_t));

    createOutputGraph(outputGraph, source, distance, predecessor);

    CU_ASSERT_EQUAL(outputGraph->nberNodes, 4);
    CU_ASSERT_EQUAL(outputGraph->sourceNode, 1);
    CU_ASSERT_EQUAL(outputGraph->destinationNode, 2);
    CU_ASSERT_EQUAL(outputGraph->totalCost, 9);
    CU_ASSERT_EQUAL(outputGraph->pathLength, 3);
    CU_ASSERT_EQUAL(outputGraph->pathList[0], 1);
    CU_ASSERT_EQUAL(outputGraph->pathList[1], 0);
    CU_ASSERT_EQUAL(outputGraph->pathList[2], 2);

    free(distance);
    free(predecessor);
    free_outputGraph_t(outputGraph);
}


/*
* Teste la fonction create_Datas_Threads.
* Crée une structure 'datas_threads_t' et vérifie que les données ont bien été modifiées.
*/
void create_Datas_Threads_Normal_Test(void) {

    FILE *fp_inp;
    if ((fp_inp = fopen("input_file1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    if (closeFile(fp_inp, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    FILE *fp_out;
    if ((fp_out = fopen("output_test1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    if (closeFile(fp_out, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    int argc = 7;
    char *argv[] = {"./unit_test", "-n", "6", "-v", "-f", "output_test1", "input_file1"};
    bool help_msg = false;

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    uint8_t resultSuccess = create_Datas_Threads(datas_threads, argc, argv, &help_msg);

    remove("input_file1");
    remove("output_test1");

    CU_ASSERT_PTR_NOT_NULL(datas_threads);
    CU_ASSERT_FALSE(help_msg);
    CU_ASSERT_EQUAL(resultSuccess, EXIT_SUCCESS);

    CU_ASSERT_EQUAL(datas_threads->nb_threads, 6);
    CU_ASSERT_TRUE(datas_threads->verbose);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->fp_output);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->input_fp);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->mutex);

    free(datas_threads->mutex);
    if (closeFile(datas_threads->fp_output, true) == EXIT_FAILURE) {
        closeFile(datas_threads->input_fp, true);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }
    if (closeFile(datas_threads->input_fp, true) == EXIT_FAILURE) {
        free(datas_threads);
        exit(EXIT_FAILURE);
    }
    free(datas_threads);
}


/*
* Teste la fonction create_Datas_Threads.
* Crée une structure datas_threads_t et vérifie que les données ont bien été modifiées.
* Ici, le paramètre -h est utilisé, le programme doit afficher le message d'aide et renvoyer NULL.
*/
void create_Datas_Threads_Help_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    int argc = 8;
    char *argv[] = {"./unit_test", "-n", "6", "-v", "-f", "output_test1", "-h", "input_file1"};
    bool help_msg = false;

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }

    FILE *fp_output;
    if ((fp_output = fopen("output_test1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }
    if (closeFile(fp_output, true) == EXIT_FAILURE) {
        remove("output_test1");
        free(datas_threads);
        exit(EXIT_FAILURE);
    }

    uint8_t resultSuccess = create_Datas_Threads(datas_threads, argc, argv, &help_msg);

    CU_ASSERT_TRUE(help_msg);
    CU_ASSERT_EQUAL(resultSuccess, EXIT_SUCCESS);


    char expected_buffer[550] = "";
    strcat(expected_buffer, "UTILISATION:\n    ./unit_test [OPTIONS] input_file\n");
    strcat(expected_buffer, "    input_file: chemin vers le fichier d'instance representant le "
            "graphe a traiter.\n");
    strcat(expected_buffer, "    -f output_file: chemin vers le fichier qui contiendra le resultat "
        "de programme, au format specifie dans l'enonce. Defaut : stdout.\n");
    strcat(expected_buffer, "    -n n_threads: nombre de threads de calcul executant "
            "l'algorithme de plus court chemin. Defaut : 4.\n");
    strcat(expected_buffer, "    -v: autorise les messages de debug. Si ce n'est pas active, "
            "aucun message de ce type ne peut etre affiche, excepte les "
            "messages d'erreur en cas d'echec. Defaut : false.\n");
    const uint32_t size_buffer = 550;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


/*
* Teste la fonction create_Datas_Threads_Args.
* Crée une structure datas_threads_t et vérifie que les données ont bien été modifiées.
*/
void create_Datas_Threads_Args_Test(void) {

    FILE *fp_inp;
    if ((fp_inp = fopen("input_file1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
     
    if (closeFile(fp_inp, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    FILE *fp_out;
    if ((fp_out = fopen("output_test1", "wb")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }
    
    if (closeFile(fp_out, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    int argc = 7;
    char *argv[] = {"./unit_test", "-n", "6", "-v", "-f", "output_test1", "input_file1"};
    bool help_msg = false;

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    create_Datas_Threads_Args(datas_threads, argc, argv, &help_msg);

    remove("input_file1");
    remove("output_test1");

    CU_ASSERT_PTR_NOT_NULL(datas_threads);
    CU_ASSERT_FALSE(help_msg);

    CU_ASSERT_EQUAL(datas_threads->nb_threads, 6);
    CU_ASSERT_TRUE(datas_threads->verbose);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->fp_output);
    CU_ASSERT_PTR_NOT_NULL(datas_threads->input_fp);

    if (closeFile(datas_threads->fp_output, true) == EXIT_FAILURE) {
        closeFile(datas_threads->input_fp, true);
        free(datas_threads);
        exit(EXIT_FAILURE);
    }
    if (closeFile(datas_threads->input_fp, true) == EXIT_FAILURE) {
        free(datas_threads);
        exit(EXIT_FAILURE);
    }
    free(datas_threads);
}


/*
* Fonction principale lançant les tests du module struct_creater.c.
* Cette fonction est appelée par le main du programme test.
*/
int main_struct_creater_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test struct_creater.c", init_suite_struct_creater, clean_suite_struct_creater);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "create_basic_InputGraph_Test", create_basic_InputGraph_Test)) ||
        (NULL == CU_add_test(pSuite, "createOutputGraph_Test", createOutputGraph_Test)) ||
        (NULL == CU_add_test(pSuite, "create_Datas_Threads_Normal_Test", create_Datas_Threads_Normal_Test)) ||
        (NULL == CU_add_test(pSuite, "create_Datas_Threads_Help_Test", create_Datas_Threads_Help_Test)) ||
        (NULL == CU_add_test(pSuite, "create_Datas_Threads_Args_Test", create_Datas_Threads_Args_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}