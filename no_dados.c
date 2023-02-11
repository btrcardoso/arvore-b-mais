#include "no_dados.h"

// Imprime dados do nó
void imprime_no_dados(NoDados *no_dados){

    if(no_dados == NULL){ 
        printf("Nó de dados nulo\n");
        return;
    }
    
    printf("Número de chaves m no no_dados: %d\n", no_dados->m);
    printf("Ponteiro (posição) do nó pai no_dados arquivo: %d\n", no_dados->ppai);
    for(int i=0; i<4; i++){
        imprime_cliente(no_dados->s[i]);
    }
    
}

// Cria nó
NoDados *no_dados(){

    // aloca espaço para um bloco que caiba o NoDados
    NoDados *no_dados = (NoDados*) malloc(sizeof(NoDados));

    // inicializa o espaço de memória com zeros
    if(no_dados) memset(no_dados, 0, sizeof(NoDados));

    // copia os valores para os campos de no_dados
    no_dados->m = 0;
    no_dados->ppai = -1;  
    no_dados->pprox = -1; 
    for(int i=0; i<4; i++){
        no_dados->s[i] = NULL;   
    } 

    return no_dados;
}

// Salva nó no arquivo out, na posicao atual do cursor
void salva_no_dados(NoDados *no_dados, FILE *out){
    
}

// Le um no do arquivo in na posicao atual do cursor
// Retorna um ponteiro para no lido do arquivo
NoDados *le_no_dados(FILE *in){
    
}

// Busca o nó da posição no arquivo
NoDados * buscar_no_dados(int pos, FILE *in){
    
}

// Retorna tamanho do no em bytes
int tamanho_no_dados(){
    return sizeof(int) +        // m
        sizeof(int) +           // ppai                 
        sizeof(int) +           // pprox
        tamanho_cliente() * 4;  // s[]
}

// Libera o espaço de memória de no_dados
void libera_no_dados(NoDados* no_dados){
    // liberando espaços de memória ocupados pelos registros de cliente
    for(int i=0; i<4; i++){
        free(no_dados->s[i]);
    }

    // liberando espaço de memória do nó
    free(no_dados);
}