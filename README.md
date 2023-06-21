# Description

Ce projet implémente, en langage C, `l'algorithme de plus court chemin de Bellman-Ford` dans un graphe orienté et pondéré pouvant contenir des poids négatifs.

Ce projet implémente une `architecture multithreading` simple et efficace. Seule la boucle principale du programme est effectuée par les threads car elle constitue à elle seule plus de 95% du temps d'exécution (du moins pour des graphes de tailles conséquentes).

Dans ce projet, l'algorithme de Bellman-Ford est appliqué |V| fois, |V| étant le nombre de noeud du graphe.
L'objectif est de trouver, pour chaque noeud source, le chemin plus long parmi tous les chemins les plus courts possibles (pour chaque noeud d'arrivée).

Pour arriver à cela, pour chaque noeud du graphe, l'algorithme calcule tous les coûts minimums pour accéder à chaque noeud d'arrivée. Ensuite, le noeud d'arrivée ayant le plus long coût sera choisi.
On y stockera également en résultat le chemin pour y accéder (Voir Spécification/fichier binaire de sortie).

# Introduction

Ce projet a été développé en `deuxième année de bachelier en ingénieur civil` à `l'université catholique de Louvain`, dans le cadre du `cours LEPL1503`.

L'objectif principal était d'apprendre les spécificités du `langage C` et de développer des compétences en `programmation concurrente` en utilisant des threads.

Le second objectif était d'apprendre à travailler en équipe à l'aide d'outils collaboratifs comme Git.

# Auteurs

Mathis Delsart et Adrien Antonutti.

15 Mai 2023.

# Spécification du programme

## Fichier binaire d'entrée

Ce programme prend en entrée un fichier `binaire` qui contient la représentation d'un graphe (pas nécessairement connexe) avec les informations suivantes dans l'ordre:
* le `nombre de noeuds` (entier non-signé encodé sur 32 bits)
* le `nombre de liens` (entier non-signé encodé sur 32 bits)
* |R| ensembles (|R| étant le nombre de liens) contenant :
    * le `noeud de départ` du lien (entier non-signé encodé sur 32 bits)
    * le `noeud d'arrivée` du lien (entier non-signé encodé sur 32 bits)
    * le `coût` du lien (entier signé encodé sur 32 bits)

## Fichier binaire de sortie

Le programme renvoie un fichier `binaire` de sortie qui contient les informations suivantes :
* le nombre de noeuds (entier non-signé encodé sur 32 bits)
* |V| ensembles (|V| étant le nombre de noeuds) contenant :
    * le `noeud de départ` (= source) (entier non-signé encodé sur 32 bits)
    * le `noeud d'arrivée` (= destination) [qui est définit comme le noeud du plus long chemin parmis les plus courts chemins partant du noeud source] (entier non-signé encodé sur 32 bits)
    * le `coût du plus long chemin parmi les plus courts chemins` (entier signé encodé sur 64 bits)
    * la `longueur de ce chemin` (entier non-signé encodé sur 32 bits)
    * les `noeuds constituant ce chemin` du `noeud source` au `noeud destination` (entiers non-signés encodés sur 32 bits)

### Remarques

Si le graphe d'entrée possède un `cycle négatif`, c'est-à-dire une boucle fermée dont `le coût total du chemin le long de cette boucle` est `inférieur à 0`, alors aucun chemin ne sera trouvé.

Si pour un noeud source `s`, un `cycle négatif` est trouvé alors :
* coût de distance `infini` pour tous les noeuds excepté le noeud source qui a un coût de `0`
* un chemin égal à `[s]`
* une longueur du chemin égale à `1` par conséquent
* un coût du chemin égal à `0`

# Structures de données

`5 structures de données` sont utilisés dans le programme principal et 6 pour les tests.
Ces structures permettent de `synthétiser` et de `stocker efficacement les résultats` et/ou informations nécessaires.

