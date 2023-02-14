#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cliente.c"
#include "metadados.c"
#include "no.c"
#include "no_dados.c"

FILE * abrir_arquivo_escrita_novo(char *nome, FILE *f){

    // abre arquivo binário para escrita. Se o arquivo já existir o conteúdo é apagado.
    if((f = fopen(nome, "wb")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

FILE * abrir_arquivo_leitura(char *nome, FILE *f){

    // abre arquivo binário para leitura
    if((f = fopen(nome, "rb")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

FILE * abrir_arquivo_leitura_escrita(char *nome, FILE *f){

    // abre arquivo binário para leitura/escrita
    if((f = fopen(nome, "r+b")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

FILE * abrir_arquivo_leitura_escrita_novo(char *nome, FILE *f){

    // abre arquivo binário para leitura/escrita. Se o arquivo já existir o conteúdo é apagado.
    if((f = fopen(nome, "w+b")) == NULL){
        printf("Erro ao abrir o arquivo");
        exit(1);
    } else {
        return f;
    }

}

void iniciar_metadados(){
    FILE * f;
    
    // gravar informações em metadados.dat
    f = abrir_arquivo_escrita_novo("metadados.dat", f);
    iniciar_arquivo_metadados(f);
    fclose(f);

    // ler as informações de metadados.dat
    // f = abrir_arquivo_leitura("metadados.dat", f);
    // Metadados * md = le_metadados(f);
    // imprime_metadados(md);
    // free(md);
    // fclose(f);

    free(f);
}

void iniciar_indice(){
    FILE *f;

    // gravar informações em indice.dat
    f = abrir_arquivo_escrita_novo("indice.dat", f);
    fclose(f);

    free(f);
}

void iniciar_dados(){
    FILE *f;

    // gravar informações em dados.dat
    f = abrir_arquivo_escrita_novo("dados.dat", f);
    fclose(f);

    free(f);
}

/* FUNÇÕES DE TESTE */

void inserir_no_arquivo_de_indice(FILE *out){
    printf("\n-----------------------Inserindo informações no arquivo-----------------------\n");
    No *pag;
    for(int i=0; i<10; i++){
        pag = no();
        salva_no(pag, out);
        free(pag);
    }
}

void ler_arquivo_de_indice(FILE *in){
    printf("\n-----------------------Lendo arquivo de indice-----------------------\n");
    
    rewind(in);
    No *n;
    while((n = le_no(in)) != NULL){
        imprime_no(n);
        free(n);
        printf("\n");
    }

}

void teste_arquivo_de_indice(){
    FILE *findice = abrir_arquivo_leitura_escrita_novo("indice.dat", findice);

    inserir_no_arquivo_de_indice(findice);
    ler_arquivo_de_indice(findice);

    fclose(findice);
    free(findice);
}

void inserir_no_arquivo_de_dados(FILE *out){
    printf("\n-----------------------Inserindo informações no arquivo de dados-----------------------\n");
    
    NoDados *n;
    for(int i=0; i<4; i++){
        n = no_dados();
        n = inserir_cliente_em_no_dado(n, cliente(0*i, "ana"));
        n = inserir_cliente_em_no_dado(n, cliente(1*i, "bia"));
        n = inserir_cliente_em_no_dado(n, cliente(2*i, "carlos"));
        n = inserir_cliente_em_no_dado(n, cliente(3*i, "daniel"));
        
        salva_no_dados(n, out);
        libera_no_dados(n);
    } 
}

void atualizar_arquivo_de_dados(FILE *in){
    printf("\n-----------------------Atualizando arquivo de dados-----------------------\n");
    
    rewind(in);
    int pos = 0;
    fseek(in, tamanho_no_dados() * pos, SEEK_SET);
    NoDados *nd = no_dados();
    salva_no_dados(nd, in);
    libera_no_dados(nd);
}

void ler_arquivo_de_dados(FILE *in){
    printf("\n-----------------------Lendo arquivo de dados-----------------------\n");
    
    rewind(in);
    NoDados *nd;
    while((nd = le_no_dados(in)) != NULL){
        printf("\n");
        imprime_no_dados(nd);
        libera_no_dados(nd);
    }

}

void teste_arquivo_de_dados(){
    FILE *fdados = abrir_arquivo_leitura_escrita_novo("dados.dat", fdados);

    inserir_no_arquivo_de_dados(fdados);
    atualizar_arquivo_de_dados(fdados);
    ler_arquivo_de_dados(fdados);

    fclose(fdados);
    free(fdados);
}

int main(void){

    // iniciar_metadados();
    // iniciar_indice();
    teste_arquivo_de_indice();
    // teste_arquivo_de_dados();
    
    return 0;
}

