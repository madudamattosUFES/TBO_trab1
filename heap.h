#ifndef heap_h
#define heap_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct no_heap No_heap;

int retorna_item(No_heap* a);

No_heap* retorna_dir(No_heap* a);

No_heap* retorna_esq(No_heap* a);

No_heap* retorna_pai(No_heap* a);

No_heap* retorna_min(No_heap** nos);

int retorna_tam(No_heap** nos);

No_heap* remove_min(No_heap** nos);

No_heap* criaNo(int i);

void heapify(No_heap** nos, bool (*compare)(int, int));

void heapify(No_heap** nos, bool (*compare)(int, int));

No_heap** constroi_heap(int* vec, int N);

bool compara_min(int a, int b);

void trocaItem(No_heap* a, No_heap* b);

void imprimeNos(No_heap** nos);

void libera_heap(No_heap** heap);

#endif