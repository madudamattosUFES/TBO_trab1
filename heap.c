#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "heap.h"

struct no_heap{
    int item;
    No* dir;
    No* esq;
    No* pai;
};

int retorna_item(No* a){
    return a->item;
}

No* retorna_dir(No* a){
    return a->dir;
}

No* retorna_esq(No* a){
    return a->esq;
}

No* retorna_pai(No* a){
    return a->pai;
}

No* retorna_min(No** nos){
    return nos[1];
}

int retorna_tam(No** nos){
    return nos[0]->item;
}

No* criaNo(int i){
    No* a = malloc(sizeof(No));

    a->item = i;
    a->dir = NULL;
    a->esq = NULL;
    a->pai = NULL;

    return a;
}

// Ajeita a heap de uma árvore binária construída, trazendo os nós de baixo para cima um por um. 
void heapify(No** nos, bool (*compare)(int, int)){

    int N = nos[0]->item;
    int i, j;

    for(int i=N; i>1; i--){
        if(i/2 < 1) break;

        for(int j = i; j>1; j=(j-1)/2){
            if(compare(nos[j]->item, nos[j]->pai->item)){
                trocaItem(nos[j]->pai, nos[j]);
            } else break; 
        }

        for(int j = i; j>1; j=j/2){ 
            if(compare(nos[j]->item, nos[j]->pai->item)){
                trocaItem(nos[j]->pai, nos[j]);
            } else break;
        }

    }

}

// Constroi uma arvore binária a partir de um vetor de inteiros e armazena todos os nós num vetor de Nós, e em seguida ajusta como estrutura heap.
// A árvore binária está construída e possui a raiz em nos[1]. 
// O índice 0 no vetor de nós (nos[0]) armazena a quantidade de nós (itens) na árvore.  
// Retorna o vetor de nós
No** constroi_heap(int* vec, int N){

    No** nos = malloc((N+1) * sizeof(No*));

    // Armazena a quantidade de iten no índice 0 
    nos[0] = criaNo(N);

    // Inicializa o vetor de nós
    for(int i=0; i<N; i++){
        nos[i+1] = criaNo(vec[i]); 
    }

    // Constroi a árvore binária assinalando os filhos correspondentes, onde para cada nó, seus filhos estão armazenado em 2n e 2n + 1.
    for(int i=1; i<=N; i++){
        if(2*i > N) break;
        nos[i]->esq = nos[2*i];
        nos[2*i]->pai = nos[i];

        if(2*i + 1 > N) break; 
        nos[i]->dir = nos[2*i + 1];
        nos[2*i + 1]->pai = nos[i];
    } 


    // ajusta a heap de baixo para cima para a árvore construída
    heapify(nos, compara_min);

    return nos;
}


// Retorna o nó que possui o item de valor mínimo
No* remove_min(No** nos){
    if(!nos || nos[0]->item <= 0) return NULL;

    int N = nos[0]->item;
    
    if (N == 1) { // Se só há um elemento, apenas remove e retorna
        No* minRef = nos[1];
        nos[0]->item = 0;
        return minRef;
    }
    
    // troca o nó da cabeça da árvore com o ultimo
    trocaItem(nos[1], nos[N]);
    No* minRef = nos[N];
    // nos[N] = NULL; 
    N = --nos[0]->item; // atualiza N

    // Processo de sink down para ajustar a heap
    No* atual = nos[1];

    // realiza o heapify novamente p/ ajustar a heap
    heapify(nos, compara_min);

    return minRef;     
}

// Troca os itens(chaves) de dois nós
void trocaItem(No* a, No* b){
    int temp = a->item;
    a->item = b->item;
    b->item = temp;
}


// Imprime os nós a partir do índice 1
// A impressão é feita linearmente através do vetor de nós
void imprimeNos(No** nos){
    int N = nos[0]->item;

    for(int i=1; i<=N; i++){
        printf("%d ", nos[i]->item);
    }
}

bool compara_min(int a, int b){
    return a < b ? true : false;
}

void libera_heap(No** heap){
    if (!heap) return;

    int N = heap[0]->item;

    for(int i = 0; i<=N; i++){
        if (heap[i]) {
            free(heap[i]);
            heap[i] = NULL; 
        }
    }

    free(heap);
}
