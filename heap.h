#ifndef heap_h
#define heap_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct no_heap No;

int retorna_item(No* a);

No* retorna_dir(No* a);

No* retorna_esq(No* a);

No* retorna_pai(No* a);

No* retorna_min(No** nos);

int retorna_tam(No** nos);

No* remove_min(No** nos);

No* criaNo(int i);

void heapify(No** nos, bool (*compare)(int, int));

void heapify(No** nos, bool (*compare)(int, int));

No** constroi_heap(int* vec, int N);

bool compara_min(int a, int b);

void trocaItem(No* a, No* b);

void imprimeNos(No** nos);

void libera_heap(No** heap);

#endif