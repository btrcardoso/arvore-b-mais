#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cliente.c"
#include "metadados.c"
#include "no.c"
#include "no_dados.c"


/****************************************** FUNÇÕES PARA ABERTURA DE ARQUIVOS ******************************************/

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


/************************************ FUNÇÕES PARA CRIAÇÃO DOS ARQUIVOS DA ÁRVORE B+ ************************************/

void criar_arquivo_de_metadados(){
    FILE * f = abrir_arquivo_escrita_novo("metadados.dat", f);
    iniciar_arquivo_metadados(f);
    fclose(f);
    free(f);
}

void criar_arquivo_de_indice(){
    FILE *f = abrir_arquivo_escrita_novo("indice.dat", f);
    fclose(f);
    free(f);
}

void criar_arquivo_de_dados(){
    FILE *f = abrir_arquivo_escrita_novo("dados.dat", f);
    fclose(f);
    free(f);
}


/***************************************** FUNÇÕES DE TESTE DO ARQUIVO DE ÍNDICE *****************************************/

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

void testar_arquivo_de_indice(){
    FILE *findice = abrir_arquivo_leitura_escrita("indice.dat", findice);

    inserir_no_arquivo_de_indice(findice);
    ler_arquivo_de_indice(findice);

    fclose(findice);
    free(findice);
}


/***************************************** FUNÇÕES DE TESTE DO ARQUIVO DE DADOS *****************************************/

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

void testar_arquivo_de_dados(){
    FILE *fdados = abrir_arquivo_leitura_escrita("dados.dat", fdados);

    inserir_no_arquivo_de_dados(fdados);
    atualizar_arquivo_de_dados(fdados);
    ler_arquivo_de_dados(fdados);

    fclose(fdados);
    free(fdados);
}


/** BUSCAR **/

// retorna posicao do nó no arquivo de indice ou onde deveria estar
int busca(int x, FILE *f_metadados, FILE *f_indice, FILE *f_dados){

    // pegando metadados do arquivo
    Metadados *md = le_metadados(f_metadados);

    // posicao do nó nos arquivos de índice e dados
    int p_f_indice;
    int p_f_dados;

    // flag para saber se a chave foi encontrada
    int encontrou = 0;

    // posicao do nó no arquivo de indice ou onde deveria estar
    int p_f_indice_encontrado = -1;


    if(md->flag_raiz_folha == 1){
        // quando a raíz é folha (arquivo de dados), a busca é feita diretamente no arquivo de índice
        p_f_indice = -1;
        p_f_dados = 0;
    } else {
        // senão, a busca é feita primeiro no arquivo de índice
        p_f_indice = md->pont_raiz;
        p_f_dados = -1;
    }

    // busca no arquivo de indice
    while(p_f_indice != -1){

        No *pag = buscar_no(p_f_indice, f_indice);

        for(int i=0; i<4; i++){

            int si = pag->s[i];

            if(x < si){ 
                // buscar em um nó filho anterior à chave s[i]
                p_f_indice = pag->p[i];
                
                //sair do loop
                i = 3;
                
            } else if(i == 3 || pag->s[i+1] == -1) {             // tomar cuidado: considere que aqui o p[i+1] é diferente de -1
                // buscar em um nó filho posterior à chave s[i]
                p_f_indice = pag->p[i+1];

                //sair do loop
                i = 3;
            }

        }

        libera_no(pag);

        if(pag->flag_aponta_folha){
            // se a página aponta para o arquivo de dados, ir para busca no arquivo de dados
            p_f_dados = p_f_indice;
            p_f_indice_encontrado = p_f_indice;
            p_f_indice = -1;
        }

    }

    // busca no arquivo de dados
    while(p_f_dados != -1){

        NoDados *pag_dados = buscar_no_dados(p_f_dados, f_dados);

        for(int i=0; i<4; i++){

            int cod_cliente = pag_dados->s[i]->codCliente;

            if(cod_cliente > x || cod_cliente == -1){
                // não achou

                // sair do loop
                i = 3;
                p_f_dados = -1;

            } else if(cod_cliente == x) {
                // achou
                encontrou = 1;

                // sair do loop
                i = 3;
                p_f_dados = -1;
            } 

        }

        libera_no_dados(pag_dados);

    }

    if(encontrou == 1){
        printf("Chave encontrada na posição %d no arquivo de indice.\n", p_f_indice_encontrado);
    } else {
        printf("Chave não encontrada. Deveria estar na posição %d no arquivo de indice\n", p_f_indice_encontrado);
    }    

    free(md);
    return p_f_indice_encontrado;

}

/** INSERIR  **/

void inserir(Cliente *cli, FILE *fmd, FILE *fd){

    // pegando metadados do arquivo
    Metadados *md = le_metadados(fmd);

    if(md->pont_raiz == -1){  // nenhum cliente foi inserido ainda

        // Criação de nó de dados e inserção do primeiro cliente
        NoDados * nd = no_dados();
        nd = inserir_cliente_em_no_dado(nd, cli);
        salva_no_dados(nd, fd);
        libera_no_dados(nd);

        // atualização do arquivo de metadados
        md->pont_raiz = 0;
        md->flag_raiz_folha = 1;
        salva_metadados(md, fmd);

        // ler_arquivo_de_dados(fd);
        // Metadados *teste_md = le_metadados(fmd);
        // imprime_metadados(teste_md);
        // free(teste_md);

    } else { 

        

    }

    

    free(cli);
    free(md);
}

/******************************************************** MAIN ********************************************************/

int main(void){

    criar_arquivo_de_metadados();   
    criar_arquivo_de_indice();
    criar_arquivo_de_dados();

    FILE *fmd = abrir_arquivo_leitura_escrita("metadados.dat", fmd);
    FILE *fi = abrir_arquivo_leitura_escrita("indice.dat", fi);
    FILE *fd = abrir_arquivo_leitura_escrita("dados.dat", fd);
    
    {
        inserir(cliente(3,"ze das couve"), fmd, fd);
        busca(3, fmd, fi, fd);
        busca(2, fmd, fi, fd);
    }
    

    fclose(fmd);
    fclose(fi);
    fclose(fd);
    
    free(fmd);
    free(fi);
    free(fd);
    
    return 0;
}

