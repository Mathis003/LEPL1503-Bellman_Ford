#include "../include/bellman_test.h"


/*
* Initialise la suite de tests.
*/
int init_suite_bellman(void) {
    return EXIT_SUCCESS;
}


/*
* Clean la suite de tests.
*/
int clean_suite_bellman(void) {
    return EXIT_SUCCESS;
}


/*
* Teste la fonction d'initialisation du graphe qui va être analysé par la suite (initializeAll). 
* Le tableau "predecessor" ne doit contenir que des "-1". 
* Le tableau "distance" ne doit contenir que des "Infinty" sauf à l'index du noeud source, où la valeur doit être 0.
*/
void initializeAll_Test(void) {
    
    uint32_t nberNodes = 100;
    uint32_t source = 7;

    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }
    
    int32_t *predecessor = (int32_t *) malloc(nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }

    // Fonction à tester.
    initializeAll(nberNodes, source, distance, predecessor);

    for (uint32_t i = 0; i < nberNodes; i++) {
        CU_ASSERT(predecessor[i] == -1);

        if (i != source) {
            CU_ASSERT(distance[i] == INFINITY_VALUE);
        } else {
            CU_ASSERT(distance[i] == 0);
        }
    }

    free(distance);
    free(predecessor);
}


/*
* Teste la fonction chargée de repérer les cycles négatifs (checkNegCycle)
* avec un graphe qui n'a pas de cycle négatif.
* Ici, il est vérifié que la fonction renvoie bien "false".
*/
void checkNegCycle_False_Test(void) {

    uint32_t nberNodes = 4;
    uint32_t nberEdges = 5;
    uint32_t source = 0;

    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }
    
    int32_t *predecessor = (int32_t *) malloc(nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }

    initializeAll(nberNodes, source, distance, predecessor);

    datas_threads_t *datas = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas == NULL) {
        printMemoryAllocationError("datas", errno);
        exit(EXIT_FAILURE);
    }
    datas->verbose = false;
    
    if (initializeInputGraph(datas, nberNodes, nberEdges) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    datas->input_graph->edges[0] = (edge_t) {0, 1, -1};
    datas->input_graph->edges[1] = (edge_t) {0, 2, 4};
    datas->input_graph->edges[2] = (edge_t) {1, 2, 2};
    datas->input_graph->edges[3] = (edge_t) {2, 3, 3};
    datas->input_graph->edges[4] = (edge_t) {3, 1, 1};

    uint32_t nodeFrom;
    uint32_t nodeTo;
    int32_t cost;

    for (uint8_t i = 0; i < datas->input_graph->nberNodes - 1; i++) {
        for (uint8_t j = 0; j < datas->input_graph->nberEdges; j++) {

            nodeFrom = datas->input_graph->edges[j].from;
            nodeTo = datas->input_graph->edges[j].to;
            cost = datas->input_graph->edges[j].cost;

            if ((distance[nodeFrom] != INFINITY_VALUE) && (distance[nodeFrom] + cost < distance[nodeTo])) {
                distance[nodeTo] = distance[nodeFrom] + cost;
                predecessor[nodeTo] = nodeFrom;
            }
        }
    }
    
    CU_ASSERT_FALSE(checkNegCycle(datas->input_graph, distance));

    free(distance);
    free(predecessor);
    free_graph_t(datas->input_graph);
    free(datas);
}


/*
* Teste la fonction chargée de repérer les cycles négatifs (checkNegCycle)
* avec un graphe qui possède un cycle négatif.
* Ici, il est vérifié que la fonction renvoie bien "true".
*/
void checkNegCycle_True_Test(void) {

    uint32_t nberNodes = 4;
    uint32_t nberEdges = 5;
    uint32_t source = 0;

    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }

    int32_t *predecessor = (int32_t *) malloc(nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }

    initializeAll(nberNodes, source, distance, predecessor);

    datas_threads_t *datas = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas == NULL) {
        printMemoryAllocationError("datas", errno);
        exit(EXIT_FAILURE);
    }
    datas->verbose = false;
    
    if (initializeInputGraph(datas, nberNodes, nberEdges) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    datas->input_graph->edges[0] = (edge_t) {0, 1, -7};
    datas->input_graph->edges[1] = (edge_t) {2, 0, 4};
    datas->input_graph->edges[2] = (edge_t) {1, 2, 2};
    datas->input_graph->edges[3] = (edge_t) {2, 3, 3};
    datas->input_graph->edges[4] = (edge_t) {3, 1, 1};

    uint32_t nodeFrom;
    uint32_t nodeTo;
    int32_t cost;

    for (uint8_t i = 0; i < datas->input_graph->nberNodes - 1; i++) {
        for (uint8_t j = 0; j < datas->input_graph->nberEdges; j++) {

            nodeFrom = datas->input_graph->edges[j].from;
            nodeTo = datas->input_graph->edges[j].to;
            cost = datas->input_graph->edges[j].cost;

            if ((distance[nodeFrom] != INFINITY_VALUE) && (distance[nodeFrom] + cost < distance[nodeTo])) {
                distance[nodeTo] = distance[nodeFrom] + cost;
                predecessor[nodeTo] = nodeFrom;
            }
        }
    }    

    CU_ASSERT_TRUE(checkNegCycle(datas->input_graph, distance));

    free(distance);
    free(predecessor);
    free_graph_t(datas->input_graph);
    free(datas);
}


