#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct No No;
typedef struct Grafo Grafo;
typedef struct CamMinimo CamMinimo;

// Funções de manipulação do grafo
No* Cria_No(int vertice, float peso);
Grafo* Cria_Grafo(int numVertices);
void Destroi_Grafo(Grafo* grafo);
void AdicionaAresta_Grafo(Grafo* grafo, int src, int dest, float peso);
Grafo* Le_Grafo(FILE* arquivo);
void Imprime_Grafo(Grafo* grafo);

// Algoritmo de Dijkstra
void Dijkstra(Grafo* g, FILE* arquivoSaida);

// Funções auxiliares utilizadas internamente (na implementação do Dijkstra com PQ)
float retornaDistancia(void* i);
bool compara_min(float a, float b);
void imprimeItem(void* i);
int retornaChave(void* i);
void liberaItem(void* i);
void mudaChave(void* item, int novaChave);
void mudaDistancia(void* item, float novaChave);

#endif // GRAPH_H