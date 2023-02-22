#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cliente.c"
#include "metadados.c"
#include "no.c"
#include "no_dados.c"


typedef struct Info{
    int p_f_indice;       // posição no arquivo de índice. se -1, então a raíz é folha. Se for maior que -1 então é uma posição do arquivo mesmo
    int p_f_dados;        // posicao no arquivo de dados 
    int pos_vetor_dados;  // posicao no vetor de dados ou posição que deveria estar no vetor de dados. [0,4]
    int encontrou;         // verifica se já existe cliente com o codigo buscado no arquivo de dados
} Info;


void imprime_info(Info *info){
    printf("\nPosição do nó no arquivo de índice: %d\n", info->p_f_indice);
    printf("Posicao do nó no arquivo de dados: %d\n", info->p_f_dados);
    printf("Posicao que o cliente está ou deveria no vetor S do nó de dados: %d\n", info->pos_vetor_dados);
    printf("Encontramos? %d\n\n", info->encontrou);
}

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
        n = inserir_cliente_em_no_dado(n, cliente(2*i, "carlos"));
        n = inserir_cliente_em_no_dado(n, cliente(1*i, "bia"));
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
    int cont = 0;
    while((nd = le_no_dados(in)) != NULL){
        printf("\n%d:\n", cont);
        imprime_no_dados(nd);
        libera_no_dados(nd);
        cont++;
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

/***************************************** FUNÇÕES DE TESTE DO ARQUIVO DE METADADOS *****************************************/
void ler_arquivo_de_metadados(FILE *in){
    printf("\n-----------------------Lendo arquivo de metadados-----------------------\n");
    
    Metadados *md = le_metadados(in);
    imprime_metadados(md);
    free(md);
}


/** BUSCAR **/

// retorna posicao do nó no arquivo de indice ou onde deveria estar
Info * busca(int x, FILE *f_metadados, FILE *f_indice, FILE *f_dados){

    // Inicializando informações
    Info *info = (Info *) malloc(sizeof(Info));
    info->p_f_indice = -1;      
    info->p_f_dados = 0;
    info->pos_vetor_dados = -1;
    info->encontrou = -1;        
    
    // pegando metadados do arquivo
    Metadados *md = le_metadados(f_metadados);

    // posicao do nó nos arquivos de índice e dados
    int p_f_indice;
    int p_f_dados;

    if(md->flag_raiz_folha == 1){
        // quando a raíz é folha, a busca é feita diretamente no arquivo de dados
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

            // Chave atual
            int si = pag->s[i];

            // salva a posição do nó atual no arquivo de índice
            info->p_f_indice = p_f_indice;

            if(x < si){ 
                // buscar em um nó filho anterior à chave s[i]
                p_f_indice = pag->p[i];
                
                //sair do loop for
                i = 3;
                
            } else if(i == 3 || pag->s[i+1] == -1) {             // tomar cuidado: considere que aqui o p[i+1] é diferente de -1
                // buscar em um nó filho posterior à chave s[i]
                p_f_indice = pag->p[i+1];

                //sair do loop for
                i = 3;
            }

        }

        if(pag->flag_aponta_folha){
            // se a página aponta para o arquivo de dados, ir para busca no arquivo de dados
            p_f_dados = p_f_indice;

            // sair do loop while
            p_f_indice = -1;
        }

        libera_no(pag);

    }

    // busca no arquivo de dados
    while(p_f_dados != -1){

        NoDados *pag_dados = buscar_no_dados(p_f_dados, f_dados);

        for(int i=0; i<4; i++){

            // Chave atual
            int cod_cliente = pag_dados->s[i]->codCliente;

            if(cod_cliente == x) {

                // achou
                info->encontrou = 1;
                info->p_f_dados = p_f_dados;
                info->pos_vetor_dados = i;

                // sair do loop for e while
                i = 3;
                p_f_dados = -1;

            } else if(cod_cliente > x || cod_cliente == -1 || i == 3) {

                // não achou
                info->encontrou = 0;
                info->p_f_dados = p_f_dados;
                info->pos_vetor_dados = i;  

                // sair do loop for e while
                i = 3;
                p_f_dados = -1;  

            } else {
                // continua buscando ...
            }

        }

        libera_no_dados(pag_dados);

    }  

    //imprime_info(info);

    free(md);
    return info;

}

