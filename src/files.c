#include "../include/files.h"


/*
* Ferme un fichier. Si le fichier ne peut pas être fermé, affiche un message d'erreur.
*
* @param fp: le pointeur vers le flux du fichier à fermer
* @param verbose: booléen => si true, affiche un message d'erreur si le fichier ne peut pas être fermé
*
* @return: un entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t closeFile(FILE *fp, bool verbose) {

    if (fclose(fp) == EOF) {
        if (verbose) {
            printCloseFileError(errno);
        }
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/*
* Lis le fichier d'entrée et crée la structure 'graph_t' qui va contenir les données du graphe d'entrée.
* Celle-ci est ensuite stockée dans la structure 'datas_threads_t'.
*
* @param datas_threads: pointeur vers la structure datas_threads_t contenant les données communes des threads.
*
* @return: un entier => EXIT_SUCCESS si tout s'est bien passé, EXIT_FAILURE sinon.
*/
uint8_t read_entry_file(datas_threads_t *datas_threads) {

    // Créer la structure graph_t qui va contenir les données du graphe d'entrée.
    if (create_basic_InputGraph(datas_threads) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free(datas_threads->mutex);
        free(datas_threads);
        printVariableNULL("input_graph");
        return EXIT_FAILURE;
    }

    uint32_t from;
    uint32_t to;
    int32_t cost;

    for (uint32_t i = 0; i < datas_threads->input_graph->nberEdges; i++) {

        // Lit les données du fichier d'entrée.
        if (fread(&from, sizeof(uint32_t), 1, datas_threads->input_fp) != 1) {
            printMethodFailed("fread", errno);
            pthread_mutex_destroy(datas_threads->mutex);
            free_datas_threads(datas_threads);
            return EXIT_FAILURE;
        }

        if (fread(&to, sizeof(uint32_t), 1, datas_threads->input_fp) != 1) {
            printMethodFailed("fread", errno);
            pthread_mutex_destroy(datas_threads->mutex);
            free_datas_threads(datas_threads);
            return EXIT_FAILURE;
        }

        if (fread(&cost, sizeof(int32_t), 1, datas_threads->input_fp) != 1) {
            printMethodFailed("fread", errno);
            pthread_mutex_destroy(datas_threads->mutex);
            free_datas_threads(datas_threads);
            return EXIT_FAILURE;
        }

        // Modifie la structure 'graph_t'.
        datas_threads->input_graph->edges[i].from = be32toh(from);
        datas_threads->input_graph->edges[i].to = be32toh(to);
        datas_threads->input_graph->edges[i].cost = be32toh(cost);

        // Affiche les données des liens.
        if (datas_threads->verbose) {
            printInputGraphLien(i, datas_threads->input_graph->edges[i].from, datas_threads->input_graph->edges[i].to, datas_threads->input_graph->edges[i].cost);
        }
    }

    if (closeFile(datas_threads->input_fp, datas_threads->verbose) == EXIT_FAILURE) {
        pthread_mutex_destroy(datas_threads->mutex);
        free_datas_threads(datas_threads);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/*
* Ecrit dans un fichier le nombre de noeuds du graphe.
* Si le nombre de noeuds ne peut pas être écrit, affiche un message d'erreur.
*
* @param datas_threads: un pointeur vers la structure 'datas_threads_t' contenant les données partagées entre les threads
*
* @return: nombre entier => EXIT_SUCCESS si le nombre de noeuds a pu être écrit, EXIT_FAILURE sinon.
*/
uint8_t writeNberNodes_OutputFile(datas_threads_t *datas_threads) {

    // On convertit le nombre de noeuds en big endian.
    uint32_t nberNodes = htobe32(datas_threads->input_graph->nberNodes);
    
    // On écrit le nombre de noeuds dans le fichier.
    if (datas_threads->fp_output != stdout) {
        if (fwrite(&nberNodes, sizeof(uint32_t), 1, datas_threads->fp_output) != 1) {
            printErrorWriting("nberNodes", errno);
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


/*
* Ecris toutes les données correspondant à un bloc dans un fichier binaire de sortie.
% Un bloc correspond aux résultats du programme pour un noeud source du graphe.
* Si les données ne peuvent pas être écrites, affiche un message d'erreur.
*
* @param datas_threads: le pointeur vers la structure 'datas_threads_t' contenant les données partagées entre les threads.
* @param outputGraph: le pointeur vers la structure 'outputGraph_t' contenant les données à écrire.
* @param distance: pointeur d'entier signé (sur 32bits) représentant les distances entre le noeud source et les autres noeuds du graphe.
*
* @return: nombre entier => EXIT_SUCCESS si les données ont pu être écrites, EXIT_FAILURE sinon.
*/
uint8_t writeData_OutputFile(datas_threads_t *datas_threads, outputGraph_t *outputGraph, int64_t *distance) {
    
    FILE *fp_output = datas_threads->fp_output;

    // On convertit les données en big endian.
    uint32_t sourceNode = htobe32(outputGraph->sourceNode);
    uint32_t destinationNode = htobe32(outputGraph->destinationNode);
    int64_t totalCost = htobe64(outputGraph->totalCost);
    uint32_t maxLength = htobe32(outputGraph->pathLength);


    if (fp_output == stdout) {
        // Pour afficher sur la sortie standart stdout
        datas_threads->verbose = 1;
    } else {
        // On écrit les données dans le fichier.
        if (fwrite(&sourceNode, sizeof(uint32_t), 1, fp_output) != 1) {
            printErrorWriting("sourceNode", errno);
            return EXIT_FAILURE;
        }

        if (fwrite(&destinationNode, sizeof(uint32_t), 1, fp_output) != 1) {
            printErrorWriting("destinationNode", errno);
            return EXIT_FAILURE;
        }

        if (fwrite(&totalCost, sizeof(int64_t), 1, fp_output) != 1) {
            printErrorWriting("totalCost", errno);
            return EXIT_FAILURE;
        }
        
        if (fwrite(&maxLength, sizeof(uint32_t), 1, fp_output) != 1) {
            printErrorWriting("maxLength", errno);
            return EXIT_FAILURE;
        }
    }

    // On affiche les données écrites dans stdout si verbose est à true.
    if (datas_threads->verbose) {
        printDatas(outputGraph->nberNodes, sourceNode, destinationNode, maxLength, totalCost, distance);
    }

    uint32_t node;
    bool extraline = false;

    // On écrit les noeuds du chemin dans le fichier.
    for (uint32_t i = 0; i < be32toh(maxLength); i++) {
        node = htobe32(outputGraph->pathList[i]);

        if (i == be32toh(maxLength) - 1) {
            extraline = true;
        }
        if (fp_output != stdout) {
            if (fwrite(&node, sizeof(uint32_t), 1, fp_output) != 1) {
                printErrorWriting("node", errno);
                return EXIT_FAILURE;
            }
        }
        // Affiche les noeuds du chemin.
        if (datas_threads->verbose) {
            printPath(node, extraline);
        }
    }
    return EXIT_SUCCESS;
}