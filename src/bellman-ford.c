#include "../include/bellman-ford.h"


/*
* Initialise les pointeurs d'entiers 'distance' (INFINITY et 0 pour le noeud source) et 'predecessor' (-1).
*
* @params nberNode: nombre entier non signé (sur 32bits) représentant le nombre de noeuds du graphe d'entrée.
* @params source : nombre entier non signé (sur 32bits) représentant le noeud source.
* @params distance : pointeur d'entiers signés (sur 32bits) représentant le coût entre le noeud source
*                    et le noeud i tel que i est l'index du tableau.
* @params predecessor : pointeur d'entiers signés (sur 32bits) représentant le prédecesseur du noeud i tel que i est l'index du tableau.
*
* @return: void.
*/
void initializeAll(uint32_t nberNode, uint32_t source, int64_t *distance, int32_t *predecessor) {

    // Initialise les distances et les prédecesseurs.
    for (uint32_t i = 0; i < nberNode; i++) {
        distance[i] = INFINITY_VALUE;
        predecessor[i] = -1;
    }
    distance[source] = 0;
}


/*
* Détecte si le graphe possède un cycle négatif.
*
* @params input_graph: pointeur vers la structure 'graph_t' représentant le graphe d'entrée.
* @params distance : pointeur d'entiers signés (sur 32bits) représentant le coût entre le noeud source
*                    et le noeud i tel que i est l'index du tableau.
*
* @return: booléen => true si un cycle négatif a été détecté, false sinon.
*/
bool checkNegCycle(graph_t *input_graph, int64_t *distance) {

    // Déclaration des variables pour éviter de les
    // déclarer à chaque itération de la boucle.
    uint32_t nodeFrom;
    uint32_t nodeTo;
    int32_t cost;

    for (uint32_t i = 0; i < input_graph->nberEdges; i++) {
            
        nodeFrom = input_graph->edges[i].from;
        nodeTo = input_graph->edges[i].to;
        cost = input_graph->edges[i].cost;

        // Si un nouveau chemin plus court est encore trouvé, alors il y a un cycle négatif.
        if ((distance[nodeFrom] != INFINITY_VALUE) && (distance[nodeFrom] + cost < distance[nodeTo])) {
            return true;
        }
    }
    return false;
}



/*
* Applique l'algorithme de Bellman ford au graphe d'entrée 'datas_threads->input_graph'.
*
* @params datas_threads: pointeur vers la structure 'datas_threads_t' contenant le graphe d'entrée et le mode verbose.
* @params source: nombre entier non signé (sur 32bits) représentant le noeud source.
* @params distance: pointeur d'entiers signés (sur 32bits) représentant le coût entre le noeud source et le noeud i tel que i est l'index du tableau.
* @params predecessor: pointeur d'entiers signés (sur 32bits) représentant le prédecesseur du noeud i tel que i est l'index du tableau.
* @params info_verbose: pointeur vers une chaîne de caractères représentant les informations (précédentes) à afficher sur la console stdout.
*
* @return: void.
*/
void bellman_ford(datas_threads_t *datas_threads, uint32_t source, int64_t *distance, int32_t *predecessor, char *info_verbose) {

    graph_t *input_graph = datas_threads->input_graph;

    // Initialise les distances et les prédecesseurs.
    initializeAll(input_graph->nberNodes, source, distance, predecessor);

    // Complète 'info_verbose' avec les informations de début d'initialisation.
    if (datas_threads->verbose) {
        initialisationSucceed(info_verbose, source);
    }

    // On déclare déjà les variables en dehors de la boucle pour
    // éviter de les déclarer à chaque itération de la boucle.
    uint32_t nodeFrom;
    uint32_t nodeTo;
    int32_t cost;

    // Permet de sortir de la boucle si aucun nouveau chemin plus
    // court n'est trouvé lors d'une itération complète.
    bool breaker;
    
    // Applique la boucle principale de l'algorithme de Bellman ford.
    for (uint32_t i = 0; i < input_graph->nberNodes - 1; i++) {

        // On initialise la variable tel qu'aucun nouveau chemin
        // plus court n'est trouvé lors de l'itération complète.
        breaker = true;
        for (uint32_t j = 0; j < input_graph->nberEdges; j++) {

            nodeFrom = input_graph->edges[j].from;
            nodeTo = input_graph->edges[j].to;
            cost = input_graph->edges[j].cost;

            // Si un nouveau chemin plus court est trouvé, alors on met à jour les distances et les prédecesseurs.
            if ((distance[nodeFrom] != INFINITY_VALUE) && (distance[nodeFrom] + cost < distance[nodeTo])) {
                distance[nodeTo] = distance[nodeFrom] + cost;
                predecessor[nodeTo] = nodeFrom;
                breaker = false;
            }
        }

        // Si aucun nouveau chemin plus court n'est trouvé lors de
        // l'itération complète, alors on sort de la boucle.
        if (breaker == true) {
            break;
        }
    }

    // Si un nouveau chemin plus court est encore trouvé, alors il y a un cycle négatif.
    // Si l'algorithme est sortit de la boucle avec l'instruction 'break', alors il n'y a d'office pas de cycle négatif.
    if (breaker == false) {
        if (checkNegCycle(datas_threads->input_graph, distance)) {
            // Reinitialise les distances et les prédecesseurs car cycle négatif trouvé.
            initializeAll(input_graph->nberNodes, source, distance, predecessor);
            
            // Complète 'info_verbose' avec les informations de détection de cycle négatif.
            if (datas_threads->verbose) {
                NegCycleDetected(info_verbose, nodeFrom, nodeTo, cost, distance);
            }
        } else {
            breaker = false;
        }
    }
    // Complète 'info_verbose' avec les informations de détection de cycle négatif.
    if ((breaker == false) && (datas_threads->verbose)) {
        NegCycleNotDetected(info_verbose);
    }
}