La structure `edge_t` représente `un lien du graphe`.

La structure `graph_t` est la représentation du fichier binaire d'entrée, et donc du `graphe d'entrée`.

La structure `outputGraph_t` est la représentation des `résultats de l'algorithme de Bellman ford pour un noeud source`. C'est donc un bloc de résultats sur les |V| ensembles à renvoyer dans le fichier binaire de sortie.

La structure `datas_threads_t` représente les `données communes` aux threads. Tous les threads peuvent librement accéder à cette même structure durant leur exécution et ce sans conflit.

La structure `datas_threads_algorithm_t` est une `structure propre à chaque thread` effectuant la phase algorithmique du projet, c'est-à-dire l'algorithme de Bellman ford. Elle contient donc des `données spécifiques unique pour chaque thread` et n'est donc absolument `pas partagés` entre eux.

La structure `all_struct_t` est uniquement utilisé lors du `test final du programme complet`. Elle stocke `tous les résultats d'un fichier binaire de sortie` afin de le `retrier` par la suite par ordre croissant des noeuds sources afin de pouvoir comparer la sortie avec celle du script Python.

Elles se trouvent toutes dans le fichier header `./include/unassociated/struct.h` exceptée `all_struct_t` qui se trouve dans le fichier header `./tests/include/helper/sort_bin_files.h`.
Tous les attributs sont bien expliqués dans ces fichiers headers.

# Structures des fichiers

La plupart des fichiers headers ont leur fichier source associé.

Ce n'est cependant pas le cas pour 4 fichiers headers se trouvant dans le dossier `./include/unassociated`:
* `./include/unassociated/macro.h` : Regroupe toutes les `macros utilisées` dans tous le code.
* `./include/unassociated/struct.h` : Regroupe toutes les strcutures utilisées dans tous le code (pas les tests).
* `./include/unassociated/portable_endian.h` : Regroupe toutes les fonctions permettant de `convertir le format des nombres binaires` ("Big Endian", "Little Endian" et "Host").
Ce fichier est nécessaire pour que le programme soit `compatible pour chaque OS` (dont MacOS).
* `./include/unassociated/portable_semaphore.h` : Regroupe toutes les fonctions permettant de créer, d'utiliser et de détruire des `sémaphores` (qui permet la `synchronisation des threads`). Ce fichier est également nécessaire pour que le programme soit `compatible pour chaque OS` (dont MacOS). Nous n'utilisons pas ce fichier car nous utilisons uniquement un mutex pour synchroniser les threads.


Pour les autres fichiers sources (et leur fichier .h associé), nous avons:
* `./src/bellman-ford.c` : Fonctions en rapport avec `l'algorithme de Bellman ford` mais également les fonctions permettant de `retracer l'historique des chemins`.
* `./src/display.c` : Fonctions qui `affichent à l'écran` des `messages de debug et/ou d'erreurs` sur la console stdout et/ou stderr. Tous les printf(), sprintf(),.. se retrouvent dans ce module.
* `./src/files.c` : Fonctions associées aux `fichiers (lecture et écriture)`. Toutes les fonctions permettant de `lire le fichier binaire d'entrée` et `d'écrire dans le fichier binaire de sortie` se trouvent dans ce module.
* `./src/struct_initializer.c`, `./src/struct_creater.c` et `./src/struct_free.c`: Fonctions permettant `d'initialiser, de créer et de libérer les structures` se trouvants dans `./include/unassociated/struct.h`. Nous aurions pu tout mettre en un seul module mais nous voulions rendre le code le plus clair possible.
* `./src/threads.c` : Fonctions permettant de `lancer les threads et de les attendres`.
* `sp.c` : Programme `principal` comprenant le `point d'entrée` (= fonction main()). C'est donc un module qui assemble toutes les fonctions des autres modules afin de produire le résultat voulu.


