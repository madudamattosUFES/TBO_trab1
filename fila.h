#include <stdio.h>
#include <stdbool.h>

typedef struct priority_queue PQ;

PQ* PQ_init(void** vec, void* tam, bool (*compare)(int, int), int (*retornaChave)(void*), void (*liberaItem)(void*), void (*mudaChave)(void* item, int novaChave), void (*imprimeItem)(void* item));

void* PQ_delmin(PQ* pq);

void* PQ_min(PQ* pq);

bool PQ_empty(PQ* pq);

int PQ_size(PQ* pq);

void PQ_finish(PQ* pq);

void PQ_imprime(PQ* pq);