/*
* Fonction auxiliaire utilisé pour 'getPath'.
% Permet de compléter le tableau 'pathListAux' avec les noeuds présents dans le 'predecessor'.
% Complèete également la longueur totale du chemin le plus court dans la structure 'outputGraph_t'.
*
* @params pathListAux: pointeur non complété d'entiers non signés (sur 32bits) représentant le chemin le plus court inersé.
* @params source: nombre entier non signé (sur 32bits) représentant le noeud source.
* @params destination: entiers non signés (sur 32bits) représentant le noeud destination.
* @params predecessor: pointeur d'entiers signés (sur 32bits) représentant le prédecesseur du noeud i tel que i est l'index du tableau.
*
* @return: void.
*/
void completePathList(outputGraph_t *output_graph, uint32_t *pathListAux, uint32_t source, int32_t *predecessor) {

    uint32_t current = output_graph->destinationNode;
    uint32_t idx = 0;

    // Rempli le tableau 'pathListAux' avec les noeuds présent dans le 'predecessor'.    
    while (current != source) {
        pathListAux[idx] = current;
        current = predecessor[current];
        idx++;
    }
    // Ajoute le noeud source au tableau 'pathListAux'.
    pathListAux[idx] = source;

    // Initialise la longueur totale du chemin le plus court.
    output_graph->pathLength =  idx + 1;
}


/*
* Fonction auxiliaire utilisé pour 'getPath'.
* Permet de retourner les éléments du tableau 'pathList' afin d'obtenir le chemin du noeud source au noeud destination et non l'inverse.
* Complète donc le tableau 'pathList'.
*
* @params idx: nombre entier signé (sur 32bits) représentant le dernier index du tableau 'pathListAux'.
* @params pathListAux: pointeur de nombre entier non signé (sur 32bits) représentant le chemin le plu court inversé.
* @params pathList: pointeur (non complété) d'entiers signés (sur 32bits) représentant le chemin le plus court.
*
* @return: void.
*/
void reversePathList(outputGraph_t *output_graph, uint32_t *pathListAux) {

    // Boucle do while car PathListAux contient au minimum un élément.
    uint32_t i = 0;
    int32_t idx = output_graph->pathLength - 1;
    do {
        output_graph->pathList[i] = pathListAux[idx];
        i++;
        idx--;
    } while (idx >= 0);
}


/*
* Permet de retracer le chemin du plus court chemin entre un noeud source et un noeud destination du graphe.
* Complète les attributs PathList et PathLength.
* Renvoie une erreur si les données n'ont pas pu être écrites correctement.
*
* @params source: nombre entier non signé (sur 32bits) représentant le noeud source.
* @params output_graph: le pointeur vers la structure 'output_graph_t' contenant les données à écrire.
* @params predecessor: pointeur d'entiers signés (sur 32bits) représentant le prédecesseur du noeud i tel que i est l'index du tableau.
*
* @return: EXIT_SUCCESS si les données ont pu être écrites, EXIT_FAILURE sinon.
*/
uint8_t getPath(uint32_t source, outputGraph_t *output_graph, int32_t *predecessor) {

    uint32_t lengthPath = 1;

    // Si le noeud destination n'est pas atteignable.
    if (predecessor[output_graph->destinationNode] == -1) {

        // Initialise les attributs de 'output_graph'.
        output_graph->totalCost = 0;
        output_graph->pathLength = lengthPath;
        output_graph->pathList = (uint32_t *) malloc(sizeof(uint32_t));
        if (output_graph->pathList == NULL) {
            printMemoryAllocationError("outputGraph->pathList", errno);
            return EXIT_FAILURE;
        }
        output_graph->pathList[0] = source;
        return EXIT_SUCCESS;
    }

    // Initialise le tableau 'pathListAux' (tableau auxilliaire pour compléter 'pathList').
    // Alloue la mémoire maximale possible pour le tableau 'pathListAux'.
    uint32_t *pathListAux = (uint32_t *) malloc(sizeof(uint32_t) * output_graph->nberNodes);
    if (pathListAux == NULL) {
        printMemoryAllocationError("pathListAux", errno);
        return EXIT_FAILURE;
    }

    // Complète le tableau 'pathListAux' avec les noeuds présent dans le 'predecessor'.
    completePathList(output_graph, pathListAux, source, predecessor);

    output_graph->pathList = (uint32_t *) malloc(output_graph->pathLength * sizeof(uint32_t));
    if (output_graph->pathList == NULL) {
        printMemoryAllocationError("output_graph->pathList", errno);
        return EXIT_FAILURE;
    }

    // Retourne le tableau 'pathList' dans le bon ordre afin d'obtenir le chemin du noeud source au noeud destination.
    reversePathList(output_graph, pathListAux);

    // Libère la mémoire allouée au tableau 'pathListAux' et renvoie la longueur du chemin le plus court.
    free(pathListAux);

    return EXIT_SUCCESS;
}