/*
* Teste la fonction qui calcule l'algorithme de Bellman Ford.
* Ici, il est vérifié les valeurs que prennent les tableaux "distance" et "predecessor".
*/
void bellman_ford_Test(void) {

    uint32_t source = 2;
    uint32_t nberNodes = 4;
    uint32_t nberEdges = 5;

    datas_threads_t *datas = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas == NULL) {
        printMemoryAllocationError("datas", errno);
        exit(EXIT_FAILURE);
    }
    datas->verbose = false;
    
    if (initializeInputGraph(datas, nberNodes, nberEdges) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }

    int32_t *predecessor = (int32_t *) malloc(nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }

    initializeAll(nberNodes, source, distance, predecessor);

    datas->input_graph->edges[0] = (edge_t) {0, 1, 2};
    datas->input_graph->edges[1] = (edge_t) {1, 2, 3};
    datas->input_graph->edges[2] = (edge_t) {2, 3, -1};
    datas->input_graph->edges[3] = (edge_t) {3, 0, 4};
    datas->input_graph->edges[4] = (edge_t) {2, 0, 5};

    bellman_ford(datas, source, distance, predecessor, "bellman_ford_test");
    
    CU_ASSERT_EQUAL(distance[0], 3);
    CU_ASSERT_EQUAL(distance[1], 5);
    CU_ASSERT_EQUAL(distance[2], 0);
    CU_ASSERT_EQUAL(distance[3], -1);
    CU_ASSERT_EQUAL(predecessor[1], 0);
    CU_ASSERT_EQUAL(predecessor[0], 3);
    CU_ASSERT_EQUAL(predecessor[3], 2);

    free(distance);
    free(predecessor);
    free_graph_t(datas->input_graph);
    free(datas);
}


/*
* Teste la fonction qui complète le tableau des chemins (completePathList).
* Ici, il est vérifié les valeurs que prennent les tableaux "pathList" et "lengthPath".
*/
void completePathList_Test(void) {

    outputGraph_t *output_graph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (output_graph == NULL) {
        printMemoryAllocationError("output_graph", errno);
        exit(EXIT_FAILURE);
    }

    int32_t *predecessor = (int32_t *) malloc(5 * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }
    memcpy(predecessor,  (int32_t[]) { -1, 0, 1, 2, 1 }, 5 * sizeof(int32_t));

    uint32_t *pathListAux = (uint32_t *) malloc(5 * sizeof(uint32_t));
    if (pathListAux == NULL) {
        printMemoryAllocationError("pathListAux", errno);
        exit(EXIT_FAILURE);
    }

    // Il ne peut pas y avoir de cas où le chemin est de longueur 0 car getPath() s'en occupe au préalable.

    // Cas où le chemin est de longueur 3
    output_graph->destinationNode = 2;
    completePathList(output_graph, pathListAux, 0, predecessor);

    CU_ASSERT_EQUAL(output_graph->pathLength, 3);
    CU_ASSERT_EQUAL(pathListAux[0], 2);
    CU_ASSERT_EQUAL(pathListAux[1], 1);
    CU_ASSERT_EQUAL(pathListAux[2], 0);

    // Cas où le chemin est de longueur 4
    output_graph->destinationNode = 3;
    completePathList(output_graph, pathListAux, 0, predecessor);

    CU_ASSERT_EQUAL(output_graph->pathLength, 4);
    CU_ASSERT_EQUAL(pathListAux[0], 3);
    CU_ASSERT_EQUAL(pathListAux[1], 2);
    CU_ASSERT_EQUAL(pathListAux[2], 1);
    CU_ASSERT_EQUAL(pathListAux[3], 0);

    free(pathListAux);
    free(predecessor);
    free(output_graph);
}


