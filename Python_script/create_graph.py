import random
import argparse
import warnings
import os
import itertools
import random


# Ce fichier est issu du code Python fourni par les professeurs pour le projet du
# cours LEPL1503 de l'année académique 2023-2024.

# Nous l'avons modifié pour permettre de générer des graphes complets (le sens des arrêtes est aléatoire)
# ayant des coûts (aléatoires également) compris entre 1 et 10*nb_nodes. Cela permet de tester notre
# algorithme sur des graphes plus complexes que ceux réalisé de manière purement aléatoire.
# Les arguments ont donc été modifié afin d'y ajouter l'argument "-x int" où int est le nombre de noeuds du
# graphe complet généré. La fonction permettant de faire ceci est "generate_complete_graph_links(nb_nodes)" et
# les modules itertools et random ont été utilisés.


def generate_complete_graph_links(nb_nodes):
    nodes = {i: list() for i in range(nb_nodes)}
    nber_links = int((nb_nodes * (nb_nodes - 1)) / 2)

    for pair in itertools.combinations(nodes, 2):
        random_bin = random.randint(0, 1)
        node_a = pair[random_bin]
        node_b = pair[1 - random_bin]
        nodes.setdefault(node_a, list()).append((node_b, random.randint(1, 10*nb_nodes)))
        nodes.setdefault(node_b, list())

    return nodes, nber_links



def create_random_graph(args):

    nb_nodes = args.nodes
    nb_links = args.links

    random.seed(args.seed)

    nodes = {i: list() for i in range(nb_nodes)}
    for _ in range(nb_links):
        node_a = random.randint(0, nb_nodes - 1)
        node_b = random.randint(0, nb_nodes - 1)
        while node_b == node_a:
            node_b = random.randint(0, nb_nodes - 1)
        c_ab = random.randint(1, 10*nb_nodes)

        nodes.setdefault(node_a, list()).append((node_b, c_ab))
        nodes.setdefault(node_b, list())

    return nodes, nb_links


def ntf_parse(args):
    with open(args.ntf) as fd:
        data = fd.read().split("\n")
    
    # Get the number of nodes by checking at the max index.
    nb_nodes = 0
    for line in data:
        tab = line.split(" ")
        nb_nodes = max([nb_nodes, int(tab[0]), int(tab[1])])

    nodes = {i: list() for i in range(nb_nodes)}
    for line in data:
        tab = line.split(" ")
        node_a = int(tab[0])
        node_b = int(tab[1])

        c_ab = int(tab[2])
        nodes.setdefault(node_a, list()).append((node_b, c_ab))
        nodes.setdefault(node_b, list())

    return nodes, len(data)


def to_header_file(nodes, nb_links, output):
    # Check that the extension is correct.
    _, file_extension = os.path.splitext(output)
    if file_extension != ".h":
        warnings.warn(
            f'The extension of the output file is not ".h": {output}')

    s = ""

    # Create the array containing the links.
    for node_id, neighs in nodes.items():
        for (neigh_id, cost) in neighs:
            s += f"\t{{{node_id}, {neigh_id}, {cost}}},\n"
    s = s[:-2] + "\n"

    with open(output, "w+") as fd:
        content = f'#include <stdint.h>\n\n#define NB_NODES {len(nodes)}\n#define NB_LINKS {nb_links}\n\nint64_t links[NB_LINKS][3] = {{\n{s}}};'
        fd.write(content)


def to_binary_file(nodes, nb_links, output):
    nb_nodes = len(nodes)
    with open(output, "wb+") as fd:
        fd.write(nb_nodes.to_bytes(4, "big"))
        fd.write(nb_links.to_bytes(4, "big"))

        for node in nodes:
            for j, cost in nodes[node]:
                fd.write(node.to_bytes(4, "big"))
                fd.write(j.to_bytes(4, "big"))
                fd.write(cost.to_bytes(4, "big", signed=True))


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-t", "--ntf", type=str, default=None,
                        help="Parse an NTF file instead of creating a graph")
    parser.add_argument("-x", "--graph_complet", type=int, default=0,
                        help="Create a complete graph")
    parser.add_argument("-o", "--output", type=str,
                        help="Output file", default="graph.bin")
    parser.add_argument("-n", "--nodes", type=int,
                        help="Number of nodes. Unused if '--ntf'", default=5)
    parser.add_argument("-l", "--links", type=int,
                        help="Number of links. Unused if '--ntf'", default=10)
    parser.add_argument("-c", "--c-header", action="store_true",
                        help="Writes the graph as a C header file (.h) instead of a binary file")
    parser.add_argument("-s", "--seed", type=int, help="Seed for random generation of the graph", default=42)
    args = parser.parse_args()

    if args.ntf:
        graph, nb_links = ntf_parse(args)
    elif args.graph_complet != 0:
        graph, nb_links = generate_complete_graph_links(args.graph_complet)
    else:
        graph, nb_links = create_random_graph(args)


    if args.c_header:
        to_header_file(graph, nb_links, args.output)
    else:
        to_binary_file(graph, nb_links, args.output)
