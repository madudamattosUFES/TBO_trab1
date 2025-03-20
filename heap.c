#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "heap.h"

struct no_heap {
    void* item;
    No_heap* dir;
    No_heap* esq;
    No_heap* pai;
};

struct heap {
    No_heap** nos;
    int tam;
    bool (*compare)(int, int);
    int (*retornaChave)(void*);
    void (*liberaItem)(void*);
    void (*mudaChave)(void* item, int novaChave);
    void (*imprimeItem)(void* item);
};

void* retorna_item(No_heap* a) {
    return a->item;
}

No_heap* retorna_dir(No_heap* a) {
    return a->dir;
}

No_heap* retorna_esq(No_heap* a) {
    return a->esq;
}

No_heap* retorna_pai(No_heap* a) {
    return a->pai;
}

No_heap* retorna_min(No_heap** nos) {
    return nos[1];
}

int retorna_tam(Heap* heap) {
    return heap->tam;
}

No_heap** retornaNos(Heap* h) {
    return h->nos;
}

No_heap* criaNo(void* i) {
    No_heap* a = malloc(sizeof(No_heap));
    a->item = i;
    a->dir = NULL;
    a->esq = NULL;
    a->pai = NULL;
    return a;
}

// Ajeita a heap de uma árvore binária construída, trazendo os nós de baixo para cima um por um.
void heapify(Heap* heap) {
    int N = heap->tam;
    int i, j;

    for (i = N; i > 1; i--) {
        if (i / 2 < 1) break;

        for (j = i; j > 1; j = (j - 1) / 2) {
            if (heap->compare(heap->retornaChave(heap->nos[j]->item), heap->retornaChave(heap->nos[j]->pai->item))) {
                trocaItem(heap->nos[j]->pai, heap->nos[j]);
            } else break;
        }

        for (j = i; j > 1; j = j / 2) {
            if (heap->compare(heap->retornaChave(heap->nos[j]->item), heap->retornaChave(heap->nos[j]->pai->item))) {
                trocaItem(heap->nos[j]->pai, heap->nos[j]);
            } else break;
        }
    }
}

// Constroi uma arvore binária a partir de um vetor de inteiros e armazena todos os nós num vetor de Nós, e em seguida ajusta como estrutura heap.
// A árvore binária está construída e possui a raiz em nos[1].
// O índice 0 no vetor de nós (nos[0]) armazena a quantidade de nós (itens) na árvore.
// Retorna o vetor de nós
Heap* constroi_heap(void** vec, void* tam, bool (*compare)(int, int), int (*retornaChave)(void*), void (*liberaItem)(void*), void (*mudaChave)(void* item, int novaChave), void (*imprimeItem)(void* item)) {

    Heap* heap = malloc(sizeof(Heap));
    
    int N = retornaChave(tam);

    printf("N: %d\n", N);

    heap->nos = malloc((N+1) * sizeof(No_heap*)); 
    heap->tam = N;
    heap->compare = compare;
    heap->retornaChave = retornaChave;
    heap->liberaItem = liberaItem;
    heap->mudaChave = mudaChave;
    heap->imprimeItem = imprimeItem;

    // Inicializa o vetor de nós
    for(int i=0; i<N; i++){
        heap->nos[i+1] = criaNo(vec[i]); 
    }

    // Constroi a árvore binária assinalando os filhos correspondentes, onde para cada nó, seus filhos estão armazenado em 2n e 2n + 1.
    for(int i=1; i<=N; i++){
        if(2*i > N) break;
        heap->nos[i]->esq = heap->nos[2*i];
        heap->nos[2*i]->pai = heap->nos[i];

        if(2*i + 1 > N) break; 
        heap->nos[i]->dir = heap->nos[2*i + 1];
        heap->nos[2*i + 1]->pai = heap->nos[i];
    } 

    heap->nos[0] = criaNo(tam);

    // Armazena a quantidade de itens no índice 0 
    mudaChave(heap->nos[0]->item, N);

    // ajusta a heap de baixo para cima para a árvore construída
    heapify(heap);

    return heap;
}

// Retorna o nó que possui o item de valor mínimo
No_heap* remove_min(Heap* heap) {
    if(!heap || !heap->nos || heap->tam <= 0) return NULL;

    int N = retorna_tam(heap);
    
    if (N == 1) { // Se só há um elemento, apenas remove e retorna
        No_heap* minRef = heap->nos[1];
        heap->mudaChave(heap->nos[0]->item, --(heap->tam));
        heap->nos[1] = NULL;
        return minRef;
    }
    
    // troca o nó da cabeça da árvore com o ultimo
    trocaItem(heap->nos[1], heap->nos[N]);
    No_heap* minRef = heap->nos[N];
    
    heap->nos[N] = NULL;
    // atualiza o tam da heap e atualiza o no[0]
    heap->mudaChave(heap->nos[0]->item, --(heap->tam));

    // Processo de sink down para ajustar a heap
    No_heap* atual = heap->nos[1];

    // realiza o heapify novamente p/ ajustar a heap
    heapify(heap);

    return minRef;     
}

// Troca os itens(chaves) de dois nós
void trocaItem(No_heap* a, No_heap* b){
    void* temp = a->item;
    a->item = b->item;
    b->item = temp;
}

// Imprime os nós a partir do índice 1
// A impressão é feita linearmente através do vetor de nós
void imprimeNos(Heap* heap){
    if(!heap || !heap->nos || heap->tam <= 0){
        printf("--");
        return;
    }

    int N = heap->tam;

    for(int i=1; i<=N; i++){
        heap->imprimeItem(heap->nos[i]->item);
    }
}


void libera_heap(Heap* heap){
    if (!heap) return;

    int N = heap->tam;

    for(int i = 0; i<=N; i++){
        if (heap->nos[i]) {
            heap->liberaItem(heap->nos[i]->item);
            free(heap->nos[i]);
            heap->nos[i] = NULL; 
        }
    }
    
    free(heap);
}
