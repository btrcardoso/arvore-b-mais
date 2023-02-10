typedef struct Cliente {
    int codCliente;
    char nome[100];
} Cliente;

// Imprime dados do cliente
void imprime_cliente(Cliente *cli);

// Cria cliente
Cliente * cliente(int codCliente, char *nome);

// Salva cliente no arquivo out, na posicao atual do cursor
void salva_cliente(Cliente *cli, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
Cliente *le_cliente(FILE *in);

// Retorna tamanho do cliente em bytes
int tamanho_cliente();
