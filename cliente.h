#include <stdlib.h>
#include <stdio.h>

typedef struct Cliente {
    int codCliente;
    char nome[100];
} Cliente;

// Imprime dados do cliente
void imprime_cliente(Cliente *cli);

// Cria cliente
Cliente * cliente(int codCliente, char *nome);

// Retorna tamanho do cliente em bytes
int tamanho_cliente();
