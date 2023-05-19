#include "../include/threads.h"


/*
* Attend la fin de tous les threads.
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' contenant les données communes aux threads.
* @param threads: pointeur vers une structure 'pthread_t' représentant un thread ( = tableau de threads à attendre).
*
* @return: nombre entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t wait_threads(pthread_t *threads, datas_threads_t *datas_threads) {

    void *result_ptr;
    // Boucle sur tous les threads.
    for (uint8_t i = 0; i < datas_threads->nb_threads; i++) {
        
        // Attend la fin du thread.
        if (pthread_join(threads[i], &result_ptr) != 0) {
            printThreadError(i);
            return EXIT_FAILURE;
        }
        
        // Si il y a eu une erreur dans function_thread_algorithm().
        if (result_ptr == NULL) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


/*
* Lances les threads pour effetcuer la phase algorithmique du programme.
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' contenant les données communes aux threads.
* @param threads: pointeur vers une structure 'pthread_t' représentant un thread ( = tableau de threads).
*
* @return: nombre entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t launch_algorithm_threads(pthread_t *threads, datas_threads_t *datas_threads) {

    // Utile pour équilibrer le travail entre les threads.
    uint32_t basic_nber_iteration = datas_threads->input_graph->nberNodes / datas_threads->nb_threads;
    uint32_t rest_nber_iteration = datas_threads->input_graph->nberNodes % datas_threads->nb_threads;

    // Utile pour savoir le nombre d'itération de chaque thread et leur(s) sourceNode(s) associé(s).
    uint32_t current_nber_iteration = basic_nber_iteration + 1;
    uint32_t lastNode = 0;
    
    for (uint8_t i = 0; i < datas_threads->nb_threads; i++) {

        // Pour équilibrer de la meilleure des manières le travail entre les threads.
        if (rest_nber_iteration > 0) {
            rest_nber_iteration--;
        } else {
            current_nber_iteration = basic_nber_iteration;
        }

        // Crée la structure 'datas_threads_algorithm_t' contenant les arguments du thread.
        datas_threads_algorithm_t *datas_threads_algo = (datas_threads_algorithm_t *) malloc(sizeof(datas_threads_algorithm_t));
        if (datas_threads_algo == NULL) {
            printMemoryAllocationError("datas_threads_algo", errno);
            return EXIT_FAILURE;
        }
        initialize_Data_algorithm_threads(datas_threads, datas_threads_algo, lastNode, current_nber_iteration);

        // Lance le thread n°i.
        if (pthread_create(&threads[i], NULL, function_thread_algorithm, datas_threads_algo) != 0) {
            printMethodFailed("pthread_create", errno);
            free(datas_threads_algo);
            return EXIT_FAILURE;
        }

        // Met à jour le nombre d'itérations déjà effectuées par les
        // threads précédents pour les prochains threads.
        lastNode += current_nber_iteration;
    }

    return EXIT_SUCCESS;
}


/*
* Fonction exécutée par chaque thread pour effectuer la phase algorithmique du programme.
*
* @param args_struct: pointeur vers la structure 'data_thread_algorithm_t' (casté en void *) contenant les arguments du thread.
*
* @return: void.
*/
void *function_thread_algorithm(void *args_struct) {
    
    // Cast les arguments du thread en 'data_thread_algorithm_t'.
    datas_threads_algorithm_t *datas_threads_algo = (datas_threads_algorithm_t *) args_struct;

    datas_threads_t *datas_threads = datas_threads_algo->datas_threads;
    
    // Alloue de la mémoire pour la structure 'outputGraph_t' représentant un bloc du graphe de sortie.
    outputGraph_t *output_graph = (outputGraph_t *) malloc(sizeof(outputGraph_t));
    if (output_graph == NULL) {
        printMemoryAllocationError("outputGraph", errno);
        free(datas_threads_algo);
        return NULL;
    }
    output_graph->nberNodes = datas_threads->input_graph->nberNodes;

     // Alloue de la mémoire pour les différents tableaux (= pointeurs).
    int64_t *distance = (int64_t *) malloc(output_graph->nberNodes * sizeof(int64_t));
    if (distance == NULL) {
        printMemoryAllocationError("distance", errno);
        free_outputGraph_t(output_graph);
        free(datas_threads_algo);
        return NULL;
    }

    int32_t *predecessor = (int32_t *) malloc(output_graph->nberNodes * sizeof(int32_t));
    if (predecessor == NULL) {
        printMemoryAllocationError("predecessor", errno);
        free_outputGraph_t(output_graph);
        free(distance);
        free(datas_threads_algo);
        return NULL;
    }

    char *info_verbose = (char *) calloc(1000, sizeof(char));
    if (info_verbose == NULL) {
        printMemoryAllocationError("info_verbose", errno);
        free_outputGraph_t(output_graph);
        free(distance);
        free(predecessor);
        free(datas_threads_algo);
        return NULL;
    }

    // Début et fin de l'intervalle de noeuds à traiter par le thread.
    uint32_t current_source = datas_threads_algo->lastNode;
    uint32_t last_source = current_source + datas_threads_algo->nber_iteration;

    while (current_source < last_source) {

        // Permet d'update les tableaux distance, predecessor et info_verbose.
        bellman_ford(datas_threads, current_source, distance, predecessor, info_verbose);        

        // Permet de créer le graphe de sortie pour stocker les résultats.
        if (createOutputGraph(output_graph, current_source, distance, predecessor) == EXIT_FAILURE) {
            free_datas_threads_algorithm_t(predecessor, distance, output_graph, info_verbose, datas_threads_algo);
            return NULL;
        }

        // *** Commencement de la section critique. *** //

        // Verrouille l'accès à un seul thread => permet de sécuriser le processus.
        if (pthread_mutex_lock(datas_threads->mutex) != 0) {
            printMethodFailed("pthread_mutex_lock", errno);
            free_datas_threads_algorithm_t(predecessor, distance, output_graph, info_verbose, datas_threads_algo);
            return NULL;
        }

        if (datas_threads->verbose) {
            printf("%s", info_verbose);
        }

        // Ecrit les résultats dans le fichier de sortie.
        if (writeData_OutputFile(datas_threads, output_graph, distance) == EXIT_FAILURE) {
            free_datas_threads_algorithm_t(predecessor, distance, output_graph, info_verbose, datas_threads_algo);
            return NULL;
        }

        // Déverrouille l'accès à un seul thread.
        if (pthread_mutex_unlock(datas_threads->mutex) != 0) {
            printMethodFailed("pthread_mutex_unlock", errno);
            free_datas_threads_algorithm_t(predecessor, distance, output_graph, info_verbose, datas_threads_algo);
            return NULL;
        }

        // *** Fin de la section critique. *** //

        // Reset pour la prochaine itération.
        free(output_graph->pathList);
        memset(info_verbose, 0, 1000 * sizeof(char));

        // Incrémente la valeur pour la condition while().
        current_source++;
    }

    // Libère la mémoire allouée restante.
    free(info_verbose);
    free(predecessor);
    free(distance);
    free(output_graph);
    free(datas_threads_algo);

    // Retourne un pointeur void différent de NULL pour reconnaître s'il y a une erreur.
    return (void *) datas_threads->input_graph;
}