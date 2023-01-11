#include "cliente.c"

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

int main(void){
    return 0;
}