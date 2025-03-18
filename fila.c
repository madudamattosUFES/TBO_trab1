#include <stdio.h>
#include <stdbool.h>
#include "fila.h"
#include "heap.h"

struct priority_queue{
    No** heap;
    int iUlt; // índice do último da fila
};

// Inicia a fila de prioridades a partir de um vetor de inteiros
PQ* PQ_init(int* vec, int N){
    PQ* pq = malloc(sizeof(PQ));

    pq->heap = constroi_heap(vec, 10);

    pq->iUlt = retorna_tam(pq->heap);

    return pq;
}

// Remove o menor item e retorna o seu valor, caso a vila esteja vazia retorna -1
int PQ_delmin(PQ* pq){
    No* minNo = remove_min(pq->heap);
    
    if(!minNo) return -1;

    int item = retorna_item(minNo);

    free(minNo);

    return item;
}

// Returns the largest item. Dual: min.
int PQ_min(PQ* pq){
    return retorna_item(retorna_min(pq->heap));
}

bool PQ_empty(PQ* pq){
    if(pq->iUlt <= 0) return true;
    else return false;
}

int PQ_size(PQ* pq){
    return pq->iUlt; 
}

void PQ_finish(PQ* pq){
    libera_heap(pq->heap);
    free(pq);
}

void PQ_imprime(PQ* pq){
    if(!pq || pq->iUlt <= 0){
        printf("\n Fila vazia!");
        return;
    }    
    imprimeNos(pq->heap);
}