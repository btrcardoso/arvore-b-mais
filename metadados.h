// Estrutura que armazena a posição do nó raíz e uma flag que diz se a raíz é folha ou não
typedef struct Metadados {
    int pont_raiz;           // ponteiro para a raiz da árvore
    int flag_raiz_folha;     // flag se a raíz é folha
} Metadados;

void iniciar_arquivo_metadados(FILE *out);

Metadados * le_metadados(FILE *in);

void imprime_metadados(Metadados *md);

void salva_metadados(Metadados *md, FILE *out);

void atualiza_arquivo_metadados(FILE *out, int pont_raiz, int flag_raiz_folha);