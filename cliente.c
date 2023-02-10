#include "cliente.h"

// Imprime dados do cliente
void imprime_cliente(Cliente *cli){
    printf("Codigo: %d\n", cli->codCliente);
    printf("Nome: %s\n", cli->nome);
}

// Cria cliente
Cliente * cliente(int codCliente, char *nome){
    // aloca espaço para um bloco que caiba o Cliente
    Cliente *cli = (Cliente *) malloc(sizeof(Cliente));

    // inicializa o espaço de memória com zeros
    if(cli) memset(cli, 0, sizeof(Cliente));

    // copia os valores para os campos de cli
    cli->codCliente = codCliente;
    strcpy(cli->nome, nome);

    return cli;
}

// Salva cliente no arquivo out, na posicao atual do cursor
void salva_cliente(Cliente *cli, FILE *out){
    /*
    fwrite(const void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fwrite(
        ponteiro para dados a serem gravados, 
        tamanho do item em bytes,
        máximo de itens a serem gravados,
        ponteiro para a estrutura file
    );
    */

    fwrite(&cli->codCliente, sizeof(int), 1, out);
    fwrite(cli->nome, sizeof(char), sizeof(cli->nome), out);
}

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
Cliente *le_cliente(FILE *in){
    Cliente *cli = (Cliente*) malloc(sizeof(Cliente));

    /*
    fread(void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fread(
        local de armazenamento, 
        tamanho do item em bytes,
        numero maximo de itens a serem lidos, 
        ponteiro para estrutura file
    );
    */

    if(0 >= fread(&cli->codCliente, sizeof(int), 1, in)){
        free(cli);
        return NULL;
    }

    fread(cli->nome, sizeof(char), sizeof(cli->nome), in);

    return cli;
}

int tamanho_cliente(){
    return sizeof(int)          // codCliente
         + sizeof(char) * 100;  // nome
}