Cliente ** vetor_ordenado(Cliente **s, Cliente *novo_cli){

    // inicia um novo vetor de 5 posições
    Cliente ** novo_vetor = (Cliente **) malloc(sizeof(Cliente*) * 5);

    // adiciona novo cliente no vetor de 5 posições
    for(int i = 0; i<5; i++){
        //novo_vetor[i] = (Cliente *) malloc(sizeof(Cliente *));

        if(i<4){
            novo_vetor[i] = s[i];
        } else {
            novo_vetor[i] = novo_cli;
        }
    }

    // ordenação bolha
    int i, j, n = 5;
    Cliente *aux;

    for(i=n-1;i>0;i--){
    
        for(j=0;j<i;j++){
        
            if(novo_vetor[j]->codCliente > novo_vetor[j+1]->codCliente){
                aux = novo_vetor[j];
                novo_vetor[j] = novo_vetor[j+1];
                novo_vetor[j+1] = aux;
            }
            
        }

    } 

    return novo_vetor;

}

/** INSERIR  **/

/*
funcao de inserir no arquivo de indice precisa:

    posicao do nó W no arquivo de ínidice 
    flag_aponta_folha
    posicao filho esqquerdo no arquivo (P)
    posicao filho direito no arquivo (Q)
*/

// Atualiza o pai de um nó_dado no arquivo de dados
void atualiza_pai_de_no_dado(FILE *f_dados, int p_f_dados, int ppai){
    NoDados *nd = buscar_no_dados(p_f_dados, f_dados);
    nd->ppai = ppai;
    fseek(f_dados, tamanho_no_dados() * p_f_dados, SEEK_SET);
    salva_no_dados(nd, f_dados);
    libera_no_dados(nd);
}

// recebe dois nós filhos e dá um pai pra eles
void inserir_em_arquivo_de_indice(int chave, int p_f_indice, int flag_aponta_folha, int p_filho_esq, int p_filho_dir, FILE *f_metadados, FILE *f_indice, FILE *f_dados){

    if(p_f_indice == -1){ // nós filhos são folhas e não tem pai

        // criacao no nó de índice
        No *no_pai = no();
        inserir_chave_em_no(no_pai, chave, p_filho_esq, p_filho_dir);
        no_pai->flag_aponta_folha = 1;

        // salvar nó de indice
        fseek(f_indice, 0, SEEK_END);                                  // posiciona ponteiro no fim do arquivo
        salva_no(no_pai, f_indice);                                    // salva
        int no_pai_p_f_indice = (ftell(f_indice) / tamanho_no()) - 1;  // pega a posição do nó no arquivo
    
        // atualizacao do ponteiro raiz no arquivo de metadados
        atualiza_arquivo_metadados(f_metadados, no_pai_p_f_indice, 0);

        // atualização do pai do filho esquerdo e direito no arquivo de dados
        atualiza_pai_de_no_dado(f_dados, p_filho_esq, no_pai_p_f_indice);
        atualiza_pai_de_no_dado(f_dados, p_filho_dir, no_pai_p_f_indice);

        // liberar nó pai
        libera_no(no_pai);

    } else { // o nó filho esquerdo tem um pai no arquivo de índice

        No *no_pai = buscar_no(p_f_indice, f_indice);

        if(no_pai->m < 4){ // O nó tem espaço 

            // insere a chave no nó 
            inserir_chave_em_no(no_pai, chave, -1 /* conferir se é -1 ou p_filho_esq */, p_filho_dir);

            // salvar nó de indice atualizado
            fseek(f_indice, tamanho_no() * p_f_indice, SEEK_SET); 
            salva_no(no_pai, f_indice);                                    
            
            // atualização do pai do filho direito no arquivo de indice/dados
            if(no_pai->flag_aponta_folha == 0){

                // atualização do pai do filho direito no arquivo de indice
                No *nd = buscar_no(p_filho_dir, f_indice);
                nd->ppai = p_f_indice;
                fseek(f_indice, tamanho_no() * p_filho_dir, SEEK_SET);
                salva_no(nd, f_indice);
                libera_no(nd);

            } else if(no_pai->flag_aponta_folha == 1){

                // atualização do pai do filho direito no arquivo de dados
                atualiza_pai_de_no_dado(f_dados, p_filho_dir, p_f_indice);

            }

            // liberar nó pai
            libera_no(no_pai);

        } else { // O nó de indice está cheio e deve ser particionado

            printf("Nó cheio. Tratar. sair.");
            exit(1);
        
        }
        

    }

}