/*
* Teste la fonction qui inverse le tableau des chemins (reversePathList).
* Ici, il est vérifié les valeurs que prennent les tableaux "output_graph->pathList"
*/
void reversePathList_Test(void) {

    outputGraph_t *output_graph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (output_graph == NULL) {
        printMemoryAllocationError("output_graph", errno);
        exit(EXIT_FAILURE);
    }

    output_graph->pathList = (uint32_t *) malloc(5 * sizeof(uint32_t));
    if (output_graph->pathList == NULL) {
        printMemoryAllocationError("output_graph->pathList", errno);
        exit(EXIT_FAILURE);
    }
    output_graph->pathLength = 5;

    uint32_t *pathListAux = (uint32_t *) malloc(5 * sizeof(uint32_t));
    if (pathListAux == NULL) {
        printMemoryAllocationError("pathListAux", errno);
        exit(EXIT_FAILURE);
    }
    memcpy(pathListAux, (uint32_t[]) {0, 1, 2, 3, 4}, 5 * sizeof(uint32_t));

    reversePathList(output_graph, pathListAux);

    uint32_t expected_value[] = {4, 3, 2, 1, 0};
    for (int i = 0; i < 5; i++) {
        CU_ASSERT_EQUAL(output_graph->pathList[i], expected_value[i]);
    }

    free(pathListAux);
    free(output_graph->pathList);

    output_graph->pathList = (uint32_t *) malloc(10 * sizeof(uint32_t));
    if (output_graph->pathList == NULL) {
        printMemoryAllocationError("output_graph->pathList", errno);
        exit(EXIT_FAILURE);
    }
    output_graph->pathLength = 10;

    uint32_t *big_pathListAux = (uint32_t *) malloc(10 * sizeof(uint32_t));
    if (big_pathListAux == NULL) {
        printMemoryAllocationError("big_pathListAux", errno);
        exit(EXIT_FAILURE);
    }
    memcpy(big_pathListAux, (uint32_t[]) {-1, 10, -90902, 3, 4, 9, 19, -9, 0, 2001}, 10 * sizeof(uint32_t));

    reversePathList(output_graph, big_pathListAux);

    uint32_t expected_value_2[] = {2001, 0, -9, 19, 9, 4, 3, -90902, 10, -1};
    for (int i = 0; i < 10; i++) {
        CU_ASSERT_EQUAL(output_graph->pathList[i], expected_value_2[i]);
    }

    free(big_pathListAux);
    free(output_graph->pathList);
    free(output_graph);
}


/*
* Teste la fonction qui renvoie le plus court chemin entre deux noeuds en donnant les noeuds de passage dans l'ordre. 
* Pour cela, la fonction est testée 3 fois.
*/
void getPath_Test(void) {

    uint32_t nberNodes = 4;
    uint32_t nberEdges = 5;

    datas_threads_t *datas = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas == NULL) {
        printMemoryAllocationError("datas", errno);
        exit(EXIT_FAILURE);
    }
    datas->verbose = false;
    
    if (initializeInputGraph(datas, nberNodes, nberEdges) == EXIT_FAILURE) {
        printMethodFailed("initializeInputGraph", errno);
        exit(EXIT_FAILURE);
    }

    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        exit(EXIT_FAILURE);
    }

    int32_t *predecessor = (int32_t *) malloc(nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        exit(EXIT_FAILURE);
    }

    datas->input_graph->edges[0] = (edge_t) {0, 1, 2};
    datas->input_graph->edges[1] = (edge_t) {1, 2, 3};
    datas->input_graph->edges[2] = (edge_t) {2, 3, -1};
    datas->input_graph->edges[3] = (edge_t) {3, 0, 4};
    datas->input_graph->edges[4] = (edge_t) {2, 0, 5};

    uint32_t pathList[][4] = {
        {0, 1, 2},
        {1, 2, 3, 0},
        {2, 3, 0, 1},
        {3, 0, 1, 2}
    };

    outputGraph_t *outputGraph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (outputGraph == NULL) {
        printMemoryAllocationError("outputGraph", errno);
        exit(EXIT_FAILURE);
    }
    outputGraph->nberNodes = nberNodes;

    uint32_t lengthPath[] = {3, 4, 4, 4};
    uint32_t startNodes[] = {0, 1, 2, 3};
    uint32_t endNodes[] = {2, 0, 1, 2};
    uint32_t result;

    for (int i = 0; i < datas->input_graph->nberNodes; i++) {

        initializeAll(nberNodes, startNodes[i], distance, predecessor);

        bellman_ford(datas, startNodes[i], distance, predecessor, "getPathTest");

        outputGraph->destinationNode = endNodes[i];

        result = getPath(startNodes[i], outputGraph, predecessor);

        CU_ASSERT_EQUAL(result, EXIT_SUCCESS);
        CU_ASSERT_EQUAL(outputGraph->pathLength, lengthPath[i]);

        for (int j = 0; j < outputGraph->pathLength; j++) {
            CU_ASSERT_EQUAL(outputGraph->pathList[j], pathList[i][j]);
        }

        free(outputGraph->pathList);
    }

    free(distance);
    free(predecessor);
    free_graph_t(datas->input_graph);
    free(datas);
    free(outputGraph);
}


/*
* Fonction principale des tests du module bellman-ford.c.
* Elle lance tous les tests.
* Cette fonction est appelée dans le main du programme test.
*/
int main_bellman_test() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test bellman-ford.c", init_suite_bellman, clean_suite_bellman);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */
    if ((NULL == CU_add_test(pSuite, "initializeAll_Test", initializeAll_Test)) ||
        (NULL == CU_add_test(pSuite, "checkNegCycle_False_Test", checkNegCycle_False_Test)) ||
        (NULL == CU_add_test(pSuite, "checkNegCycle_True_Test", checkNegCycle_True_Test)) ||
        (NULL == CU_add_test(pSuite, "bellman_ford_Test", bellman_ford_Test)) ||
        (NULL == CU_add_test(pSuite, "completePathList_Test", completePathList_Test)) ||
        (NULL == CU_add_test(pSuite, "reversePathList_Test", reversePathList_Test)) ||
        (NULL == CU_add_test(pSuite, "getPath_Test", getPath_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}