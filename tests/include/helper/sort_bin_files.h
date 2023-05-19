#ifndef LEPL1503__SORT_HELPER__
#define LEPL1503__SORT_HELPER__

#include "../../../include/files.h"


/*
* Note: La structure 'all_struct_t' ne se trouve pas dans le dossier ./src/unassociated/struct.h
*       car elle est utilisé uniquement utilisée lors des tests.
*/


/*
* Structure stockant tous les résultats du fichier binaire de sortie du programme.
* Utilisé pour la phase de vérification du programme (pour trier le fichier binaire
* afin de le comparer avec ceux issus du programme Python).
*
* @attribut nber_nodes: nombre entier non signé (sur 32 bits) représentant le nombre de noeuds du graphe d'entrée.
* @attribut bloc: pointeur vers un tableau de pointeur vers la structure 'outputGraph_t' représentant tous les résultats du programme.
*/
typedef struct {
    uint32_t nber_nodes;
    outputGraph_t **bloc;
} all_struct_t;


uint8_t create_struct_to_sort(all_struct_t *all_datas, const char *name_graph);

int cmpfunc (const void *bloc1, const void *bloc2);

uint8_t create_sorted_bin_file(const char *name_graph);


#endif // LEPL1503__SORT_HELPER__