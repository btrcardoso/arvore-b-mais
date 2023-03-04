#include "no.h"

// Imprime dados do nó
void imprime_no(No *no){

    if(no == NULL){ 
        printf("Nó nulo\n");
        return;
    }
    
    printf("Número de chaves m no nó: %d\n", no->m);
    printf("Página aponta para folha? %d\n", no->flag_aponta_folha);
    printf("Ponteiro (posição) do nó pai no arquivo: %d\n(", no->ppai);
    for(int i=0; i<4; i++){
        printf("p%d=%d), (s%d=%d, ", i, no->p[i], i, no->s[i]);
    }
    printf("p%d=%d)\n", 4, no->p[4]);
}

// Cria nó
No * no(){
    // aloca espaço para um bloco que caiba o No
    No *no = (No *) malloc(sizeof(No));

    // inicializa o espaço de memória com zeros
    if(no) memset(no, 0, sizeof(No));

    // copia os valores para os campos de n
    no->m = 0;
    no->flag_aponta_folha = 1;  
    no->ppai = -1;   
    for(int i=0; i<4; i++){
        no->p[i] = -1; 
        no->s[i] = -1;   
    }
    no->p[4] = -1;   

    return no;
}

// Salva nó no arquivo out, na posicao atual do cursor
void salva_no(No *no, FILE *out){
    /*
    fwrite(const void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fwrite(
        ponteiro para dados a serem gravados, 
        tamanho do item em bytes,
        máximo de itens a serem gravados,
        ponteiro para a estrutura file
    );
    */

    fwrite(&no->m, sizeof(int), 1, out);
    fwrite(&no->flag_aponta_folha, sizeof(int), 1, out);
    fwrite(&no->ppai, sizeof(int), 1, out);
    for(int i=0; i<4; i++){
        fwrite(&no->p[i], sizeof(int), 1, out); 
        fwrite(&no->s[i], sizeof(int), 1, out);   
    }
    fwrite(&no->p[4], sizeof(int), 1, out); 
}

// Le um no do arquivo in na posicao atual do cursor
// Retorna um ponteiro para no lido do arquivo
No *le_no(FILE *in){
    No *no = (No*) malloc(sizeof(No));

    /*
    fread(void *_Buffer, size_t _ElementSize, size_t _ElementCount, FILE *_Stream)
    fread(
        local de armazenamento, 
        tamanho do item em bytes,
        numero maximo de itens a serem lidos, 
        ponteiro para estrutura file
    );
    */

    if(0 >= fread(&no->m, sizeof(int), 1, in)){
        free(no);
        return NULL;
    }

    fread(&no->flag_aponta_folha, sizeof(int), 1, in);
    fread(&no->ppai, sizeof(int), 1, in);
    for(int i=0; i<4; i++){
        fread(&no->p[i], sizeof(int), 1, in); 
        fread(&no->s[i], sizeof(int), 1, in);   
    }
    fread(&no->p[4], sizeof(int), 1, in); 

    return no;
}

// Busca o nó da posição no arquivo
No * buscar_no(int pos, FILE *in){
    // Calcula a posição correta do ponteiro
    fseek(in, tamanho_no() * pos, SEEK_SET);

    // retorna o nó que está na posição encontrada
    return le_no(in);
}

// Retorna tamanho do no em bytes
int tamanho_no(){
    return sizeof(int)     // m
        + sizeof(int)      // flag_aponta_folha
        + sizeof(int)      // ppai
        + sizeof(int) * 4  // s[]
        + sizeof(int) * 5; // p[]
}

// libera o nó
void libera_no(No *no){
     // liberando espaços de memória ocupados pelos registros de cliente
    free(no->s);
    free(no->p);

    // liberando espaço de memória do nó
    free(no);
}


// Insere chave em nó
No * inserir_chave_em_no(No* no, int chave, int p_esq, int p_dir){
    int qtd = no->m;

    if(qtd >= 4){
        printf("Não é possível inserir a chave no nó, pois o nó está cheio.\n");
    
        // reorganização de acordo com o algoritmo da árvore b+
        exit(1);
    } else {
        no->s[qtd] = chave;                    // chave 
        if(p_esq != -1) no->p[qtd] = p_esq;    // ponteiro à esquerda da chave (só é colocada na primeira inserção)
        no->p[qtd+1] = p_dir;                  // ponteiro à direita da chave
        no->m += 1;

        // ordenação bolha
        int i, j, n = no->m;
        int aux_s, aux_p;

        for(i=n-1;i>0;i--){
        
            for(j=0;j<i;j++){
            
                if(no->s[j] > no->s[j+1]){
                    aux_s = no->s[j];
                    aux_p = no->p[j+1];

                    no->s[j] = no->s[j+1];
                    no->p[j+1] = no->p[j+2];

                    no->s[j+1] = aux_s;
                    no->p[j+2] = aux_p;

                }
                
            }

        } 

    }
    
    
    
    return NULL;
}