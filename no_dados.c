#include "no_dados.h"

// Imprime dados do nó
void imprime_no_dados(NoDados *no_dados){

    if(no_dados == NULL){ 
        printf("Nó de dados nulo\n");
        return;
    }
    
    printf("Número de dados m no no_dados: %d\n", no_dados->m);
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
        //no_dados->s[i] = NULL;   
        no_dados->s[i] = cliente(-1, "");
    } 

    return no_dados;
}

// Salva nó no arquivo out, na posicao atual do cursor
void salva_no_dados(NoDados *no_dados, FILE *out){
    /*
    fwrite(const void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fwrite(
        ponteiro para dados a serem gravados, 
        tamanho do item em bytes,
        máximo de itens a serem gravados,
        ponteiro para a estrutura file
    );
    */

    fwrite(&no_dados->m, sizeof(int), 1, out);
    fwrite(&no_dados->ppai, sizeof(int), 1, out);
    fwrite(&no_dados->pprox, sizeof(int), 1, out);
    for(int i=0; i<4; i++){

        if(no_dados->s[i] == NULL){
            printf("Nó de dados nulo. Não foi possível salvar. Abortar.\n");
            exit(1);
        } else {
           salva_cliente(no_dados->s[i], out);
        }

    }
}

// Le um no do arquivo in na posicao atual do cursor
// Retorna um ponteiro para no lido do arquivo
NoDados *le_no_dados(FILE *in){
    NoDados *no_dados = (NoDados*) malloc(sizeof(NoDados));

    /*
    fread(void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fread(
        local de armazenamento, 
        tamanho do item em bytes,
        numero maximo de itens a serem lidos, 
        ponteiro para estrutura file
    );
    */

    if(0 >= fread(&no_dados->m, sizeof(int), 1, in)){
        free(no_dados);
        return NULL;
    }

    fread(&no_dados->ppai, sizeof(int), 1, in);
    fread(&no_dados->pprox, sizeof(int), 1, in);
    for(int i=0; i<4; i++){
        no_dados->s[i] = le_cliente(in);
    }

    return no_dados;
}

// Busca o nó da posição no arquivo
NoDados * buscar_no_dados(int pos, FILE *in){
    // Calcula a posição correta do ponteiro
    fseek(in, tamanho_no_dados() * pos, SEEK_SET);

    // retorna o nó que está na posição encontrada
    return le_no_dados(in);
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