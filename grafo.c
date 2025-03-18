#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

typedef struct No 
{
    int vertice;
    float peso;
    struct No* prox;
} No;

struct Grafo 
{
    int raiz;
    int numVertices;
    No** listaAdj;
};

static No* Cria_No(int vertice, float peso)
{
    No* no = (No*)malloc(sizeof(No));
    no->vertice = vertice;
    no->peso = peso;
    no->prox = NULL;
    return no;
}

Grafo* Cria_Grafo(int numVertices)
{
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));
    grafo->numVertices = numVertices;
    grafo->listaAdj = (No**)malloc(numVertices * sizeof(No*));
    for (int i = 0; i < numVertices; i++) 
        grafo->listaAdj[i] = NULL;
    return grafo;
}

void Destroi_Grafo(Grafo* grafo)
{
    for (int i = 0; i < grafo->numVertices; i++) 
    {
        No* temp = grafo->listaAdj[i];
        while (temp) 
        {
            No* prev = temp;
            temp = temp->prox;
            free(prev);
        }
    }
    free(grafo->listaAdj);
    free(grafo);
}

void AdicionaAresta_Grafo(Grafo* grafo, int src, int dest, float peso)
{
    No* no = Cria_No(dest, peso);
    no->prox = grafo->listaAdj[src];
    grafo->listaAdj[src] = no;
}

Grafo* Le_Grafo(FILE* arquivo)
{
    int linhas = 0;
    char c, fim = 0;
    while ((c = fgetc(arquivo)) != EOF) {
        if (c == '\n') {
            linhas++;
        }
        fim = c;
    }
    // Se o último caractere for '\n', significa que houve uma linha em branco extra
    if (linhas == '\n' && linhas > 0)
    linhas--;

    int numVertices = linhas; // desconta o cabeçalho

    Grafo* grafo = Cria_Grafo(numVertices);
    rewind(arquivo);
    fscanf(arquivo, "node_%d\n", &grafo->raiz);

    int src;
    while (fscanf(arquivo, "node_%d, ", &src) != EOF)
    {
        for (int dest = 0; dest < numVertices; dest++)
        {
            if (src == dest)
                continue;
                
            float peso;
            fscanf(arquivo, "%f", &peso);
            fscanf(arquivo, ", ");
            if (peso != 0)
                AdicionaAresta_Grafo(grafo, src, dest, peso);
        }
        fscanf(arquivo, "\n");
    }

    return grafo;
}

void Imprime_Grafo(Grafo* graph)
{
    for (int i = 0; i < graph->numVertices; i++) 
    {
        No* temp = graph->listaAdj[i];
        printf("Vertex %d: ", i);
        while (temp) 
        {
            printf("(%d, %.2f) ", temp->vertice, temp->peso);
            temp = temp->prox;
        }
        printf("\n");
    }
}
