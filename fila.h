#ifndef FILA_H
#define FILA_H

#include <stdio.h>
#include <stdbool.h>

typedef struct priority_queue PQ;

PQ* PQ_init(void** vec, void* tam, 
            bool (*compare)(float, float), 
            float (*retornaChave)(void*), 
            void (*liberaItem)(void*), 
            void (*mudaChave)(void* item, float novaChave), 
            void (*imprimeItem)(void* item));

void* PQ_delmin(PQ* pq);
void* PQ_min(PQ* pq);
bool PQ_empty(PQ* pq);
int PQ_size(PQ* pq);
void PQ_finish(PQ* pq);
void PQ_imprime(PQ* pq);
void PQ_decrease_key(PQ* pq, void* item, float novaChave);

#endif // FILA_H