Le principe est le même pour le dossier tests : 
* `./tests/src/final_test.c` : Module permettant de `tester le programme complet` sur 19 graphes au total (14 cas de bords et 5 cas généraux) avec 1, 4 et 80 threads. C'est donc un `module de tests de validation`.
* `./tests/src/main_test.c` est le module `principal` des tests pour tous les lancer.
* `./tests/src/helper` est un dossier comprenant deux autres fichiers sources utilisés uniquement pour les tests. Les fichiers sont `./tests/src/helper/verifyOutput.c` et `./tests/src/helper/sort_bin_files.c` et servent à `trier dans l'ordre croissant des noeuds sources` le fichier binaire de sortie et puis de le `comparer avec la sortie du script Python` (uniquement dans `./tests/src/final_test.c`). Ils ont leur fichier .h associé également se trouvant dans le dossier `./tests/include/helper`.
* Tous les autres fichiers sont simplement des `modules de tests unitaires` correspondant à chaque module se trouvant dans `./src`.


Le dossier `./binary_files` comprend deux dossiers `./binary_files/general_case` et `./binary_files/limits_case` comprenant trois dossiers chacun qui ont les mêmes rôles :
* `./binary_files/.../input` : Comprend tous les fichiers binaires utilisés en tant que graphe d'entrée dans le test final (les cas généraux et cas de bords).
* `./binary_files/.../output_Python` : Comprend tous les fichiers binaires de sortie (via le script Python) avec comme graphe d'entrée les fichier se trouvant dans `./binary_files/.../input`
* `./binary_files/.../visualize_graph` : Représente les fichier binaire sous forme de graphes des fichiers se trouvant dans `./binary_files/.../input`.
* `./binary_files/.../output_C` : Comprend tous les fichiers binaires de sortie (via notre script C) avec comme graphe d'entrée les fichiers se trouvant dans `./binary_files/.../input`. Ce dossier est uniquement créé à l'appel de `make tests` et est supprimé à l'appel `make clean` (Commandes expliquées dans les sections `Compilation et exécution des tests` et `Suppression des fichiers compilés`).

`Remarque` : Les `...` entre les deux dossiers est soit `general_case`, soit `limits_case`.

Le dossier `./Python_script` contient tous les fichier `.py` fourni par les professeurs. Ils permettent de `générer des graphes aléatoire ou selon un fichier NTF`, de `vérifier` si notre output de notre programme C a `le bon format` attendu par les spécifications, de `visualiser les graphes` (voir dossier `./binary_files/.../visualize_graph` pour un exemple) et enfin de `run le programme Python` pour résoudre le même problème. Cela nous a donc été utile pour faire notre `analyse de performance`. Nous avons modifié l'un des fichier .py => Voir section `Extras` pour plus de détail.


