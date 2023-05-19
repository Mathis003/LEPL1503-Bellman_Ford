#include "../../include/helper/sort_bin_files.h"

/*
* Note: Ce module n'a pas de tests unitaires car c'est une aide pour tester les autres fonctions.
*       Nous avons évidemment vérifier que ces fonctions marchent à 100% mais nous n'avons pas fais
*       de tests unitaires car le main ne nécessite pas de ces fonctions.
*       Pour verifyOutput.c, il y a des tests unitaires car nous l'avons fais
*       à l'époque même si ce n'était pas nécessaire selon nous.
*/


/*
* Crée la structure 'all_struct_t' avec toutes les données du fichier binaire de sortie de notre programme C.
* Cette structure est composé de blocs qui contiennent les données pour un noeud source i.
*
* @params name_graph: tableau de caractère (= pointeur) représentant le nom du fichier binaire de sortie.
*
* @return : nombe entier => EXIT_SUCCESS si la structure a été créée avec succès. EXIT_FAILURE sinon.
*/
uint8_t create_struct_to_sort(all_struct_t *all_datas, const char *name_graph) {

    FILE *fp;
    if ((fp = fopen(name_graph, "rb")) == NULL) {
        free(all_datas);
        printMethodFailed("fopen", errno);
        return EXIT_FAILURE;
    }

    uint32_t nberNodes;

    // Lis les données du fichier d'entrée.
    if (fread(&nberNodes, sizeof(uint32_t), 1, fp) != 1) {
        printf("Error: fread() failed\n");
        free(all_datas);
        return EXIT_FAILURE;
    }

    all_datas->nber_nodes = be32toh(nberNodes);

    all_datas->bloc = (outputGraph_t **) malloc(all_datas->nber_nodes * sizeof(outputGraph_t *));
    if (all_datas->bloc == NULL) {
        free(all_datas);
        return EXIT_FAILURE;
    }

    uint32_t sourceNode;
    uint32_t destinationNode;
    int64_t totalCost;
    uint32_t maxLength;
    uint32_t node_pathList;
    
    for (uint32_t i = 0; i < all_datas->nber_nodes; i++) {

        if (fread(&sourceNode, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        if (fread(&destinationNode, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        if (fread(&totalCost, sizeof(int64_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }
        
        if (fread(&maxLength, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        all_datas->bloc[i] = (outputGraph_t *) malloc(sizeof(outputGraph_t));
        if (all_datas->bloc[i] == NULL) {
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }
        all_datas->bloc[i]->nberNodes = be32toh(maxLength);
        all_datas->bloc[i]->sourceNode = be32toh(sourceNode);
        all_datas->bloc[i]->destinationNode = be32toh(destinationNode);
        all_datas->bloc[i]->totalCost = be64toh(totalCost);
        all_datas->bloc[i]->pathLength = be32toh(maxLength);

        all_datas->bloc[i]->pathList = (uint32_t *) malloc(all_datas->bloc[i]->pathLength * sizeof(uint32_t));
        if (all_datas->bloc[i]->pathList == NULL) {
            for (uint32_t k = 0; k < i; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        for (uint32_t j = 0; j < all_datas->bloc[i]->pathLength; j++) {
            if (fread(&node_pathList, sizeof(uint32_t), 1, fp) != 1) {
                printf("Error: fread() failed\n");
                for (uint32_t k = 0; k < i; k++) {
                    free(all_datas->bloc[k]->pathList);
                    free(all_datas->bloc[k]);
                }
                free(all_datas->bloc);
                free(all_datas);
                return EXIT_FAILURE;
            }

            all_datas->bloc[i]->pathList[j] = be32toh(node_pathList);
        }
    }

    if (closeFile(fp, true) == EXIT_FAILURE) {
        for (uint32_t k = 0; k < all_datas->nber_nodes; k++) {
            free(all_datas->bloc[k]->pathList);
            free(all_datas->bloc[k]);
        }
        free(all_datas->bloc);
        free(all_datas);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/*
* Fonction de comparaison pour le tri qsort des blocs contenus dans la structure 'all_struct_t'.
* Compare les deux blocs en fonction de la valeur de leur noeud source.
*
* @params bloc1: pointeur vers le premier bloc à comparer.
* @params bloc2: pointeur vers le deuxième bloc à comparer.
*
* @return: nombre entier => 0 si les deux blocs sont égaux.
                            un nombre négatif si le premier bloc est inférieur au deuxième.
                            un nombre positif si le premier bloc est supérieur au deuxième.
*/
int cmpfunc (const void *bloc1, const void *bloc2) {

    const outputGraph_t *info1 = *(const outputGraph_t **) bloc1;
    const outputGraph_t *info2 = *(const outputGraph_t **) bloc2;

    return (info1->sourceNode - info2->sourceNode);
}


/*
* Fonction qui créé un fichier binaire trié à partir d'un fichier binaire "désordonné" (quand réalisé avec plusieurs threads par notre programme C).
* Chaque bloc est trié en fonction de la valeur de son noeud source.
* Cette fonction permet de par la suite comparer les fichiers binaires triés avec les sortie du script python
* via les fonctions définies dans verifyOutput.c.
*
* @params name_graph: tableau de caractère (= pointeur) représentant le nom du fichier binaire "désordonné" (quand réalisé avec plusieurs threads).
*
* @return: nombre entier => EXIT_SUCCESS si le fichier a été créé, EXIT_FAILURE sinon.
*/
uint8_t create_sorted_bin_file(const char *name_graph) {

    all_struct_t *all_datas = malloc(sizeof(all_struct_t));
    if (all_datas == NULL) {
        printMemoryAllocationError("all_datas", errno);
        return EXIT_FAILURE;
    }

    if (create_struct_to_sort(all_datas, name_graph) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }
    
    qsort(all_datas->bloc, all_datas->nber_nodes, sizeof(outputGraph_t *), cmpfunc);

    FILE *fp;
    if ((fp = fopen(name_graph, "wb")) == NULL) {
        for (uint32_t k = 0; k < all_datas->nber_nodes; k++) {
            free(all_datas->bloc[k]->pathList);
            free(all_datas->bloc[k]);
        }
        free(all_datas->bloc);
        free(all_datas);
        printMethodFailed("fopen", errno);
        return EXIT_FAILURE;
    }

    uint32_t nberNodes = htobe32(all_datas->nber_nodes);

    // Ecrit les données dans le fichier de sortie.
    if (fwrite(&nberNodes, sizeof(uint32_t), 1, fp) != 1) {
        printf("Error: fread() failed\n");
        for (uint32_t k = 0; k < all_datas->nber_nodes; k++) {
            free(all_datas->bloc[k]->pathList);
            free(all_datas->bloc[k]);
        }
        free(all_datas->bloc);
        free(all_datas);
        return EXIT_FAILURE;
    }

    uint32_t sourceNode;
    uint32_t destinationNode;
    int64_t totalCost;
    uint32_t maxLength;
    uint32_t node_pathList;

    for (uint32_t i = 0; i < all_datas->nber_nodes; i++) {

        sourceNode = htobe32(all_datas->bloc[i]->sourceNode);
        destinationNode = htobe32(all_datas->bloc[i]->destinationNode);
        totalCost = htobe64(all_datas->bloc[i]->totalCost);
        maxLength = htobe32(all_datas->bloc[i]->pathLength);
        
        if (fwrite(&sourceNode, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = i; k < all_datas->nber_nodes; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        if (fwrite(&destinationNode, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = i; k < all_datas->nber_nodes; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        if (fwrite(&totalCost, sizeof(int64_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = i; k < all_datas->nber_nodes; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        if (fwrite(&maxLength, sizeof(uint32_t), 1, fp) != 1) {
            printf("Error: fread() failed\n");
            for (uint32_t k = i; k < all_datas->nber_nodes; k++) {
                free(all_datas->bloc[k]->pathList);
                free(all_datas->bloc[k]);
            }
            free(all_datas->bloc);
            free(all_datas);
            return EXIT_FAILURE;
        }

        for (uint32_t j = 0; j < all_datas->bloc[i]->pathLength; j++) {
            node_pathList = htobe32(all_datas->bloc[i]->pathList[j]);
            if (fwrite(&node_pathList, sizeof(uint32_t), 1, fp) != 1) {
                printf("Error: fread() failed\n");
                for (uint32_t k = i; k < all_datas->nber_nodes; k++) {
                    free(all_datas->bloc[k]->pathList);
                    free(all_datas->bloc[k]);
                }
                free(all_datas->bloc);
                free(all_datas);
                return EXIT_FAILURE;
            }
        }   

        free(all_datas->bloc[i]->pathList);
        free(all_datas->bloc[i]);
    }

    free(all_datas->bloc);
    free(all_datas);

    return closeFile(fp, true);
}