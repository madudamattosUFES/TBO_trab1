#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "grafo.h"
#include <string.h>
#include "fila.h"

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

// Grafo* Le_Grafo(FILE* arquivo)
// {
//     int linhas = 0;
//     char buffer[1024];

//     // Conta quantas linhas válidas existem (ignorando vazias)
//     while (fgets(buffer, sizeof(buffer), arquivo)) {
//         if (buffer[0] != '\n' && buffer[0] != '\0') {
//             linhas++;
//         }
//     }

//     if (linhas < 1) {
//         printf("Arquivo vazio ou inválido.\n");
//         return NULL;
//     }

//     int numVertices = linhas - 1; // uma linha é o cabeçalho com raiz

//     // Cria o grafo
//     Grafo* grafo = Cria_Grafo(numVertices);

//     // Volta para o início do arquivo
//     rewind(arquivo);

//     // Lê a raiz
//     int raiz;
//     if (fscanf(arquivo, "node_%d\n", &raiz) != 1) {
//         printf("Erro ao ler a raiz.\n");
//         Destroi_Grafo(grafo);
//         return NULL;
//     }
//     grafo->raiz = raiz;

//     // Lê as arestas
//     for (int i = 0; i < numVertices; i++) {
//         if (!fgets(buffer, sizeof(buffer), arquivo)) break;

//         // Ignora linhas vazias
//         if (buffer[0] == '\n' || buffer[0] == '\0') {
//             i--; // repete o índice
//             continue;
//         }

//         int src;
//         char* ptr = buffer;

//         if (sscanf(ptr, "node_%d,", &src) != 1) {
//             printf("Erro ao ler o nó origem na linha %d: %s\n", i + 2, buffer);
//             Destroi_Grafo(grafo);
//             return NULL;
//         }

//         // Pula o prefixo até o início dos pesos
//         ptr = strchr(ptr, ',');
//         if (!ptr) continue;
//         ptr++; // aponta para depois da vírgula

//         for (int dest = 0; dest < numVertices; dest++) {
//             if (src == dest)
//                  continue;
            
//             char pesoStr[10];
//             int charsLidos = 0;
//             if (sscanf(ptr, " %9[^,],%n", pesoStr, &charsLidos) != 1) break;

//             float peso;
//             if (strcmp(pesoStr, "bomba") == 0) {
//                 peso = -1.0f;
//             } else {
//                 peso = atof(pesoStr);
//             }

//             if (src != dest && peso != 0) {
//                 AdicionaAresta_Grafo(grafo, src, dest, peso);
//             }
//             ptr += charsLidos;
//         }

//         // Adiciona aresta para ele mesmo com peso 0
//         AdicionaAresta_Grafo(grafo, src, src, 0.0f);
//     }

//     return grafo;
// }

Grafo* Le_Grafo(FILE* arquivo)
{
    int linhas = 0;
    char buffer[1024];

    Grafo *grafo = criaGrafo();
    char *linha = NULL;
    size_t tamanho_linha = 0;
    size_t bytes_lidos;
    int primeiro_no = 1; // flag para indicar se é o primeiro nó lido

    // Conta quantas linhas válidas existem (ignorando vazias)
    while (fgets(buffer, sizeof(buffer), arquivo)) {
        if (buffer[0] != '\n' && buffer[0] != '\0') {
            linhas++;
        }
    }

    if (linhas < 1) {
        printf("Arquivo vazio ou inválido.\n");
        return NULL;
    }

    int numVertices = linhas - 1; // uma linha é o cabeçalho com raiz

    // Cria o grafo
    Grafo* grafo = Cria_Grafo(numVertices);

    // Volta para o início do arquivo
    rewind(arquivo);

    while ((bytes_lidos = getline(&linha, &tamanho_linha, arquivo)) != -1)
    {
        // Remove newline e quebras
        linha[strcspn(linha, "\r\n")] = '\0';

        // Extrai número do nó
        char *token = strtok(linha, ",");
        if (!token)
            continue;

        int numero_node;
        if (sscanf(token, "node_%d", &numero_node) != 1)
        {
            fprintf(stderr, "Formato de nó inválido: %s\n", token);
            continue;
        }

        if (primeiro_no)
        {
            AdicionaAresta_Grafo(grafo, numero_node, numero_node, 0.0f); // Adiciona aresta para ele mesmo com peso 0

            primeiro_no = 0; // Desativa a flag
        }
        else
        {
            // Processa valores
            int pos = 0;
            while ((token = strtok(NULL, ",")) != NULL)
            {
                while (isspace(*token))
                    token++; // Remove espaços

                if (strcmp(token, "bomba") == 0 || atof(token) == 0)
                    AdicionaAresta_Grafo(grafo, numero_node, pos, -1.0f); // Adiciona aresta com peso -1
                    
                    /*aqui é passado o ponteiro de pos, pois há uma situação (quando o nó acha ele mesmo) que é necessário que a posição avance e a distância lida seja
                    posta no próximo nó (se o inicial for 50, quando ler a 50ª distância, a distância de 50 para 50 é definida como 0 e pos é incrementado para inserir a 50ª
                    no 51º nó, já que a distância do nó para ele mesmo não está contida na entrada)*/
                else
                {
                    float valor = atof(token);
                    AdicionaAresta_Grafo(grafo, numero_node, pos, valor);
                }
                pos++;
            }
        }

        // Adiciona aresta para ele mesmo com peso 0
        AdicionaAresta_Grafo(grafo, numero_node, numero_node, 0.0f);
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

            // Ignora arestas com peso -1 (inexistentes)
            if (peso == -1) continue;

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

        if (atual->vertice == g->raiz) {
            printf("<- node_%d ", g->raiz);
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


