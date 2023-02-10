#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cliente.c"
#include "metadados.c"

FILE * abrir_arquivo_escrita(char *nome, FILE *f){

    if((f = fopen(nome, "wb")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

FILE * abrir_arquivo_leitura(char *nome, FILE *f){

    if((f = fopen(nome, "rb")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

void iniciar_metadados(){
    FILE * f;
    
    // gravar informações em metadados.dat
    f = abrir_arquivo_escrita("metadados.dat", f);
    iniciar_arquivo_metadados(f);
    fclose(f);

    // ler as informações de metadados.dat
    f = abrir_arquivo_leitura("metadados.dat", f);
    Metadados * md = le_metadados(f);
    imprime_metadados(md);
    free(md);
    fclose(f);
}

int main(void){
    iniciar_metadados();
    return 0;
}