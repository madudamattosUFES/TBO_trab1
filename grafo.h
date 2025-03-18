#ifndef GRAPH_H
#define GRAPH_H

typedef struct Grafo Grafo;

Grafo* Cria_Grafo(int numVertices);

void Destroi_Grafo(Grafo* graph);

void AdicionaAresta_Grafo(Grafo* graph, int src, int dest, float peso);

Grafo* Le_Grafo(FILE* file);

void Imprime_Grafo(Grafo* graph);

#endif // GRAPH_H