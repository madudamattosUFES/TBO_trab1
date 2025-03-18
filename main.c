#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int main(int argc, char* argv[])
{
    char caminhoArquivo[1000];
    sprintf(caminhoArquivo, "%s", argv[1]);
    FILE* arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL)
    {
        fprintf(stderr, "Error: Unable to open file\n");
        return 1;
    }

    Grafo* grafo = Le_Grafo(arquivo);

    Imprime_Grafo(grafo);

    Destroi_Grafo(grafo);
    fclose(arquivo);

    return 0;
}