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
    bool (*compare)(float, float);
    float (*retornaChave)(void*);
    void (*liberaItem)(void*);
    void (*mudaChave)(void* item, float novaChave);
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

// Ajusta a heap, trazendo os nós de baixo para cima.
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

// Constroi a heap a partir de um vetor de itens
Heap* constroi_heap(void** vec, void* tam, bool (*compare)(float, float), float (*retornaChave)(void*), void (*liberaItem)(void*), void (*mudaChave)(void* item, float novaChave), void (*imprimeItem)(void* item)) {

    Heap* heap = malloc(sizeof(Heap));
    
    // Converte a chave representando o tamanho (esperado como número inteiro)
    int N = (int)retornaChave(tam);

    heap->nos = malloc((N+1) * sizeof(No_heap*)); 
    heap->tam = N;
    heap->compare = compare;
    heap->retornaChave = retornaChave;
    heap->liberaItem = liberaItem;
    heap->mudaChave = mudaChave;
    heap->imprimeItem = imprimeItem;

    // Inicializa o vetor de nós
    for(int i = 0; i < N; i++){
        heap->nos[i+1] = criaNo(vec[i]); 
    }

    // Constroi a árvore binária atribuindo os filhos (filho esquerdo em 2*i e direito em 2*i + 1)
    for(int i = 1; i <= N; i++){
        if(2*i > N) break;
        heap->nos[i]->esq = heap->nos[2*i];
        heap->nos[2*i]->pai = heap->nos[i];

        if(2*i + 1 > N) break; 
        heap->nos[i]->dir = heap->nos[2*i + 1];
        heap->nos[2*i + 1]->pai = heap->nos[i];
    } 

    heap->nos[0] = criaNo(tam);

    // Armazena a quantidade de itens no índice 0 
    heap->mudaChave(heap->nos[0]->item, (float)N);

    // Ajusta a heap
    heapify(heap);

    return heap;
}

No_heap* remove_min(Heap* heap) {
    if(!heap || !heap->nos || heap->tam <= 0) return NULL;

    int N = retorna_tam(heap);
    
    if (N == 1) {
        No_heap* minRef = heap->nos[1];
        heap->tam--;
        heap->mudaChave(heap->nos[0]->item, --(heap->tam));
        heap->nos[1] = NULL;
        return minRef;
    }
    
    trocaItem(heap->nos[1], heap->nos[N]);
    No_heap* minRef = heap->nos[N];
    
    heap->nos[N] = NULL;
    heap->mudaChave(heap->nos[0]->item, --(heap->tam));

    // Reajusta a heap
    heapify(heap);

    return minRef;     
}

void trocaItem(No_heap* a, No_heap* b){
    void* temp = a->item;
    a->item = b->item;
    b->item = temp;
}

// Imprime os nós a partir do índice 1
// A impressão é feita linearmente através do vetor de nós
void imprimeNos(Heap* heap){
    if(!heap || heap->nos == NULL || heap->tam <= 0){
        printf("--");
        return;
    }

    int N = heap->tam;

    for (int i = 1; i <= N; i++){
        heap->imprimeItem(heap->nos[i]->item);
    }
}


void libera_heap(Heap* heap){
    if (!heap) return;

    int N = heap->tam;

    for (int i = 0; i <= N; i++){
        if (heap->nos[i]){
            heap->liberaItem(heap->nos[i]->item);
            free(heap->nos[i]);
            heap->nos[i] = NULL; 
        }
    }
    
    free(heap->nos);
    free(heap);
}

void decrease_key(Heap* heap, void* item, float newKey) {
    if (!heap || !item) {
        return;
    }
    
    // Procura o índice do nó cuja 'item' é o desejado.
    int index = -1;
    for (int i = 1; i <= heap->tam; i++) {
        if (heap->nos[i]->item == item) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        // Item não encontrado na heap.
        return;
    }
    
    // Atualiza a chave do item usando a função mudaChave configurada
    heap->mudaChave(item, newKey);
    
    // Realiza o bubble-up à medida que a propriedade de heap foi violada.
    while(index > 1) {
        int parent = index / 2;
        float childKey = heap->retornaChave(heap->nos[index]->item);
        float parentKey = heap->retornaChave(heap->nos[parent]->item);
        if(heap->compare(childKey, parentKey)) {
            trocaItem(heap->nos[index], heap->nos[parent]);
            index = parent;
        } else {
            break;
        }
    }
}
