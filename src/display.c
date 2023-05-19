#include "../include/display.h"


// Toutes ces fonctions sont utilisées pour afficher des messages d'erreur sur le flux stderr ou stdout.
// Les messages d'erreur sont affichés en français.
// Ces fonctions ne renvoient donc rien.

// Il n'y a pas de documentation pour ces fonctions (excepté une) car elles sont très simples et leur nom est explicite.

// ================================== ERROR : BEGIN ================================== //

void printErrorMutexDestory(void) {
    fprintf(stderr, "Erreur de destruction du mutex.\n");
}

void printNberThreadsNeg(void) {
    fprintf(stderr, "Le nombre de threads de calcul doit être compris entre 1 et 127 inclus.\n");
}

void printThreadError(uint32_t id) {
    fprintf(stderr, "L'attente du thread %d a échouée.\n", id);
}


void printNeedEntryFile(void) {
    fprintf(stderr, "Vous devez fournir un fichier d'instance en entree !\n");
}


void printErrorWriting(const char *name_error, int32_t errnum) {
    fprintf(stderr, "Erreur d'écriture de '%s' : %s\n", name_error, strerror(errnum));
}


void printOpenFileError(int32_t errnum) {
    fprintf(stderr, "Erreur : impossible d'ouvrir le fichier => %s\n", strerror(errnum));
}


void printCloseFileError(int32_t errnum) {
    fprintf(stderr, "Erreur : impossible de fermer le fichier => %s\n", strerror(errnum));
}


void printMemoryAllocationError(const char *name, int32_t errnum) {
    fprintf(stderr, "Erreur d'allocation de mémoire : '%s'\n => %s\n", name, strerror(errnum));
}


void printMethodFailed(const char *name_method, int32_t errnum) {
    fprintf(stderr, "Erreur : %s() a echoué : %s\n", name_method, strerror(errnum));
}


void printVariableNULL(const char *nameVariable) {
    fprintf(stderr, "Erreur : %s est NULL\n", nameVariable);
}


// ================================== ERROR : END ================================== // 

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== BELLMAN-FORD : BEGIN ================================== // 


void initialisationSucceed(char *info_verbose, uint32_t source) {

    char buffer[36];
    if (sprintf(&(*buffer), "Noeud source = %d\n", source) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), "Initialisation du graphe réussie.\n") < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
}


void NegCycleNotDetected(char *info_verbose) {
    strcat(info_verbose, "Pas de cycle négatif détecté dans le graphe.\n\n");
}


void NegCycleDetected(char *info_verbose, uint32_t nodeFrom, uint32_t nodeTo, int32_t cost, int64_t *distance) {
    char buffer[110];
    if (sprintf(&(*buffer), "En ajoutant %d (= le coût du noeud %d au noeud %d) au coût du chemin vers le noeud %d,", cost, nodeFrom, nodeTo, nodeFrom) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), " on obtient un coût inférieur au coût du chemin vers le noeud %d.\n", nodeTo) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), "Il est donc possible de refaire indéfiniment ce cycle pour baisser le coût du chemin vers le noeud %d.\n", nodeTo) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), "distance[%d] = %"PRId64"\n", nodeFrom, distance[nodeFrom]) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), "cost = %d\n", cost) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    if (sprintf(&(*buffer), "distance[%d] = %"PRId64"\n", nodeTo, distance[nodeTo]) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    strcat(info_verbose, "=> Cycle négatif détecté dans le graphe.\n");
    strcat(info_verbose, "=> Réinitialisation de toutes les données.\n\n");
}


// ================================== BELLMAN-FORD : END ================================== // 

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== SECTION : BEGIN ================================== //


void printDecodingSection(void) {
    printf("\n-----------------\n");
    printf("PARTIE DECODAGE :");
    printf("\n-----------------\n"); 
}


void printAlgorithme_Encodage_Section(void) {
    printf("-------------------------------\n");
    printf("PARTIE : ALGORITHME ET ENCODAGE\n");
    printf("-------------------------------\n\n");
}


