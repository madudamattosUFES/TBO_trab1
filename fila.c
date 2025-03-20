#include <stdio.h>
#include <stdbool.h>
#include "fila.h"
#include "heap.h"

struct priority_queue {
    Heap* heap;
    int iUlt; // índice do último da fila
};

// Inicia a fila de prioridades a partir de um vetor de inteiros
PQ* PQ_init(void** vec, void* tam, bool (*compare)(float, float), float (*retornaChave)(void*), void (*liberaItem)(void*), void (*mudaChave)(void* item, float novaChave), void (*imprimeItem)(void* item)) {
    PQ* pq = malloc(sizeof(PQ));

    pq->heap = constroi_heap(vec, tam, compare, retornaChave, liberaItem, mudaChave, imprimeItem);

    pq->iUlt = (int)retornaChave(tam);

    return pq;
}

// Remove o menor item e retorna o seu valor, caso a fila esteja vazia retorna NULL
void* PQ_delmin(PQ* pq) {
    No_heap* minNo = remove_min(pq->heap);
    
    if (!minNo) return NULL;

    void* item = retorna_item(minNo);

    free(minNo);

    return item;
}


void* PQ_min(PQ* pq) {
    return retorna_item(retorna_min(retornaNos(pq->heap)));
}

bool PQ_empty(PQ* pq) {
    return pq->iUlt <= 0;
}

int PQ_size(PQ* pq) {
    return pq->iUlt; 
}

void PQ_finish(PQ* pq) {
    libera_heap(pq->heap);
    free(pq);
}

void PQ_imprime(PQ* pq) {
    if (!pq || pq->iUlt <= 0) {
        printf("\nFila vazia!");
        return;
    }    
    imprimeNos(pq->heap);
}

void PQ_decrease_key(PQ* pq, void* item, float novaChave) {
    decrease_key(pq->heap, item, novaChave);
}