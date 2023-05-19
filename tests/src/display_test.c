#include "../include/display_test.h"


/*
* Note 1: Il n'y a pas de documentation pour les fonctions de ce fichier car elles sont très simples et leur nom est explicite.
* Le principe est toujours le même:
*   - Rediriger stdout (ou stderr) vers un fichier temporaire créé au préalable.
*   - Appeler la fonction à tester.
*   - pour certaine fonctions, il faut printf() le 'info_verbose' après avoir appelé la fonction à tester avec en paramètre 'info_verbose'.
*   - Lire le contenu du fichier temporaire.
*   - Vérifier que le contenu du fichier temporaire est bien celui attendu.
*   - Restaurer stdout (ou stderr).
*   - Supprimer le fichier temporaire.
*/

/*
* Note 2: Les errnum utilisés dans les fonctions de ce fichier sont des valeurs aléatoires qui donne un même
*         message d'erreur sur Windows, Linux et Mac OS. Normalement, tous les tests devraient donc passer peu
*         importe l'OS utilisé.
*/

/*
* Initialise la suite de tests.
*/
int init_suite_display(void) {
    return EXIT_SUCCESS;
}


/*
* Clean la suite de tests.
*/
int clean_suite_display(void) {
    return EXIT_SUCCESS;
}


// === FUNCTION HELPER === //

/*
* Ferme le fichier temporaire et restaure stdout (ou stderr). Ensuite, réouvre le fichier temporaire en lecture seule
* et lit son contenu. Enfin, vérifie que le contenu du fichier temporaire est bien celui attendu.
* Supprime le fichier temporaire.
*
* @param tempFile: le nom du fichier temporaire.
* @param tmpfile_nber: le fichier temporaire.
* @param stream: stdout ou stderr.
* @param expected_buffer: le contenu attendu du fichier temporaire.
* @param size_buffer: la taille du buffer.
*
* @return: void.
*/
void end_funct_test(char tempFile[], FILE *tmpfile_nber, FILE *stream, char expected_buffer[], const uint32_t size_buffer) {

    // Ferme le fichier et restaure stdout
    if (closeFile(tmpfile_nber, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }
    
    freopen("/dev/tty", "w", stream);

    // Lit le contenu du fichier temporaire
    if ((tmpfile_nber = fopen(tempFile, "r")) == 0) {
        printMethodFailed("fopen", errno);
        exit(EXIT_FAILURE);
    }

    char buffer[size_buffer];
    memset(buffer, 0, sizeof(buffer));
    char line[size_buffer];
    while (fgets(line, sizeof(line), tmpfile_nber) != NULL) {
        strcat(buffer, line);
    }

    if (closeFile(tmpfile_nber, true) == EXIT_FAILURE) {
        exit(EXIT_FAILURE);
    }

    // Vérification
    CU_ASSERT_STRING_EQUAL(buffer, expected_buffer);

    // Supprime le fichier temporaire
    remove(tempFile);
}


void printOpenFileError_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    int32_t errnum = 1;
    printOpenFileError(errnum);

    char expected_buffer[68] = "Erreur : impossible d'ouvrir le fichier => Operation not permitted\n";
    const uint32_t size_buffer = 68;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
    
}


