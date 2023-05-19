import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import argparse
import os

if __name__ == "__main__":

    parser = argparse.ArgumentParser(
        description="LEPL1503 - Analyse de performance")
    parser.add_argument(
        "-pdf_loc", help="chemin vers le dossier où les figures seront sauvegardées en format pdf. Si aucun chemin n'est spécifié, les figures ne seront pas sauvegardées.", default="None_location_pdf")
    parser.add_argument(
        "-svg_loc", help="chemin vers le dossier où les figures seront sauvegardées en format svg. Si aucun chemin n'est spécifié, les figures ne seront pas sauvegardées.", default="None_location_svg")
    args = parser.parse_args()

    # Récupération des chemins vers les dossiers où les figures seront sauvegardées
    location_pdf = args.pdf_loc
    location_svg = args.svg_loc

    # Création des dossiers (s'ils n'existent pas) où les figures seront sauvegardées
    if location_pdf != "None_location_pdf":
        if not os.path.exists(location_pdf):
            os.makedirs(location_pdf)
    
    if location_svg != "None_location_svg":
        if not os.path.exists(location_svg):
            os.makedirs(location_svg)
    

    fig = plt.figure()

    axe_threads_x = np.array([1, 2, 4, 10])

    axe_y_first_graph = np.array([2.84, 2.38, 2.65, 3.39])
    axe_y_second_graph = np.array([5.71, 3.85, 3.37, 4.27])
    axe_y_third_graph = np.array([71.38, 43.08, 28.31, 27.27])
    axe_y_4eme_graph = np.array([259, 159, 98.18, 97.63])
    axe_y_5eme_graph = np.array([1429, 757, 466, 463])

    axe_y = [axe_y_first_graph, axe_y_second_graph, axe_y_third_graph, axe_y_4eme_graph, axe_y_5eme_graph]

    title = "Temps d'exécution du programme C en fonction du nombre\nde threads pour différents graphes (avec tous |V| = 100)"
    colors = ['red', 'orange', 'blue', 'black', 'magenta']
    labels = ['Mesure pour un graphe de 10 liens', 'Mesure pour un graphe de 100 liens', 'Mesure pour un graphe de 1000 liens', 'Mesure pour un graphe de 10000 liens', 'Mesure pour un graphe de 100000 liens']

    # Définition de la fonction exponentielle
    def exponential_func(x, a, b, c):
        return a * np.exp(-b * x) + c
    
    for i in range(5):
        # Approximation de la courbe exponentielle
        popt, pcov = curve_fit(exponential_func, axe_threads_x, axe_y[i])

        # Génération des valeurs pour la courbe approximée
        x_fit = np.linspace(0.6, 10, 100)
        if i == 2:
            print(*popt)

            print(exponential_func(x_fit, *popt))
        y_fit = exponential_func(x_fit, *popt)

        plt.scatter(axe_threads_x, axe_y[i], label=labels[i], color=colors[i])
        plt.plot(x_fit, y_fit, color=colors[i])

    plt.grid(True, which="both")
    plt.yscale('log')
    plt.xlabel("Nombre de threads de calcul utilisé []")
    plt.ylabel("Temps d'exécution du programme C [millisecondes]")
    plt.legend(fontsize=7)
    plt.title("Temps d'exécution du programme C en fonction du nombre\nde threads pour différents graphes (|V| = 100 pour tous)")

    # Sauvegarde les graphes en format pdf et svg (si demandé par l'utilisateur via les arguments)
    if location_pdf != "None_location_pdf":
        plt.savefig(location_pdf + "/Temps_Exec_Threads.pdf")
    if location_svg != "None_location_svg":
        plt.savefig(location_svg + "/Temps_Exec_Threads.svg")

    plt.show()



    # Nombre de graphiques à tracer
    nber_graph = 6

    # Axe des abscisses (nombre de noeuds (ou de liens) du graphe)
    axe_x = np.array([10, 100, 1000, 10000, 100000])


    # Axe des ordonnées (temps d'exécution, mémoire allouée, consommation énergetique)
    # Pour les indexex 0 et 1 : temps d'exécution
    # Pour les indexex 2 et 3 : mémoire allouée
    # Pour les indexex 4 et 5 : consommation énergetique

    # Les indexes pairs ont comme axe des x le nombre de liens du graphe (variable) => noeud fixé à 100.
    # Les indexes impairs ont comme axe des x le nombre de noeuds du graphe (variable) => lien fixé à 100.

    # Test avec le programme Python en single thread et avec le programme C (avec 1, 2, 4 et 10 threads).

    axe_y_single_thread_Python = [np.array([0.2271700859, 1.734086561, 24.12546536, 276.871194, 2938.2943], dtype='float64'),
                                  np.array([0.030375003814697266, 1.7257561683654785, 184.20549964904785, 19851.606878757477, 999999999999999999999999999], dtype='float64'),
                                  np.array([1558892, 1549900, 1629836, 2418348, 10326860]),
                                  np.array([1548460, 1549900, 1564244, 1708244]),
                                  np.array([0.51, 0.59, 0.61, 0.61, 0.62], dtype='float64'),
                                  np.array([0.45, 0.6, 0.62, 0.63, 0.66], dtype='float64')]
    
    axe_y_single_thread_C = [np.array([0.0028382, 0.0057106, 0.0713786, 0.2588858, 1.4286626], dtype='float64'),
                                         np.array([0.0019322, 0.005825, 0.0921192, 4.9600254, 570.18938], dtype='float64'),
                                         np.array([17352, 40016, 66940, 174120, 1253728]),
                                         np.array([13452, 40016, 405208, 4133184, 41613184]),
                                         np.array([0.38, 0.38, 0.43, 0.46, 0.55], dtype='float64'),
                                         np.array([0.38, 0.38, 0.43, 0.56, 0.58], dtype='float64')]
    
    axe_y_2_thread_C = [np.array([0.00237991, 0.0038531, 0.0430829, 0.1587051, 0.7569604], dtype='float64'),
                                    np.array([0.0018225, 0.0039354, 0.0510815, 2.510973, 284.317347], dtype='float64'),
                                    np.array([19880, 42544, 69468, 176648, 1256256]),
                                    np.array([14900, 42544, 418536, 4133184, 42814512]),
                                    np.array([0.38, 0.38, 0.39, 0.46, 0.64], dtype='float64'),
                                    np.array([0.38, 0.38, 0.43, 0.69, 0.74], dtype='float64')]
    
    axe_y_4_thread_C = [np.array([0.0026496, 0.0033692, 0.0283144, 0.0981776, 0.466397], dtype='float64'),
                                    np.array([0.0022604, 0.0036627, 0.0376748, 1.3163904, 146.936756], dtype='float64'),
                                    np.array([24936, 47600, 74524, 181704, 1261312]),
                                    np.array([17796, 47600, 445192, 4497168, 45217168]),
                                    np.array([0.38, 0.38, 0.39, 0.45, 0.58], dtype='float64'),
                                    np.array([0.38, 0.38, 0.39, 0.54, 0.65], dtype='float64')]
    

    axe_y_10_thread_C = [np.array([0.0033873, 0.0042658, 0.0272756, 0.0976305, 0.4625102], dtype='float64'),
                                     np.array([0.0029418, 0.0041501, 0.0424012, 1.516862, 149.144230], dtype='float64'),
                                     np.array([40104, 62768, 89692, 196872, 1276480]),
                                     np.array([26484, 62768, 525160, 5225136, 52425136]),
                                     np.array([0.38, 0.38, 0.39, 0.45, 0.58], dtype='float64'),
                                     np.array([0.38, 0.38, 0.39, 0.54, 0.65], dtype='float64')]

    name_output_plot = ["Tmps_Exec_NodesFixed", "Tmps_Exec_EdgesFixed", "Memory_NodesFixed", "Memory_EdgesFixed", "Conso_Energ_NodesFixed", "Conso_Energ_EdgesFixed"]

    # Tension du Raspberry Pi
    Tension_Raspberry_Pi = 5.06


    for i in range(nber_graph):

        fig = plt.figure()

        if i == 4:
            axe_y_single_thread_Python[i] *= Tension_Raspberry_Pi * axe_y_single_thread_Python[0]
            axe_y_single_thread_C[i] *= Tension_Raspberry_Pi * axe_y_single_thread_C[0]
            axe_y_2_thread_C[i] *= Tension_Raspberry_Pi * axe_y_2_thread_C[0]
            axe_y_4_thread_C[i] *= Tension_Raspberry_Pi * axe_y_4_thread_C[0]
            axe_y_10_thread_C[i] *= Tension_Raspberry_Pi * axe_y_10_thread_C[0]
        if i == 5:
            axe_y_single_thread_Python[i] *= Tension_Raspberry_Pi * axe_y_single_thread_Python[1]
            axe_y_single_thread_C[i] *= Tension_Raspberry_Pi * axe_y_single_thread_C[1]
            axe_y_2_thread_C[i] *= Tension_Raspberry_Pi * axe_y_2_thread_C[1]
            axe_y_4_thread_C[i] *= Tension_Raspberry_Pi * axe_y_4_thread_C[1]
            axe_y_10_thread_C[i] *= Tension_Raspberry_Pi * axe_y_10_thread_C[1]
        
        # Plot les données du programme Python
        if i == 3:
            plt.plot(np.array([10, 100, 1000, 10000]), axe_y_single_thread_Python[i],'--', label= "Programme Python : 1 thread", marker="o", color="red")
        else:
            plt.plot(axe_x, axe_y_single_thread_Python[i],'--', label= "Programme Python : 1 thread", marker="o", color="red")

        # Plot les données du programme C
        plt.plot(axe_x, axe_y_single_thread_C[i],'--', label= "Programme C : 1 thread", marker="o", color="blue")
        plt.plot(axe_x, axe_y_2_thread_C[i],'--', label= "Programme C : 2 threads", marker="o", color="black")
        plt.plot(axe_x, axe_y_4_thread_C[i],'--', label= "Programme C : 4 thread", marker="o", color="magenta")
        plt.plot(axe_x, axe_y_10_thread_C[i],'--', label= "Programme C : 10 threads", marker="o", color="orange")

        # Mettre les axes en échelle logarithmique
        plt.xscale('log')
        plt.yscale('log')

        # Mettre une grille pour le style
        plt.grid(True, which="both")

        nber_pair = False
        if i % 2 == 0:
            nber_pair = True

        # Rétrécit la taille de la légende sur le graphe
        plt.legend(fontsize=9.8)

        # Mettre les titres et les labels en fonction de l'index
        if nber_pair:
            plt.xlabel("Nombre de liens : |R| []")
        else:
            plt.xlabel("Nombre de noeuds : |V| []")
            if i == 1:
                plt.ylim(0.001, 19851.606878757477 + 10000)

        if i <= 1:
            if nber_pair:
                plt.title("Temps d'exécution du programme en\nfonction du nombre de liens (pour |V| = 100)")
            else:
                plt.legend(fontsize=9.2)
                plt.title("Temps d'exécution du programme en\nfonction du nombre de noeuds (pour |R| = 100)")
            plt.ylabel("Temps d'exécution du programme [secondes]")
        elif i <= 3:
            if nber_pair:
                plt.title("Mémoire allouée par le programme en\nfonction du nombre de liens (pour |V| = 100)")
            else:
                plt.ylim(8000, 1605037840 + 400000000)
                plt.title("Mémoire allouée par le programme en\nfonction du nombre de noeuds (pour |R| = 100)")
            plt.ylabel("Mémoire allouée par le programme [octets]")
        else:
            if nber_pair:
                plt.title("Consommation énergétique du programme en\nfonction du nombre de liens (pour |V| = 100)")
            else:
                plt.legend(fontsize=9)
                plt.ylim(0.001, 77000)
                plt.title("Consommation énergétique du programme en\nfonction du nombre de noeuds (pour |R| = 100)")
            plt.ylabel("Consommation énergétique du programme [Joules]")

        # Sauvegarde les graphes en format pdf et svg (si demandé par l'utilisateur via les arguments)
        if location_pdf != "None_location_pdf":
            plt.savefig(location_pdf + "/" + "{}".format(name_output_plot[i]) + ".pdf")
        if location_svg != "None_location_svg":
            plt.savefig(location_svg + "/" + "{}".format(name_output_plot[i]) + ".svg")

        # Affiche le graphe
        plt.show()