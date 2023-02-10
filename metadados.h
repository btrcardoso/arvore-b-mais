typedef struct Metadados {
    int pont_raiz;
    int flag_raiz_folha;
} Metadados;

void iniciar_arquivo_metadados(FILE *out);

Metadados * le_metadados(FILE *in);

void imprime_metadados(Metadados *md);