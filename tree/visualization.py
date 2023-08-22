import matplotlib.pyplot as plt
import networkx as nx
import numpy as np
from networkx.drawing.nx_pydot import graphviz_layout
from matplotlib.lines import Line2D

file = open("../solution.txt")

line = file.readline()
list = line.split(sep = " ")
size = int(list[0])
adjacency_matrix = np.zeros((size, size))
while (line := file.readline()):
    list = line.split(sep = " ")
    if(list[0] == "-\n"): break
    vertex = int(list[0])
    for neigh in list[1:-1]:
        adjacency_matrix[vertex, int(neigh)] = 1

line = file.readline()
file.close()
solution = [eval(el) for el in line.split(sep = " ")[0:-1]]
non_solution = []
for el in range(size):
    if el not in solution:
        non_solution.append(el)

G = nx.from_numpy_matrix(adjacency_matrix)
pos = graphviz_layout(G, prog="dot")
plt.figure(figsize=(10,6))

nx.draw_networkx_nodes(G, pos, nodelist=non_solution, edgecolors="black",
                       node_size=1200, node_color="lightblue")
nx.draw_networkx_nodes(G, pos, nodelist=solution, edgecolors="black",
                       node_size=1200, node_color="lightyellow")
nx.draw_networkx_edges(G, pos)
nx.draw_networkx_labels(G, pos, labels={i:i for i in range(size)}, font_size=16)

legend_elements = [
    Line2D([0], [0], marker='o', color='black', markerfacecolor='lightblue',
           label='Vrhovi koji nisu u rješenju', markersize=15, linewidth=0),
    Line2D([0], [0], marker='o', color='black', markerfacecolor='lightyellow',
           label='Vrhovi unutar rješenja', markersize=15, linewidth=0),
]

plt.legend(handles=legend_elements, loc='upper left', prop={"size": 14})

plt.savefig("../visualization.png", format="PNG", dpi=600)
plt.show()