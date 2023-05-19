#include "../include/files_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_files(void) {
    return EXIT_SUCCESS;
}

/*
* Clean la suite de tests.
*/
int clean_suite_files(void) {
    return EXIT_SUCCESS;
}


/*
* Teste la fonction read_entry_file.
* Lit le fichier binaire 'SmallGraph' et vérifie que le graphe d'entrée a bien été modifié.
*/
void read_entry_file_Test(void) {

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printVariableNULL("datas_threads");
        exit(EXIT_FAILURE);
    }
    datas_threads->verbose = false;

    if ((datas_threads->input_fp = fopen("binary_files/general_case/input/SmallGraph", "rb")) == NULL) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

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

    read_entry_file(datas_threads);

    // Vérifie que le graphe d'entrée a bien été modifié correctement.

    CU_ASSERT_EQUAL(datas_threads->input_graph->nberNodes, 4);
    CU_ASSERT_EQUAL(datas_threads->input_graph->nberEdges, 5);

    edge_t expected_edges[] = {
        {0, 1, 2},
        {1, 2, 3},
        {2, 3, -1},
        {2, 0, 5},
        {3, 0, 4}
    };

    for (uint8_t i = 0; i < 5; i++) {
        CU_ASSERT_EQUAL(datas_threads->input_graph->edges[i].from, expected_edges[i].from);
        CU_ASSERT_EQUAL(datas_threads->input_graph->edges[i].to, expected_edges[i].to);
        CU_ASSERT_EQUAL(datas_threads->input_graph->edges[i].cost, expected_edges[i].cost);
    }

    if (pthread_mutex_destroy(datas_threads->mutex) != 0) {
        free(datas_threads->mutex);
        free_graph_t(datas_threads->input_graph);
        free(datas_threads);
        printMethodFailed("pthread_mutex_destroy", errno);
        exit(EXIT_FAILURE);
    }
    
    // Note: Le fichier d'entrée a déjà été fermé par la fonction read_entry_file.
    free(datas_threads->mutex);
    free_graph_t(datas_threads->input_graph);
    free(datas_threads);
}


/*
* Teste la fonction writeNberNodes_OutputFile.
* Ecrit le nombre de noeuds dans un fichier temporaire.
* Vérifie que le nombre de noeuds a bien été écrit dans le fichier.
*/
void writeNberNodes_OutputFile_Test(void) {

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMemoryAllocationError("datas_threads", errno);
        exit(EXIT_FAILURE);
    }
    datas_threads->verbose = false;
    datas_threads->fp_output = tmpfile();

    if (initializeInputGraph(datas_threads, 10, 3) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    writeNberNodes_OutputFile(datas_threads);

    // On se replace au début du fichier.
    rewind(datas_threads->fp_output);

    uint32_t nberNodes;
    if (fread(&nberNodes, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    CU_ASSERT_EQUAL(nberNodes, htobe32(10));
    CU_ASSERT_EQUAL(be32toh(nberNodes), 10);

    if (closeFile(datas_threads->fp_output, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    free_graph_t(datas_threads->input_graph);
    free(datas_threads);
}


/*
* Teste la fonction writeData_OutputFile.
* Ecrit les données de distance et de prédécesseur dans un fichier temporaire.
* Vérifie que les données ont bien été écrites dans le fichier.
*/
void writeData_OutputFile_Test(void) {

    uint32_t nberNodes = 4;
    uint32_t source = 1;

    FILE *fp = tmpfile();

    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMemoryAllocationError("datas_threads", errno);
        exit(EXIT_FAILURE);
    }
    datas_threads->fp_output = fp;
    datas_threads->verbose = false;

    int64_t *distance = (int64_t *) malloc(4 * sizeof(int64_t));
    if (distance == NULL) {
        free(datas_threads);
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }
    memcpy(distance, (int64_t[]) {INFINITY_VALUE, 100, -22, 120}, sizeof(int64_t) * nberNodes);

    int32_t *predecessor = (int32_t *) malloc(4 * sizeof(int32_t));
    if (predecessor == NULL) {
        free(distance);
        free(datas_threads);
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }
    memcpy(predecessor, (int32_t[]) {2, 0, 1, 2}, sizeof(int32_t) * nberNodes);

    outputGraph_t *outputGraph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (outputGraph == NULL) {
        free(distance);
        free(predecessor);
        free(datas_threads);
        printMemoryAllocationError("outputGraph", errno);
        exit(EXIT_FAILURE);
    }
    outputGraph->nberNodes = nberNodes;

    createOutputGraph(outputGraph, source, distance, predecessor);
    
    writeData_OutputFile(datas_threads, outputGraph, distance);

    // On se replace au début du fichier.
    rewind(datas_threads->fp_output);
    
    // Vérification des données écrites dans le fichier.
    uint32_t sourceNode;
    uint32_t destinationNode;
    int64_t totalCost;
    uint32_t maxLength;
    uint32_t pathList_Node1;
    uint32_t pathList_Node2;
    uint32_t pathList_Node3;

    if (fread(&sourceNode, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&destinationNode, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&totalCost, sizeof(int64_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&maxLength, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&pathList_Node1, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&pathList_Node2, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    if (fread(&pathList_Node3, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
        free(distance);
        free(datas_threads);
        free(predecessor);
        free_outputGraph_t(outputGraph);
        printMethodFailed("fread", errno);
        exit(EXIT_FAILURE);
    }

    fclose(datas_threads->fp_output);

    CU_ASSERT_EQUAL(sourceNode, htobe32(1));
    CU_ASSERT_EQUAL(destinationNode, htobe32(3));
    CU_ASSERT_EQUAL(totalCost, htobe64(120));
    CU_ASSERT_EQUAL(maxLength, htobe32(3));
    CU_ASSERT_EQUAL(pathList_Node1, htobe32(1));
    CU_ASSERT_EQUAL(pathList_Node2, htobe32(2));
    CU_ASSERT_EQUAL(pathList_Node3, htobe32(3));

    free(distance);
    free(predecessor);
    free_outputGraph_t(outputGraph);
    free(datas_threads);
}


/*
* Fonction principale lançant les tests du module files.c.
* Cette fonction est appelée par le main du programme test.
*/
int main_file_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test files.c", init_suite_files, clean_suite_files);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "read_entry_file_Test", read_entry_file_Test)) ||
        (NULL == CU_add_test(pSuite, "writeNberNodes_OutputFile_Test", writeNberNodes_OutputFile_Test)) ||
        (NULL == CU_add_test(pSuite, "writeData_OutputFile_Test", writeData_OutputFile_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return CUE_SUCCESS;
}