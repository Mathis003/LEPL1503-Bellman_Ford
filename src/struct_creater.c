#include "../include/struct_creater.h"


/*
* Crée la structure 'graph_t' représentant le graphe d'entrée (sans les liens complétés).
*
* @param datas_threads: pointeur vers la structure 'datas_threads_t' contenant les données communes aux threads.
*
* @return: nombre entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t create_basic_InputGraph(datas_threads_t *datas_threads) {

    // Affiche la section de décodage.
    if (datas_threads->verbose) {
        printDecodingSection();
    }

    uint32_t nberNodes;
    uint32_t nberEdges;

    // Lit les données du fichier d'entrée.
    if (fread(&nberNodes, sizeof(uint32_t), 1, datas_threads->input_fp) != 1) {
        printMethodFailed("fread", errno);
        return EXIT_FAILURE;
    }

    if (fread(&nberEdges, sizeof(uint32_t), 1, datas_threads->input_fp) != 1) {
        printMethodFailed("fread", errno);
        return EXIT_FAILURE;
    }
    
    // Créer la structure 'graph_t'.
    return initializeInputGraph(datas_threads, be32toh(nberNodes), be32toh(nberEdges));
}


/*
* Créé la structure 'outputGraph_t' représentant un bloc du graphe de sortie.
*
* @param outputGraph: pointeur vers la structure 'outputGraph_t' représentant un bloc du graphe de sortie.
* @param source: nombre entier non signé (sur 32 bits) représentant le noeud source.
* @param distance: pointeur d'entiers signés (sur 64 bits) représentant les distances entre le noeud source et les autres noeuds.
* @param predecessor: pointeur d'entiers non signés (sur 32 bits) représentant les prédécesseurs des noeuds.
*
* @return: nombre entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t createOutputGraph(outputGraph_t *outputGraph, uint32_t source, int64_t *distance, int32_t *predecessor) {

    // Initialise les valeurs par défaut.
    outputGraph->sourceNode = source;
    outputGraph->destinationNode = source;
    outputGraph->totalCost = MINUS_INFINITY;

    // On met à jour les données du graphe de sortie si un chemin
    // plus long à été trouvé parmis tous ces chemins les plus courts.
    for (uint32_t j = 0; j < outputGraph->nberNodes; j++) {
        if ((source != j) && (outputGraph->totalCost < distance[j]) && (distance[j] != INFINITY_VALUE)) {

            outputGraph->totalCost = distance[j];
            outputGraph->destinationNode = j;
        }
    }

    // getPath() modifie les attributs 'pathList' et 'pathLength' du bloc du graphe de sortie.
    // Tous les attributs du bloc index=source sont initialisés.
    return getPath(source, outputGraph, predecessor);
}



/*
* Créé la structure totale 'datas_threads_t' contenant les données communes aux threads.
*
* @params datas_threads : pointeur vers la structure 'datas_threads' contenant les données communes des threads.
* @param argc : nombre entier non signé (sur 8 bits) représentant le nombre d'arguments donnés en paramètre par l'utilisateur.
* @param argv : pointeur vers un tableau de caractères représentant (en string) les paramètres.
* @param help_msg : pointeur vers un booléen indiquant si l'utilisateur a demandé l'affichage de l'aide.
*
* @return: un entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t create_Datas_Threads(datas_threads_t *datas_threads, int argc, char *argv[], bool *help_msg) {

    // Création de la première partie de la structure datas_threads_t.
    if (create_Datas_Threads_Args(datas_threads, argc, argv, help_msg) == EXIT_FAILURE) {
        if (*help_msg) {
            return EXIT_SUCCESS;
        } else {
            return EXIT_FAILURE;
        }
    }

    // Création et initialisation du mutex.
    datas_threads->mutex = malloc(sizeof(pthread_mutex_t));
    if ((datas_threads->mutex == NULL) || (pthread_mutex_init(datas_threads->mutex, NULL) != 0)) {
        fprintf(stderr, "Erreur d'initialisation du mutex.\n");
        free(datas_threads);
        return EXIT_FAILURE;
    }

    // Permet d'afficher à l'écran les résultats si la sortie est stdout.
    if (datas_threads->fp_output == stdout) {
        datas_threads->verbose = true;
    }

    return EXIT_SUCCESS;
}


/*
* Créer la première partie de la structure 'datas_threads_t' avec les arguments donnés en paramètre par l'utilisateur lors de l'exécution du programme.
*
* @params data_threads : pointeur vers la structure 'data_threads_t' contenant les données communes aux threads.
* @params argc : nombre entier non signé (sur 8 bits) représentant le nombre d'arguments donnés en paramètre par l'utilisateur.
* @params argv : pointeur vers un tableau de caractères représentant (en string) les paramètres.
* @params help_msg : pointeur vers un booléen indiquant si l'utilisateur a demandé l'affichage de l'aide.
*
* @return un entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t create_Datas_Threads_Args(datas_threads_t *datas_threads, uint8_t argc, char *argv[], bool *help_msg) {

    // Initialisation des valeurs par défaut de args
    initializeDefaultArgs(datas_threads);

    // Mise à jour des valeurs de args si l'utilisateur a spécifié des changements
    // via les arguments optionnels donnés en paramètre lors de l'exécution du programme.
    uint32_t opt;
    optind = 1;
    while ((opt = getopt(argc, argv, "hn:vf:")) != -1) {
        switch (opt) {
            case 'n':
                datas_threads->nb_threads = atoi(optarg);
                if (!(datas_threads->nb_threads > 0 && datas_threads->nb_threads <= INT8_MAX)) {
                    printNberThreadsNeg();   
                    free(datas_threads);
                    return EXIT_FAILURE;
                }
                break;
            case 'v':
                datas_threads->verbose = true;
                break;
            case 'f':
                if ((datas_threads->fp_output = fopen(optarg, "wb")) == 0) {
                    printOpenFileError(errno);
                    free(datas_threads);
                    return EXIT_FAILURE;
                }
                break;
            case '?':
            case 'h':
                usage(argv[0]);  
                *help_msg = true;
                if (datas_threads->fp_output != stdout) {
                    if (closeFile(datas_threads->fp_output, datas_threads->verbose) == EXIT_FAILURE) {
                        free(datas_threads);
                        return EXIT_FAILURE;
                    }
                }
                free(datas_threads);
                return EXIT_FAILURE;
            default:
                usage(argv[0]);
                free(datas_threads);
                return EXIT_FAILURE;
        }
    }

    
    if (optind == argc) {
        printNeedEntryFile();
        free(datas_threads);
        return EXIT_FAILURE;
    }

    // Mise à jour des valeurs de args si l'utilisateur a spécifié
    // un fichier d'entrée (obligatoire sinon erreur).
    if ((datas_threads->input_fp = fopen(argv[optind], "rb")) == 0) {
        printOpenFileError(errno);
        free(datas_threads);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}