// ================================== SECTION : END ================================== //

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== STRUCT_MODIFIER : BEGIN ================================== //


void printInputGraphInfo(uint32_t nberNodes, uint32_t nberEdges) {
    printf("\nNombre de noeuds : %d\n", nberNodes);
    printf("Nombre de liens : %d\n", nberEdges);
    printf("\n");
}


void printInputGraphLien(uint32_t index, uint32_t from, uint32_t to, int32_t weight) {
    printf("Lien n°%d : %d -> %d (coût : %d)\n", index, from, to, weight);
}


// ================================== STRUCT_MODIFIER : END ================================== //

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== FILES : BEGIN ================================== //


void printDatas(uint32_t nberNodes, uint32_t sourceNode, uint32_t destinationNode, uint32_t maxLength, int64_t totalCost, int64_t *distance) {
    printf("Source = %d\n", be32toh(sourceNode));

    printf("        Distance = [ ");
    for (uint32_t i = 0; i < nberNodes - 1; i++) {
        if (distance[i] == INFINITY_VALUE) {
            printf("inf, ");
        } else {
            printf("%"PRId64", ", distance[i]);
        }
    }

    if (distance[nberNodes - 1] == INFINITY_VALUE) {
        printf("inf ]\n");
    } else {
        printf("%"PRId64" ]\n", distance[nberNodes - 1]);
    }

    printf("        Destination = %d\n", be32toh(destinationNode));
    if (be64toh(totalCost) == INFINITY_VALUE) {
        printf("        Cost = inf\n");
    } else {
        printf("        Cost = %"PRId64"\n", be64toh(totalCost));
    }
    printf("        LengthPath = %d\n", be32toh(maxLength));
    printf("        Path : ");
}


void printPath(uint32_t pathNode, bool extraLine) {
    printf("%d ", be32toh(pathNode));
    if (extraLine) {
        printf("\n\n===============================\n");
        printf("===============================\n\n");
    }
}


// ================================== FILES : END ================================== // 

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== SP : BEGIN ================================== //


void printTimer(double time_programme) {
    printf("=========================================================\n");
    printf("Temps d'excécution total du programme : %f secondes\n", time_programme);
    printf("=========================================================\n\n");
}


/*
* Affiche, sur la console stdout, un message d'usage du fichier sp.c.
*
* @params prog_name: pointeur vers un caractère représentant le nom du programme sp.c.
*
* @return: void.
*/
void usage(char *prog_name) {
    fprintf(stdout, "UTILISATION:\n");
    fprintf(stdout, "    %s [OPTIONS] input_file\n", prog_name);
    fprintf(stdout,
            "    input_file: chemin vers le fichier d'instance representant le "
            "graphe a traiter.\n");
    fprintf(
        stdout,
        "    -f output_file: chemin vers le fichier qui contiendra le resultat "
        "de programme, au format specifie dans l'enonce. Defaut : stdout.\n");
    fprintf(stdout,
            "    -n n_threads: nombre de threads de calcul executant "
            "l'algorithme de plus court chemin. Defaut : 4.\n");
    fprintf(stdout,
            "    -v: autorise les messages de debug. Si ce n'est pas active, "
            "aucun message de ce type ne peut etre affiche, excepte les "
            "messages d'erreur en cas d'echec. Defaut : false.\n");
}


// ================================== SP : END ================================== //

//////////////////////////////////////////////////////////////////////////////////////////

// ================================== VERIFY_OUTPUT : BEGIN ================================== // 


void printDifferentSize(size_t size1, size_t size2) {
    printf("Les fichiers n'ont pas la même taille...\n");
    printf("Size1 = %zu\n", size1);
    printf("Size2 = %zu\n", size2);
}


void printSameSize(size_t size) {
    printf("Les fichiers ont la même taille.\n");
    printf("Size = %zu\n", size);
}


// ================================== VERIFY_OUTPUT : END ================================== //