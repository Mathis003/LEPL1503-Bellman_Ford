#ifndef LEPL1503__MACROS__
#define LEPL1503__MACROS__

/*
Tous les fichiers /src utilise la constante INFINITY.
Elle est définie ici car tous les fichiers /src import ce fichier graph.h.
Cette constante est utilisée pour l'algorithme de Bellman ford ainsi que
dans les autres fichiers /src pour vérifier si un noeud est accessible ou non.
Le choix de INT64_MAX (et pas INT32_MAX) est pour pour que le programme puisse
fonctionner avec des coût allant jusqu'à 9,223,372,036,854,775,807.
*/
#define INFINITY_VALUE INT64_MAX

/*
Seulement utilisé (en but de comparaison initiale) dans graph.c pour
obtenir le coût maximal entre les plus courts chemins obtenus via
l'algorithme de Bellman ford.
*/
#define MINUS_INFINITY INT64_MIN

/*
Constante permettant de choisir si le temps du programme s'affiche à la fin de l'exécution.
TIMER_ON = 1 => Affiche le temps d'excécution dans la console stdout.
TIMER_ON = 0 => N'affiche rien.
Attention : si le mode verbose (-v) est activé, le temps d'exécution sera d'office
afficher dans la console stdout et ce peu importe la valeur de TIMER_ON !
*/
#define TIMER_ON 1

#endif // LEPL1503__MACROS__