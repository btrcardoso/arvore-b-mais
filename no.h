// o nó aqui será lido como p0, (s0, p1), (s1, p2), (s2, p3), (s3, p4)

typedef struct No{
    int m;                  // Número de chaves armazenadas no nó
    int flag_aponta_folha;  // Flag booleano que diz se a página aponta para a folha
    int ppai;               // Ponteiro no arquivo para o nó pai
    int s[4];               // Vetor de chaves s0, s1, s2, s3
    int p[5];               // Vetor de ponteiros no arquivo para nós p0, p1, p2, p3, p4
} No;

// Imprime dados do nó
void imprime_no(No *no);

// Cria nó
No * no();

// Salva nó no arquivo out, na posicao atual do cursor
void salva_no(No *no, FILE *out);

// Le um no do arquivo in na posicao atual do cursor
// Retorna um ponteiro para no lido do arquivo
No *le_no(FILE *in);

// Busca o nó da posição no arquivo
No * buscar_no(int pos, FILE *in);

// Retorna tamanho do no em bytes
int tamanho_no();

