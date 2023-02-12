#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cliente.c"
#include "metadados.c"
#include "no.c"
#include "no_dados.c"

FILE * abrir_arquivo_escrita(char *nome, FILE *f){

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

void iniciar_metadados(){
    FILE * f;
    
    // gravar informações em metadados.dat
    f = abrir_arquivo_escrita("metadados.dat", f);
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
    f = abrir_arquivo_escrita("indice.dat", f);
    fclose(f);

    free(f);
}

void iniciar_dados(){
    FILE *f;

    // gravar informações em dados.dat
    f = abrir_arquivo_escrita("dados.dat", f);
    fclose(f);

    free(f);
}

/* FUNÇÕES DE TESTE */

void teste_arquivo_de_indice(){
    // Inicia variável do ponteiro para arquivo
    FILE *f;

    // escrevendo no arquivo de indice
    printf("\n-----------------------Salvando informações no arquivo-----------------------\n");
    f = abrir_arquivo_escrita("indice.dat", f);
    No *pag;
    for(int i=0; i<10; i++){
        pag = no();
        pag->m = i;
        salva_no(pag, f);
        free(pag);
    }
    fclose(f);
    
    // lendo no arquivo de indice
    printf("\n-----------------------Lendo informações no arquivo-----------------------\n");
    f = abrir_arquivo_leitura("indice.dat", f);
    No *pag_test;
    for(int i=0; i<10; i++){
        printf("\n- %d -\n", i);
        pag_test = le_no(f);
        imprime_no(pag_test);
        free(pag_test);
        printf("\n");
    }
    fclose(f);

    // Buscar nó pela posição
    printf("\n-----------------------Buscando nó pela posição-----------------------\n");
    f = abrir_arquivo_leitura("indice.dat", f);
    No *pagpos;
    pagpos = buscar_no(3, f);
    imprime_no(pagpos);
    free(pagpos);
    fclose(f);

    // Libera variável do ponteiro para arquivo
    free(f);
}

void teste_arquivo_de_dados(){
    FILE *f;

    // escrevendo no arquivo de dados
    printf("\n-----------------------Salvando informações no arquivo-----------------------\n");
    f = abrir_arquivo_escrita("dados.dat", f);
    NoDados *n = no_dados();
    n->s[0] = cliente(0, "ana");
    n->m += 1;
    n->s[1] = cliente(1, "bia");
    n->m += 1;
    n->s[2] = cliente(2, "carlos");
    n->m += 1;
    n->s[3] = cliente(3, "daniel");
    n->m += 1;
    salva_no_dados(n, f);
    libera_no_dados(n);
    fclose(f);

    // lendo no arquivo de dados
    printf("\n-----------------------Lendo informações no arquivo-----------------------\n");
    f = abrir_arquivo_leitura("dados.dat", f);
    NoDados *n1 = le_no_dados(f);
    imprime_no_dados(n1);
    libera_no_dados(n1);
    fclose(f);

    // liberando espaço de memória
    free(f);
}

int main(void){

    // iniciar_metadados();
    // iniciar_indice();
    // teste_arquivo_de_indice();
    teste_arquivo_de_dados();
    
    return 0;
}

