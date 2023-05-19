#include "include/sp.h"


/*
* Note:
*
* Nous avons essayé une approche multithreading pour le décodage, mais c'était plus lent qu'en mono-thread.
* De plus, nous devions synchroniser la lecture donc le seul moyen était de faire un mapper et de le diviser en N
* mais je ne pense pas que cela aurait été plus rapide pour la plupart des graphes.
*
* Nous avons également essayé de faire une architecture multithreading basée sur le principe consummer/producer,
* mais c'était également moins rapide que le mono-thread car nous perdions un thread pour l'algorithme principal.
* C'était également moins efficace en terme de mémoire. Nous étions obligé de copier chaque structure 'outputGraph_t'
* pour chaque noeud source afin de la stocker dans le buffer. Vu que nous n'étions pas obligé de mettre les résultats
* dans l'ordre des différents blocs, une approche simple était préférable. Cette approche consiste à décomposer le
* travail en N threads de manière la plus équitable possible. Chaque thread va effectuer une partie de l'algorithme
* et va directement mettre ses résultats dans le fichier binaire de sortie de manière totalement indépendante des
* autres threads (excepté la phase critique où les threads sont synchroniser à l'aide d'un mutex).
*/


/*
* fonction principale du programme.
*
* @param argc: nombre entier représentant le nombre d'arguments de la ligne de commande.
* @param argv: un pointeur vers un tableau de chaines de caractères contenant les arguments de la ligne de commande.
*
* @return: un entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
int main(int argc, char *argv[]) {

    // Démarre le chronomètre.
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    // Utile pour savoir si le programme a "échoué" à cause de l'option -h ou non.
    bool help_msg = false;

    // Création de la structure datas_threads_t.
    datas_threads_t *datas_threads = (datas_threads_t *) malloc(sizeof(datas_threads_t));
    if (datas_threads == NULL) {
        printMemoryAllocationError("datas_threads", errno);
        return EXIT_FAILURE;
    }

    if (create_Datas_Threads(datas_threads, argc, argv, &help_msg) == EXIT_FAILURE) {
        if (help_msg) {
            return EXIT_SUCCESS;
        }

        if (datas_threads == NULL) {
            return EXIT_FAILURE;
        }
    }

    // Création du tableau de threads.
    pthread_t threads[datas_threads->nb_threads];

    // Lis le fichier d'entrée et stocke les données dans la structure 'graph_t' contenu dans datas_threads->input_graph.
    // Ferme également le fichier d'entré.
    if (read_entry_file(datas_threads) == EXIT_FAILURE) {
        return EXIT_FAILURE; // Tous les free(), destruction du mutex et fermeture des fichiers sont effectués dans la fonction.
    }

    // Si le nombre de threads est supérieur au nombre de noeuds du graphe.
    if (datas_threads->input_graph->nberNodes < datas_threads->nb_threads) {
        if (datas_threads->input_graph->nberNodes != 0) { // Pour ne pas avoir un nbre de threads égal à 0.
            datas_threads->nb_threads = datas_threads->input_graph->nberNodes; // équivaut à un thread par noeud.
        }
    }
    
    // Ecrit le nombre de noeuds du graphe dans le fichier de sortie.
    if (writeNberNodes_OutputFile(datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Cas où le graphe est vide => Stoppe le programme.
    if (datas_threads->input_graph->nberNodes == 0) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_SUCCESS;
    }

    // Affiche le commencement de la phase algorithmique du programme.
    if (datas_threads->verbose) {
        printAlgorithme_Encodage_Section();
    }

    // Lances les threads pour effectuer l'algorithme de Bellman-Ford
    // et pour stocker les résultats dans le fichier binaire de sortie.
    if (launch_algorithm_threads(threads, datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    // Attends que tous les threads aient fini d'effectuer leur travail.
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

    // Stoppe le chronomètre.
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Si TIMER est ON ou si le mode 'verbose' est activé.
    if (TIMER_ON || datas_threads->verbose) {
        
        // Calcul le temps d'exécution du programme.
        double time_programme = (end_time.tv_sec - start_time.tv_sec);
        time_programme += (end_time.tv_nsec - start_time.tv_nsec) / 1e9;

        // Affiche le temps d'exécution du programme.
        printTimer(time_programme);
    }

    // Libère la mémoire allouée restante.
    free_datas_threads(datas_threads);

    return EXIT_SUCCESS;
}