import graphviz
import struct
import argparse
import os


# Ce fichier est issu du code Python fourni par les professeurs pour le projet du
# cours LEPL1503 de l'année académique 2023-2024.

# Nous ne l'avons pas modifié mais c'était plus simple pour comparer les résultats de le mettre ici.


def read_graph(filename):
    with open(filename, "rb") as fd:
        data = fd.read(8)
        _, nb_links = struct.unpack(">ll", data)

        graph = dict()

        for _ in range(nb_links):
            data = fd.read(12)
            node_1, node_2, cost_12 = struct.unpack(">lll", data)
            graph.setdefault(node_1, list()).append((node_2, cost_12))

    return graph


def plot_graph(graph, output_filepath):
    file, file_extension = os.path.splitext(output_filepath)
    g = graphviz.Graph(format=file_extension[1:])

    for node in graph:
        for nei_id, cost in graph[node]:
            g.node(f"{node}", label=f"{node}")
            g.node(f"{nei_id}", label=f"{nei_id}")
            g.edge(f"{node}", f"{nei_id}", label=f"{cost}", dir="forward")

    g.render(file)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "file", type=str, help="Input file (binary) representing the graph")
    parser.add_argument("save", type=str,
                        help="Save the graph visualization in the indicated path")
    args = parser.parse_args()

    graph = read_graph(args.file)
    plot_graph(graph, args.save)