void printErrorMutexDestory_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    printErrorMutexDestory();

    char expected_buffer[33] = "Erreur de destruction du mutex.\n";
    const uint32_t size_buffer = 33;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printNberThreadsNeg_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    printNberThreadsNeg();

    char expected_buffer[74] = "Le nombre de threads de calcul doit être compris entre 1 et 127 inclus.\n";
    const uint32_t size_buffer = 74;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printThreadError_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    printThreadError(3);

    char expected_buffer[36] = "L'attente du thread 3 a échouée.\n";
    const uint32_t size_buffer = 36;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printNegCycleDetected_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    char *info_verbose = (char *) calloc(398, sizeof(char));
    if (info_verbose == NULL) {
        exit(EXIT_FAILURE);
    }

    char buffer[8];
    if (sprintf(&(*buffer), "TEST...") < 0) {
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);

    uint32_t nodeFrom = 1;
    uint32_t nodeTo = 5;
    int32_t cost = 10;
    int64_t *distance = (int64_t *) malloc(6 * sizeof(int64_t));
    if (distance == NULL) {
        exit(EXIT_FAILURE);
    }
    int64_t init_values_distance[] = {1, -5, 3, 5, -10, 2};
    memcpy(distance, init_values_distance, sizeof(init_values_distance));


    NegCycleDetected(info_verbose, nodeFrom, nodeTo, cost, distance);
    printf("%s", info_verbose);

    char expected_buffer[398] = "TEST...En ajoutant 10 (= le coût du noeud 1 au noeud 5) au coût du chemin vers le noeud 1, on obtient un coût inférieur au coût du chemin vers le noeud 5.\nIl est donc possible de refaire indéfiniment ce cycle pour baisser le coût du chemin vers le noeud 5.\ndistance[1] = -5\ncost = 10\ndistance[5] = 2\n=> Cycle négatif détecté dans le graphe.\n=> Réinitialisation de toutes les données.\n\n";
    const uint32_t size_buffer = 398;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
    free(info_verbose);
    free(distance);
}


void printNegCycleNotDetected_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    char *info_verbose = (char *) calloc(57, sizeof(char));
    if (info_verbose == NULL) {
        exit(EXIT_FAILURE);
    }

    char buffer[8];
    if (sprintf(&(*buffer), "TEST...") < 0) {
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);

    NegCycleNotDetected(info_verbose);
    printf("%s", info_verbose);

    char expected_buffer[57] = "TEST...Pas de cycle négatif détecté dans le graphe.\n\n";
    const uint32_t size_buffer = 57;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
    free(info_verbose);
}


void printVariableNULL_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    printVariableNULL("function");

    char expected_buffer[28] = "Erreur : function est NULL\n";
    const uint32_t size_buffer = 28;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printNeedEntryFile_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    };
    tmpfile_nber = freopen(tempFile, "w", stderr);

    printNeedEntryFile();

    char expected_buffer[54] = "Vous devez fournir un fichier d'instance en entree !\n";
    const uint32_t size_buffer = 54;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printSameSize_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    size_t size = 8980;
    printSameSize(size);
    char expected_buffer[47] = "Les fichiers ont la même taille.\nSize = 8980\n";
    const uint32_t size_buffer = 47;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printDifferentSize_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    size_t size1 = 32;
    size_t size2 = 12980;
    printDifferentSize(size1, size2);
    char expected_buffer[68] = "Les fichiers n'ont pas la même taille...\nSize1 = 32\nSize2 = 12980\n";
    const uint32_t size_buffer = 68;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printTimer_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    double time_programme = 10.125379;
    printTimer(time_programme);

    char expected_buffer[180] = "";
    const uint32_t size_buffer = 180;
    strcat(expected_buffer, "=========================================================\n");
    strcat(expected_buffer, "Temps d'excécution total du programme : 10.125379 secondes\n");
    strcat(expected_buffer, "=========================================================\n\n");
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printPathWithExtralineOff_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t pathNode = 5;
    pathNode = htobe32(pathNode);
    printPath(pathNode, false);

    char expected_buffer[3] = "5 ";
    const uint32_t size_buffer = 3;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printPathWithExtralineOn_Test() {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t pathNode = 193084;
    pathNode = htobe32(pathNode);
    printPath(pathNode, true);

    char expected_buffer[75] = "193084 \n\n===============================\n===============================\n\n";
    const uint32_t size_buffer = 75;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printInputGraphInfo_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t nberNodes = 12;
    uint32_t nberEdges = 562;
    printInputGraphInfo(nberNodes, nberEdges);

    char expected_buffer[47] = "\nNombre de noeuds : 12\nNombre de liens : 562\n\n";

    const uint32_t size_buffer = 47;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printInputGraphLien_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    };
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t index = 2;
    uint32_t from = 9;
    uint32_t to = 0;
    int32_t weight = -10;
    printInputGraphLien(index, from, to, weight);

    char expected_buffer[34] = "Lien n°2 : 9 -> 0 (coût : -10)\n";
    const uint32_t size_buffer = 34;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printDecodingSection_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    printDecodingSection();

    char expected_buffer[56] = "\n-----------------\nPARTIE DECODAGE :\n-----------------\n";
    const uint32_t size_buffer = 56;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printAlgorithme_Encodage_Section_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    printAlgorithme_Encodage_Section();

    char expected_buffer[98] = "-------------------------------\nPARTIE : ALGORITHME ET ENCODAGE\n-------------------------------\n\n";
    const uint32_t size_buffer = 98;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void initialisationSucceed_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t source = 11;
    char *info_verbose = (char *) calloc(61, sizeof(char));
    if (info_verbose == NULL) {
        exit(EXIT_FAILURE);
    }

    char buffer[8];
    if (sprintf(&(*buffer), "TEST...") < 0) {
        exit(EXIT_FAILURE);
    }
    strcat(info_verbose, buffer);
    
    initialisationSucceed(info_verbose, source);
    printf("%s", info_verbose);

    char expected_buffer[61] = "TEST...Noeud source = 11\nInitialisation du graphe réussie.\n";
    const uint32_t size_buffer = 61;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
    free(info_verbose);
}