void inserir(Cliente *cli, FILE *f_metadados, FILE *f_indice, FILE *f_dados){

    rewind(f_metadados);
    rewind(f_indice);
    rewind(f_dados);

    Metadados *md = le_metadados(f_metadados);
    NoDados * nd;
    Info * info;

    if(md->pont_raiz == -1){  // nenhum cliente foi inserido ainda

        // Criação de nó de dados e inserção do primeiro cliente
        nd = no_dados();
        nd = inserir_cliente_em_no_dado(nd, cli);
        salva_no_dados(nd, f_dados);
        libera_no_dados(nd);

        // atualização do arquivo de metadados
        md->pont_raiz = 0;
        md->flag_raiz_folha = 1;
        salva_metadados(md, f_metadados);

    } else {// há clientes inseridos na base

        info = busca(cli->codCliente, f_metadados, f_indice, f_dados);

        if(info->encontrou == 1){
            printf("Não é possível inserir o cliente com o código %d, pois este código já existe no arquivo de dados", cli->codCliente);
            free(cli);
            free(md);
            free(info);
            return;
        }

        nd = buscar_no_dados(info->p_f_dados, f_dados);

        if(nd == NULL){

        } 

        if(nd->m >= 4){ // Nó de dados cheio. 
            
            //O nó nd deve ser particionado em nd1 e nd2 
            NoDados *nd1 = no_dados();
            NoDados *nd2 = no_dados();
            Cliente ** vetor_de_clientes_ordenados = vetor_ordenado(nd->s, cli);
            for(int j = 0; j<5; j++){
                if(j<2){
                    inserir_cliente_em_no_dado(nd1, vetor_de_clientes_ordenados[j]);
                } else {
                    inserir_cliente_em_no_dado(nd2, vetor_de_clientes_ordenados[j]);
                }
            }
            free(vetor_de_clientes_ordenados);

            //salvando o primeiro nó no arquivo de dados
            nd1->ppai = nd->ppai;    
            fseek(f_dados, tamanho_no_dados() * info->p_f_dados, SEEK_SET);
            salva_no_dados(nd1, f_dados);

            //salvando o segundo nó no arquivo de dados
            nd2->ppai = nd->ppai;    
            fseek(f_dados, 0, SEEK_END);
            salva_no_dados(nd2, f_dados);
            int nd2_p_f_dados = (ftell(f_dados) / tamanho_no_dados()) - 1;

            // insercao no arquivo de indice 
            inserir_em_arquivo_de_indice(nd2->s[0]->codCliente, nd->ppai, 1, info->p_f_dados, nd2_p_f_dados, f_metadados, f_indice, f_dados);

            // liberando 
            libera_no_dados(nd1);
            libera_no_dados(nd2);

        } else { // Nó de dados tem espaço
            nd = inserir_cliente_em_no_dado(nd, cli);
            fseek(f_dados, tamanho_no_dados() * info->p_f_dados, SEEK_SET);
            salva_no_dados(nd, f_dados);
            libera_no_dados(nd);
        }
    }

    
    free(info);
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
        Info *a;
        inserir(cliente(30, "cli30"), fmd, fi, fd);
        inserir(cliente(40, "cli40"), fmd, fi, fd);
        inserir(cliente(10, "cli10"), fmd, fi, fd);
        inserir(cliente(20, "cli20"), fmd, fi, fd);
        inserir(cliente(15, "cli15"), fmd, fi, fd);
        inserir(cliente(19, "cli19"), fmd, fi, fd);
        inserir(cliente(18, "cli18"), fmd, fi, fd);
        inserir(cliente(17, "cli17"), fmd, fi, fd);
        inserir(cliente(50, "cli50"), fmd, fi, fd);
        inserir(cliente(16, "cli16"), fmd, fi, fd);
        inserir(cliente(60, "cli60"), fmd, fi, fd);

        // a = busca(1, fmd, fi, fd);
        // imprime_info(a);
        // free(a);
        // a = busca(5, fmd, fi, fd);
        // imprime_info(a);
        // free(a);
        // a = busca(3, fmd, fi, fd);
        // imprime_info(a);
        // free(a);
        
       
    
        ler_arquivo_de_dados(fd);
        ler_arquivo_de_indice(fi);
        ler_arquivo_de_metadados(fmd);

    }
    

    fclose(fmd);
    fclose(fi);
    fclose(fd);
    
    free(fmd);
    free(fi);
    free(fd);
    
    return 0;
}

