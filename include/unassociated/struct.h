#ifndef LEPL1503__STRUCT__
#define LEPL1503__STRUCT__


#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>


/*
* Structure représentant un lien entre deux noeuds du graphe d'entrée.
*
* @attribut from: nombre entier non signé (sur 32 bits) représentant le noeud de départ.
* @attribut to: nombre entier non signé (sur 32 bits) représentant le noeud d'arrivé.
* @attribut cost: nombre entier signé (sur 32 bits) représentant le coût du lien.
*/
typedef struct {
    uint32_t from;
    uint32_t to;
    int32_t cost;
} edge_t;


/*
* Structure représentant le graphe d'entrée.
*
* @attribut nberNodes: nombre entier non signé (sur 32 bits) représentant le nombre de noeuds du graphe d'entrée.
* @attribut nberEdges: nombre entier non signé (sur 32 bits) représentant le nombre de liens du graphe d'entrée.
* @attribut edges: pointeur vers un tableau de structure 'edge_t' représentant les liens du graphe d'entrée.
*/
typedef struct {
    uint32_t nberNodes;
    uint32_t nberEdges;
    edge_t *edges;
} graph_t;


/*
* Structure stockant les résultats de l'algorithme de Bellman ford, qui seront écrit dans le fichier binaire de sortie.
*
* @attribut nberNodes: nombre entier non signé (sur 32 bits) représentant le nombre de noeuds du graphe d'entrée.
* @attribut sourceNode: pointeur vers un tableau d'entier non signé (sur 32 bits) représentant chaques noeud source (= noeud de départ) du graphe d'entrée.
* @attribut destinationNode: pointeur vers un tableau d'entier non signé (sur 32 bits) représentant chaque noeud d'arrivé (= destination) du graphe d'entrée.
* @attribut totalCost: pointeur vers un tableau d'entier signé (sur 64 bits) représentant le coût total du chemin allant du noeud source au noeud d'arrivée.
* @attribut pathList: pointeur de pointeur vers un tableau d'entier non signé (sur 32 bits) représentant le chemin allant du noeud source au noeud d'arrivée.
* @attribut pathLength: pointeur vers un tableau d'entier non signé (sur 32 bits) représentant la longueur du chemin allant du noeud source au noeud d'arrivée.
*/
typedef struct {
    uint32_t nberNodes;
    uint32_t sourceNode;
    uint32_t destinationNode;
    int64_t totalCost;
    uint32_t pathLength;
    uint32_t *pathList;
} outputGraph_t;


/*
* Structure stockant les arguments de l'utilisateur, le graphe d'entré, ainsi que du mutex global permettant de synchroniser les threads.
* Cette structure est passée en argument à tous les threads. C'est donc la structure commune qui est partagée entre tous les threads.
*
* @attribut input_file: pointeur vers un flux de fichier qui correspond au fichier binaire d'entrée.
* @attribut output_stream: pointeur vers un flux de fichier qui correspond au fichier binaire de sortie.
* @attribut nb_threads: nombre entier non signé (sur 8bits) représentant le nombre de threads de calcul exécutant le programme.
* @attribut verbose: boolean autorisant, ou non, les messages de debug / d'informations,... lors de l'exécution du programme.
*
* @attribut input_graph: pointeur vers la structure 'graph_t' représentant le graphe d'entrée.
* @attribut mutex: pointeur vers un mutex (structure 'pthread_mutex_t') qui permet de sécuriser (synchroniser les threads) les données mises à jour par les threads.
*/
typedef struct {
    // Arguments de l'utilisateur.
    FILE *input_fp;
    FILE *fp_output;
    uint8_t nb_threads;
    bool verbose;

    // Données générales utiles.
    graph_t *input_graph;
    pthread_mutex_t *mutex;
} datas_threads_t;


/*
* Structure stockant les informations nécessaires aux lancements des threads pour effectuer la phase algorithmique du programme.
*
* @attribut datas_threads: pointeur vers la structure stockant les arguments de l'utilisateur, le graphe d'entré, ainsi que du mutex permettant de syncroniser les threads.
* @attribut nber_iteration: nombre entier non signé (sur 32 bits) qui est le nombre d'itérations de l'algorithme à effectuer par le thread.
* @attribut lastNode: nombre entier non signé (sur 32 bits) qui représente le premier noeud source qui sera utilisé par le thread pour l'algorithme.
*                     Le dernier noeud source utilisé par le thread sera lastNode + nber_iteration.
*/
typedef struct {
    datas_threads_t *datas_threads;
    uint32_t lastNode;
    uint32_t nber_iteration;
} datas_threads_algorithm_t;


#endif // LEPL1503__STRUCT__