void printErrorWriting_Test(void) {
    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    const char *name_error = "DestinationNode";
    int32_t errnum = 1;
    printErrorWriting(name_error, errnum);

    // => 1 for errno is "Operation not permitted"
    char expected_buffer[67] = "Erreur d'écriture de 'DestinationNode' : Operation not permitted\n";
    const uint32_t size_buffer = 67;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);

}


void printCloseFileError_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    int32_t errnum = 22;
    printCloseFileError(errnum);

    // => 22 for errno is "Invalid argument"
    char expected_buffer[62] = "Erreur : impossible de fermer le fichier => Invalid argument\n";
    const uint32_t size_buffer = 62;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printMemoryAllocationError_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    const char *name = "PathList";
    int32_t errnum = 12;
    printMemoryAllocationError(name, errnum);

    // => 12 for errno is "Cannot allocate memory"
    char expected_buffer[73] = "Erreur d'allocation de mémoire : 'PathList'\n => Cannot allocate memory\n";
    const uint32_t size_buffer = 73;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void printMethodFailed_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stderr);

    const char *name_method = "mmap";
    int32_t errnum = 14;
    printMethodFailed(name_method, errnum);

    // => 14 for errno is "Bad address"
    char expected_buffer[41] = "Erreur : mmap() a echoué : Bad address\n";
    const uint32_t size_buffer = 41;
    end_funct_test(tempFile, tmpfile_nber, stderr, expected_buffer, size_buffer);
}


void usage_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    char *prog_name = "Sp_program";
    usage(prog_name);

    char expected_buffer[549] = "";
    strcat(expected_buffer, "UTILISATION:\n    Sp_program [OPTIONS] input_file\n");
    strcat(expected_buffer, "    input_file: chemin vers le fichier d'instance representant le "
            "graphe a traiter.\n");
    strcat(expected_buffer, "    -f output_file: chemin vers le fichier qui contiendra le resultat "
        "de programme, au format specifie dans l'enonce. Defaut : stdout.\n");
    strcat(expected_buffer, "    -n n_threads: nombre de threads de calcul executant "
            "l'algorithme de plus court chemin. Defaut : 4.\n");
    strcat(expected_buffer, "    -v: autorise les messages de debug. Si ce n'est pas active, "
            "aucun message de ce type ne peut etre affiche, excepte les "
            "messages d'erreur en cas d'echec. Defaut : false.\n");

    const uint32_t size_buffer = 549;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);
}


