#include "../../include/helper/verifyOutput.h"


/*
* Vérifie si deux fichiers binaires sont identiques en taille.
*
* @params f1 et f2: pointeur vers un flux de fichier correspondant aux deux fichiers à comparer.
*
* @return: booléen => True si f1 a la même taille que f2. False sinon.
*/
bool check_same_size(FILE *f1, FILE *f2, bool verbose) {

    // On place le curseur à la fin du fichier 1.
    if (fseek(f1, 0L, SEEK_END) != 0) {
        printMethodFailed("fseek", errno);
        return false;
    }

    // On récupère la taille du fichier 1.
    uint64_t size1 = ftell(f1);
    if (fseek(f1, 0L, SEEK_SET) != 0) {
        printMethodFailed("fseek", errno);
        return false;
    }

    // On place le curseur à la fin du fichier 2.
    if (fseek(f2, 0L, SEEK_END) != 0) {
        printMethodFailed("fseek", errno);
        return false;
    }

    // On récupère la taille du fichier 2.
    uint64_t size2 = ftell(f2);
    if (fseek(f2, 0L, SEEK_SET) != 0) {
        printMethodFailed("fseek", errno);
        return false;
    }

    // On compare les tailles des deux fichiers.
    if (size1 == size2) {
        if (verbose) {
            printSameSize(size1);
        }
        return true;
    } else {
        if (verbose) {
            printDifferentSize(size1, size2);
        }
        return false;
    }
}


/*
* Vérifie si deux fichiers binaires sont identiques en contenus.
*
* @params f1 et f2: pointeur vers un flux de fichier correspondant aux deux fichiers à comparer.
*
* Note: cette fonction ne doit être appelée que si les deux fichiers ont la même taille !
*       Sinon elle doit être modifiée pour prendre en compte les cas où les fichiers n'ont pas la même taille.
*
* @return: booléen => True si f1 a le même contenu que f2. False sinon.
*/
bool check_same_contains(FILE *f1, FILE *f2) {
    
    int byte1, byte2;

    // On compare les contenus des deux fichiers.
    while ((byte1 = getc(f1)) != EOF && (byte2 = getc(f2)) != EOF) {
        if (byte1 != byte2) {
            return false;
        }
    }
    return true;
}


/*
* Détermine si deux fichiers binaires sont identiques en taille et en contenu.
*
* @params filename1: pointeur vers un tableau de caractères correspondant au nom du premier fichier à comparer.
* @params filename2: pointeur vers un tableau de caractères correspondant au nom du second fichier à comparer.
* @params verbose: booléen => True si l'on souhaite afficher les messages d'erreurs. False sinon.
*
* @return nombre entier => EXIT_SUCCESS si les 2 fichiers sont identiques. EXIT_FAILURE sinon.
*/
bool compare_two_outputFile(char *filename1, char *filename2, bool verbose) {
    
    FILE *f1;
    FILE *f2;

    // Ouverture des fichiers.
    if ((f1 = fopen(filename1, "rb")) == 0) {
        printMethodFailed("fopen", errno);
        return EXIT_FAILURE;
    }

    if ((f2 = fopen(filename2, "rb")) == 0) {
        printMethodFailed("fopen", errno);
        return EXIT_FAILURE;
    }

    // Vérification de la taille et du contenu des fichiers.
    bool returnValue = false;
    if (check_same_size(f1, f2, verbose)) {
        if (check_same_contains(f1, f2)) {
            returnValue = true;
        }
    }

    // Fermeture des fichiers.
    if (closeFile(f1, verbose) == EXIT_FAILURE) {
        return returnValue;
    }

    if (closeFile(f2, verbose) == EXIT_FAILURE) {
        return returnValue;
    }

    return returnValue;
}