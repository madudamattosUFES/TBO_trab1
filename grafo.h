#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct Grafo Grafo;

typedef struct CamMinimo CamMinimo;

Grafo* Cria_Grafo(int numVertices);

void Destroi_Grafo(Grafo* graph);

void AdicionaAresta_Grafo(Grafo* graph, int src, int dest, float peso);

Grafo* Le_Grafo(FILE* arquivo);

void Imprime_Grafo(Grafo* graph);



#endif // GRAPH_H