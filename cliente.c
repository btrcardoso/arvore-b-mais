#include "cliente.h"
#include <string.h>

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

int tamanho_cliente(){
    return sizeof(int)          // codCliente
         + sizeof(char) * 100;  // nome
}