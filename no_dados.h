// Nós folha da Árvore B+. Cada nó comporta 4 Clientes.
typedef struct NoDados{
    int m;                  // Número de chaves armazenadas no nó
    int ppai;               // Ponteiro no arquivo para o nó pai
    int pprox;              // Ponteiro para a próxima página (Não foi utilizada)
    Cliente *s[4];          // Vetor de clientes s0, s1, s2, s3
} NoDados;

// Imprime dados do nó
void imprime_no_dados(NoDados *no_dados);

// Cria nó
NoDados *no_dados();

// Salva nó no arquivo out, na posicao atual do cursor
void salva_no_dados(NoDados *no_dados, FILE *out);

// Le um no do arquivo in na posicao atual do cursor
// Retorna um ponteiro para no lido do arquivo
NoDados *le_no_dados(FILE *in);

// Busca o nó da posição no arquivo
NoDados * buscar_no_dados(int pos, FILE *in);

// Retorna tamanho do no em bytes
int tamanho_no_dados();

// Libera o espaço de memória de no_dados
void libera_no_dados(NoDados* no_dados);

// Insere cliente no nó_dado
NoDados * inserir_cliente_em_no_dado(NoDados* no_dados, Cliente *cli);