void printDatas_Test(void) {

    // Redirige stdout vers un fichier temporaire
    char tempFile[22];
    FILE *tmpfile_nber;
    if (sprintf(tempFile, "tmpfile%d.txt", rand()) < 0) {
        printMethodFailed("sprintf", errno);
        exit(EXIT_FAILURE);
    }
    tmpfile_nber = freopen(tempFile, "w", stdout);

    uint32_t nberNodes = 3;
    uint32_t sourceNode = htobe32(1);
    uint32_t destinationNode = htobe32(0);
    uint32_t maxLength = htobe32(2);
    int64_t totalCost = htobe64(32);
    int64_t *distance = (int64_t *) malloc(nberNodes * sizeof(int64_t));
    int64_t init_values_distance[] = {10, 22, INFINITY_VALUE};
    memcpy(distance, init_values_distance, sizeof(init_values_distance));

    printDatas(nberNodes, sourceNode, destinationNode, maxLength, totalCost, distance);

    char expected_buffer[1000] = "Source = 1\n        Distance = [ 10, 22, inf ]\n        Destination = 0\n        Cost = 32\n        LengthPath = 2\n        Path : ";

    const uint32_t size_buffer = 1000;
    end_funct_test(tempFile, tmpfile_nber, stdout, expected_buffer, size_buffer);

    free(distance);
}


/*
* Fonction principale lançant les tests du module display.c.
* Cette fonction est appelée par le main du programme.test
*/
int main_display() {

    /* Ajoute une suite au registre de test */
    CU_pSuite pSuite = CU_add_suite("Test display.c", init_suite_display, clean_suite_display);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Ajoute une suite au registre de test */  
    if ((NULL == CU_add_test(pSuite, "printErrorMutexDestory_Test", printErrorMutexDestory_Test)) ||
        (NULL == CU_add_test(pSuite, "printOpenFileError_Test", printOpenFileError_Test)) ||
        (NULL == CU_add_test(pSuite, "printVariableNULL_Test", printVariableNULL_Test)) ||
        (NULL == CU_add_test(pSuite, "printNegCycleDetected_Test", printNegCycleDetected_Test)) ||
        (NULL == CU_add_test(pSuite, "printNegCycleNotDetected_Test", printNegCycleNotDetected_Test)) ||
        (NULL == CU_add_test(pSuite, "printThreadError_Test", printThreadError_Test)) ||
        (NULL == CU_add_test(pSuite, "printNberThreadsNeg_Test", printNberThreadsNeg_Test)) ||
        (NULL == CU_add_test(pSuite, "printNeedEntryFile_Test", printNeedEntryFile_Test)) ||
        (NULL == CU_add_test(pSuite, "printSameSize_Test", printSameSize_Test)) ||
        (NULL == CU_add_test(pSuite, "printDifferentSize_Test", printDifferentSize_Test)) ||
        (NULL == CU_add_test(pSuite, "printTimer_Test", printTimer_Test)) ||
        (NULL == CU_add_test(pSuite, "printPathWithExtralineOff_Test", printPathWithExtralineOff_Test)) ||
        (NULL == CU_add_test(pSuite, "printPathWithExtralineOn_Test", printPathWithExtralineOn_Test)) ||
        (NULL == CU_add_test(pSuite, "printInputGraphInfo_Test", printInputGraphInfo_Test)) ||
        (NULL == CU_add_test(pSuite, "printInputGraphLien_Test", printInputGraphLien_Test)) ||
        (NULL == CU_add_test(pSuite, "printDecodingSection_Test", printDecodingSection_Test)) ||
        (NULL == CU_add_test(pSuite, "printAlgorithme_Encodage_Section_Test", printAlgorithme_Encodage_Section_Test)) ||
        (NULL == CU_add_test(pSuite, "printErrorWriting_Test", printErrorWriting_Test)) ||
        (NULL == CU_add_test(pSuite, "printCloseFileError_Test", printCloseFileError_Test)) ||
        (NULL == CU_add_test(pSuite, "printMemoryAllocationError_Test", printMemoryAllocationError_Test)) ||
        (NULL == CU_add_test(pSuite, "printMethodFailed_Test", printMethodFailed_Test)) ||
        (NULL == CU_add_test(pSuite, "printDatas_Test", printDatas_Test)) ||
        (NULL == CU_add_test(pSuite, "initialisationSucceed_Test", initialisationSucceed_Test)) ||
        (NULL == CU_add_test(pSuite, "usage_Test", usage_Test)))
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return CUE_SUCCESS;
}