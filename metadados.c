#include "metadados.h"

void iniciar_arquivo_metadados(FILE *out){
    int pont_raiz = 0;
    int flag_raiz_folha = 0;
    
    fwrite(&pont_raiz, sizeof(int), 1, out);
    fwrite(&flag_raiz_folha, sizeof(int), 1, out);
}

Metadados * le_metadados(FILE *in){
    Metadados *md = (Metadados *) malloc(sizeof(Metadados));
    fread(&md->pont_raiz, sizeof(int), 1, in);
    fread(&md->flag_raiz_folha, sizeof(int), 1, in);
    
    return md;
}

void imprime_metadados(Metadados *md){
    printf("Ponteiro da raiz: %d\n", md->pont_raiz);
    printf("A raiz eh folha? %d\n", md->flag_raiz_folha);
}