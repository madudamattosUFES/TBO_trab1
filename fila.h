#include <stdio.h>
#include <stdbool.h>

typedef struct priority_queue PQ;

PQ* PQ_init(int* vec, int N);

int PQ_delmin(PQ* pq);

int PQ_min(PQ* pq);

bool PQ_empty(PQ* pq);

int PQ_size(PQ* pq);

void PQ_finish(PQ* pq);

void PQ_imprime(PQ* pq);