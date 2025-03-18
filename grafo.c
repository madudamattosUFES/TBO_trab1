#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"
#include "fila.h"

#define INFINITO 1000000.0f

typedef struct No 
{
    int vertice; // vertice é o numero do nó (node_0, node_1, node_2 ...)
    float peso; // peso é o peso pra chegar no nó
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

struct CamMinimo
{
    struct No* fonte;
    struct No* destino;
    struct No** filhos;
    int numFilhos;
    float distancia;
};

static No* Cria_No(int vertice, float peso)
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


// INICIO DA PARTE DO DJIKSTRA
/*
Lógica do algoritmo: pseudocódigo 

Djikstra(G, w, s)
    Inicializa-Single-Source(G, s)
    S = 0
    Q = V[G]
    while Q != 0
        u = Extract-Min(Q)
        S = S U {u}
        for each vertex v in Adj[u]
            Relax(u, v, w)
    return S    
*/

// struct camMinimo
// {
//     struct No* fonte;
//     struct No* pai;
//     struct No* destino;
//     struct No** filhos;
//     int numFilhos;
//     float distancia;
// };


CamMinimo** inicia_caminhosMinimos(Grafo* g){
    int numVertices = g->numVertices;
    CamMinimo** caminhosMinimos = (CamMinimo**)malloc(numVertices * sizeof(CamMinimo*));
    
    for(int i=0; i<numVertices; i++){
        CamMinimo* cam = (CamMinimo*)malloc(sizeof(CamMinimo));
        cam->fonte = g->raiz;
        cam->destino = g->listaAdj[i];
        cam->filhos = (CamMinimo**)malloc(numVertices * sizeof(CamMinimo*));
        cam->numFilhos = 0;
        cam->distancia = INFINITO;
        caminhosMinimos[i] = cam;
    }
}


void relaxa_vertice(CamMinimo* caminho, Grafo* grafo, int indice){
    int numVertices = grafo->numVertices;

    // visita a lista de adjacencia do vértice e faz o relaxamento
    No* no = grafo->listaAdj[indice];
    while(no){
        int vertice = no->vertice;
        float peso = no->peso;
        if(no->distancia > caminho->distancia  + peso){
            no->distancia = caminho->distancia + peso;
            no->pai = caminho->filhos[caminho->numFilhos];
            caminho->filhos[caminho->numFilhos] = no;
            caminho->numFilhos++;
        }
        no = no->prox;
    }

}


void Djikstra(Grafo* grafo, int src){
    PQ* pq = PQ_init();
    No** vistos; 
    while(!PQ_empty(pq)){
        int u = PQ_delmin(pq);
        insere(vistos, u);
        No* no = grafo->listaAdj[u];
        relaxa_vertice(caminho, grafo, u);
    }
    PQ_finish(pq);
}


void imprime_caminhosMinimos(CamMinimo** caminhosMinimos, Grafo* grafo){
    
    CamMinimo* cam = caminhosMinimos[0];
    // inicia fila de prioridade com os caminhos minimos
    PQ* pq = PQ_init(NULL, grafo->numVertices);
    while(!PQ_empty(pq)){
        // retira da fila de prioridades o caminho com menor distancia
        printf("SHORTEST PATH TO node_%d: ");
        
        for(int i=cam->numFilhos; i<=0; i--){
            print("node_%d ", cam->filhos[i]->vertice);
            if(i > 0){
                printf("<- ");
            }
        }

        printf(" (Distance: %.2f)\n", cam->distancia);
    }
    
}