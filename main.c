// #include <stdio.h>
// #include <stdlib.h>
// #include <math.h>
// #include "heap.h"
// #include "fila.h"
// #include "grafo.h"

// bool compara_min(int a, int b) {
//     return a < b;
// }

// void imprimeItem(void* i) {
//     No* no = (No*)i;
//     printf("%d ", no->vertice);
// }

// int retornaChave(void* i) {
//     No* no = (No*)i;
//     return no->vertice;
// }

// void liberaItem(void* i) {
//     free(i);
// }

// void mudaChave(void* item, int novaChave) {
//     No* no = (No*)item;
//     no->vertice = novaChave;
// }

// int main() {
//     int N = 5;

//     No* no1 = Cria_No(1, 0);
//     No* no2 = Cria_No(2, 0);
//     No* no3 = Cria_No(3, 0);
//     No* no4 = Cria_No(4, 0);
//     No* no5 = Cria_No(5, 0);
//     No* no0 = Cria_No(N, 0);

//     No** nos = (No**)malloc(N * sizeof(No*));
//     nos[0] = no5;
//     nos[1] = no4;
//     nos[2] = no3;
//     nos[3] = no2;
//     nos[4] = no1;

//     PQ* pq = PQ_init((void**)nos, (void*)no0, compara_min, retornaChave, liberaItem, mudaChave, imprimeItem);

//     printf("Fila Inicial:\n");

//     PQ_imprime(pq);

//     printf("\n");

//     printf("Removendo elementos:\n");
    
//     while (!PQ_empty(pq)) {
//         No* item = (No*)PQ_delmin(pq);

//         if (item == NULL) break;

//         PQ_imprime(pq);

//         printf("\nitem removido: ");
//         imprimeItem(item);
//         printf("\n");
//     }

//     PQ_finish(pq);
//     free(nos);

//     return 0;
// }