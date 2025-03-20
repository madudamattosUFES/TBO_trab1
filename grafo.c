#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "fila.h"
#include <stdint.h>

#define INFINITO 1000000.0f

typedef struct No 
{
    int vertice; // vertice é o numero do nó de destino da aresta
    float peso; // peso é o peso pra chegar no nó destino
    struct No* prox;

    struct No* pai;
    struct No* filho;
    float distancia;
} No;

struct Grafo 
{
    int raiz;
    int numVertices;
    struct No** listaAdj;
};

No* Cria_No(int vertice, float peso)
{
    No* no = (No*)malloc(sizeof(No));
    no->vertice = vertice;
    no->peso = peso;
    no->prox = NULL;

    // para o djikstra
    no->pai = NULL;
    no->distancia = INFINITO;
    
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
        // adiciona uma aresta do grafo para ele mesmo com peso 0
        AdicionaAresta_Grafo(grafo, src, src, 0.0f);
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
            if(temp->vertice != i){
                printf("(%d, %.2f) ", temp->vertice, temp->peso);
            }
            temp = temp->prox;
        }
        printf("\n");
    }
}

void Dijkstra(Grafo* g){
    if(g == NULL || g->listaAdj == NULL){
        fprintf(stderr, "Grafo ou listaAdj é NULL.\n");
        return;
    }
    
    int N = g->numVertices;

    // Cria um vetor para armazenar todos os vértices do grafo
    No** vertices = (No**)malloc(N * sizeof(No*));

    // Cria o nó auxiliar para a primeira PQ
    No* tam1 = Cria_No(N, 0);
    tam1->distancia = N;


    //Inicializa a lista de vértices
    for(int i=0; i<N; i++){
        for(No* temp = g->listaAdj[i]; temp != NULL; temp = temp->prox){
            temp->distancia = INFINITO;
            temp->pai = NULL;
            temp->filho = NULL;
        }
        vertices[i] = g->listaAdj[i];
    }

    // inicializa raiz
    g->listaAdj[g->raiz]->distancia = 0.0f;

    // Inicializa a primeira PQ usando g->listaAdj e tam1
    PQ* pq = PQ_init((void**)g->listaAdj, (void*)tam1, 
                     compara_min, retornaDistancia, liberaItem, mudaDistancia, imprimeItem);

    // Cálculo dos caminhos mínimos (PQ1)
    while(!PQ_empty(pq)){
        No* atual = (No*)PQ_delmin(pq);
        if(atual == NULL) break;
        
        int u = atual->vertice;
        
        for(No* vizinho = g->listaAdj[u]; vizinho != NULL; vizinho = vizinho->prox){
            int v = vizinho->vertice;
            float peso = vizinho->peso;

            // Relaxamento com decrease-key
            if(g->listaAdj[v]->distancia > g->listaAdj[u]->distancia + peso){
                float novoDist = g->listaAdj[u]->distancia + peso;
                g->listaAdj[v]->distancia = novoDist;
                g->listaAdj[v]->pai = g->listaAdj[u];
                // Atualiza a posição do nó na heap da PQ
                PQ_decrease_key(pq, g->listaAdj[v], novoDist);
            }
        }
    }

    // A seguir, usamos uma segunda PQ para imprimir os caminhos mínimos
    No* tam2 = Cria_No(N, 0);
    tam2->distancia = N;
    

    PQ* pq2 = PQ_init((void**)vertices, (void*)tam2, 
                      compara_min, retornaDistancia, liberaItem, mudaDistancia, imprimeItem);

    
    while(!PQ_empty(pq2)){
        No* atual = (No*)PQ_delmin(pq2);
        if(atual == NULL){
            break;
        }
        printf("SHORTEST PATH TO node_%d: ", atual->vertice);
        No* temp = atual;
        while(temp != NULL){ 
            printf("node_%d ", temp->vertice);
            temp = temp->pai;
            if(temp != NULL) printf("<- ");
        }
        printf("(Distance: %.2f)\n", atual->distancia);
    }

    PQ_finish(pq);
    PQ_finish(pq2);
    free(tam1);
    free(tam2);
    free(vertices);
}

float retornaDistancia(void* i) {
    No* no = (No*)i;
    return no->distancia;
}

bool compara_min(float a, float b) {
    return a < b;
}

void imprimeItem(void* i) {
    No* no = (No*)i;
    printf("(%d %.2f)", no->vertice, no->distancia);
}

void liberaItem(void* i) {
    free(i);
}

void mudaDistancia(void* item, float novaChave) {
    No* no = (No*)item;
    no->distancia = novaChave;
}


