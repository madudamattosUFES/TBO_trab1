#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct no_heap No_heap;
typedef struct heap Heap;

// Funções para manipulação de nós
void* retorna_item(No_heap* a);
No_heap* retorna_dir(No_heap* a);
No_heap* retorna_esq(No_heap* a);
No_heap* retorna_pai(No_heap* a);
No_heap* retorna_min(No_heap** nos);
int retorna_tam(Heap* heap);
No_heap** retornaNos(Heap* h);
No_heap* criaNo(void* i);

// Funções para manipulação da heap
void heapify(Heap* heap);
Heap* constroi_heap(void** vec, void* tam, bool (*compare)(int, int), int (*retornaChave)(void*), void (*liberaItem)(void*), void (*mudaChave)(void* item, int novaChave), void (*imprimeItem)(void* item));
No_heap* remove_min(Heap* heap);
void trocaItem(No_heap* a, No_heap* b);
void imprimeNos(Heap* heap);
bool compara_min(int a, int b);
void libera_heap(Heap* heap);

#endif // HEAP_H