Le dossier `./Visualize_structProject` contient un graphique (ainsi que le fichier .dot pour l'obtenir) permettant de `visualiser la structure du projet`. Plus d'informations se trouvent dans la section `Extras`.


Le dossier `./rapport` contient notre rapport final, un script Python ainsi qu'un dossier `./rapport/bin_graph` contenant les graphes utilisés pour la comparaison des performances sur différentes métriques (pour le rapport). Le script Python permet de générer les graphes et de les sauvegarder où vous le souhaitez. Plus d'informations à ce sujet dans la section `Extras`.


Le fichier `.gitignore` permet d'ignorer certains fichiers (ou type de fichiers) lorsqu'on push le code sur le GitLab.

Le MakeFile permet la `compilation automatique` du projet. Toutes les commandes associées au MakeFile sont expliquées dans la section `Commandes du MakeFile`.

Le fichier `README.md` est le fichier que vous lisez. Il permet aux reviewers d'avoir une meilleure approche du projet et de comprendre au préalable la base du projet avant de s'aventurer dans les détails d'implémention non décrits ici en détail.


# Dépendance

Pour compiler le projet, il faut également préalablement avoir installé l'outil `CMake` pour pouvoir utiliser les commandes du `MakeFile`.

Pour `run` les tests du projet, il faut également avoir installé la librairie `CUnit` sur votre machine.

Pour la commande du `make coverage` du MakeFile, il faut aussi avoir installé l'outil `lcov`.

### Installation

Toutes les informations pour installer CUnit, CMake et lcov peuvent être trouvées ici :
* https://sites.uclouvain.be/SyllabusC/notes/Outils/cunit.html (`CUnit`)
* https://wiki.student.info.ucl.ac.be/Logiciels/CMake (`CMake`)
* https://formulae.brew.sh/formula/lcov (`lcov` pour MacOS)


# Commandes du MakeFile

## Compilation du main

La commande `make` (ou `make main`) utilise le Makefile pour compiler tous les fichiers sources se trouvant dans le dossier `./src` ainsi que le fichier principal `sp.c` afin de créé un exécutable `main`.

## Compilation et exécution des tests

La commande `make tests` utilise le Makefile pour compiler tous les fichiers sources du main (dans le dossier `./src`), ainsi que ceux des tests (dans le dossier `./tests/src`). Un exécutable `unit_test` est créé et est directement exécuté par la commande. Cette commande créé également deux dossiers nommé `./binary_files/general_case/output_C` et `./binary_files/limits_case/output_C` (Lire la section `Structures des fichiers` pour plus d'informations).

#### Remarques

Pour le test final, les dossiers `binary_files/limits_case/output_C` et `binary_files/general_case output_C` sont créés avec les résultats des 19 graphes (realisé avec notre implémentation C) se trouvant dans les dossiers `binary_files/limits_case/input` et `binary_files/general_case/input`.

## Compilation, exécution des tests avec rapport de coverage

La commande `make coverage` a le même rôle que `make tests` mais crée en plus un rapport de coverage pour le projet. Celui-ci est disponible au format html dans ce fichier `./coverage/index.html`. Vous devez ouvrir ce fichier dans votre naviguateur pour apercevoir les résultats. Les résultats par fichier source sont également disponibles dans `./coverage/src`.

Ce `test coverage` permet d'avoir un aperçu de là où les tests passent et quelles fonctions et lignes sont testées.

Les lignes non testées sont en réalité les "blocs de codes" se situant après une condition du style : "if (Appel_Système() == A_Echoué) {"bloc de code"} où Appel_Système() est une fonction prédéfinie (fopen(), fread(), malloc(),...) et A_Echoué est la valeur de retour de la fonction Appel_Système() si elle échoue.

La raison pour laquelle ces blocs de codes ne sont pas testés est simplement car c'est compliqué de simuler des erreurs d'appel système.

## Suppression des fichiers compilés

La commande `make clean` utilise le Makefile pour supprimer :
* tous les `objects .o`
* tous les fichiers `exécutables` ayant l'extension `.exe` + `main` et `unit_test`
* les `répertoires de débogage` générés par le compilateur GCC (`main.dSYM` et `unit_test.dSYM`) [Se créé sur  MacOS lors de la compilation]
* les dossiers `binary_files/general_case/output_C` et `binary_files/limits_case/output_C`
* les fichiers auxiliaires du test du code coverage (mais pas le rapport situé dans le dossier coverage)

### Remarques

Pour utiliser les commandes du MakeFile, il faut toujours se trouver à la racine du projet.
Vous pouvez facilement vérifier en tapant la commande `ls` dans le terminal. Si `MakeFile` apparaît, vous êtes à la racine du projet. Sinon vous devez vous déplacer en tapant `cd NameFolder` pour `avancer` dans un dossier ou `cd ..` pour `reculer`.

## Exécution de l'exécutable sp (pour run le projet)

`./main [-h -v -f NameOutputFile -n NbreThreads] NameOfInputFile`

### Argument obligatoire
`NameOfInputFile` : Chemin vers le fichier binaire d'entrée représentant le graphe à traiter.

### Arguments optionnels
`-v` : Autorise les messages de debug (en plus des messages d'erreurs à l'exécution) et de résultats sur la sortie standart (= stdout).
=> false par défaut (= pas de messages autorisés).

`-f NameOutputFile` : Chemin vers le fichier binaire de sortie qui contiendra tous les résultats.
=> stdout par défaut (ne crée pas de fichier binaire mais affiche seulement au format texte les résultats sur la sortie standart).

`-n NbreThreads` :Nombre de threads de calcul.
=> 4 par défaut.

`-h` : Ecrit toutes les commandes et options dans le terminal.

### Exemples

* `./main my_graph.bin`
* `./main -f my_output_graph.bin my_graph.bin`
* `./main -n 8 -f my_output_graph.bin my_graph.bin`
* `./main -v -n 8 -f my_output_graph.bin my_graph.bin`
* `./main -h`
* ,...

# Contribution

Nous apprécions toutes les `contributions` à notre projet. Voici quelques façons dont vous pouvez nous aider.

## Signaler des bugs

Si vous trouvez un `bug` ou un comportement inattendu dans notre programme, vous pouvez créer une `issue` sur notre page `GitLab` pour nous le faire savoir.

## Proposer des fonctionnalités

Si vous avez une idée de `fonctionnalité` que vous aimeriez voir dans notre programme, vous pouvez créer une `issue` sur notre page `GitLab`.

## Soumettre des pull requests

Si vous avez une `solution à un problème` ou une `nouvelle fonctionnalité` à ajouter, vous pouvez `soumettre une pull request`.

# Extras

1. Dans le dossier `Visualize_structProject`, vous pouvez apercevoir la structure de la première version du projet (sans le multithreading) sous forme de graphique visuel. Le code, écrit à l'aide de la librairie `graphviz`, se trouve dans le fichier `structProject.dot`. Nous n'avons malheureusement pas eu le temps de mettre à jour ce graphique pour qu'il corresponde à la dernière version du projet (avec architecture multithreading). Pour `run le fichier .dot` et obtenir le graphe (si vous souhaitez l'améliorer), il suffit de tapper la commande suivante dans le `terminal` : `dot -T[format] Input_File -o Output_File.[format]`.
Par exemple, si vous vous trouvez à la racine du projet : `dot -Tpdf ./Visualize_structProject -o struct_Project.pdf`.

2. Dans les dossiers `binary_files/limits_case/visualize_graph` et `binary_files/general_case/visualize_graph`, vous pouvez apercevoir les 19 graphiques qui sont testés dans le test final. Ces graphiques ont été réalisés à l'aide du script Python et de la librairie `graphviz`.

3. Dans le dossier `./rapport` se trouve notre `rapport final du Projet` mais également les fichiers binaires testés pour l'analyse de performance. Le script Python `./rapport/perf_analyse.py` permet de générer les graphes se trouvant dans le rapport. Vous pouvez modifiez les données directement dans le script si vous le souhaitez.
Pour lancer le script, il suffit de faire la commande suivante : `python3 CheminRelatif [-pdf_loc Location_pdf_files -svg_loc Location_svg_files]`.
L'argument `CheminRelatif` est obligatoire et est le chemin relatif vers le fichier. Si vous vous trouvez à la racine du projet alors ce chemin sera `./rapport/perf_analyse.py`. Les deux autres arguments sont optionnels. Si vous ne les utilisez pas, les graphes ne seront pas sauvegardés. Si vous l'utilisez, les graphes seront sauvegardés là où vous avez demandé et dans le format `pdf`, `svg` ou bien même `les deux`.

4. Dans le dossier `./Python_script` fourni par les professeurs, nous avons modifié le fichier `./Python_script/create_graph.py` afin de permettre de générer également des `graphes complets`. Vous pouvez directement trouver plus de détail dans la `documentation` de ce fichier source.