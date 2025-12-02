#include "rede_social.h"

RedeSocial* criaRede(int maxPessoas, int maxConexoes){
    RedeSocial *rede = (RedeSocial*)malloc(sizeof(RedeSocial));
    rede->maxPessoas = maxPessoas;
    rede->pessoas = (Pessoa*)malloc(maxPessoas * sizeof(Pessoa));
    rede->qtdPessoas = 0;

    rede->maxConexoes = maxConexoes;
    rede->matriz = (Conexao*) malloc(maxConexoes * sizeof(Conexao));
    rede->qtdConexoes = 0;

    return rede;
}

void apagarRede(RedeSocial *rede){
    if (rede != NULL){
        if(rede->pessoas != NULL)
            free(rede->pessoas);
        if(rede->matriz != NULL)
            free(rede->matriz);
        free(rede);
    }
}

void adicionarPessoa(RedeSocial *rede, char *nome, int idade){
    if(rede->qtdPessoas >= rede->maxPessoas){
        int novacap = rede->maxPessoas * 2;
        Pessoa *tempPessoas = (Pessoa*)realloc(rede->pessoas, novacap * sizeof(Pessoa));
        
        if(tempPessoas == NULL){
            printf("Erro Fatal: Falha ao realocar memoria para mais pessoas.\n");
            return;
        }

        rede->pessoas = tempPessoas;
        rede->maxPessoas = novacap;
        printf("Aviso: Vetor de pessoas redimensionado para %d posicoes.\n", novacap);
    }
    
    int indice = rede->qtdPessoas;
    rede->pessoas[indice].id = indice;
    rede->pessoas[indice].idade = idade;
    strncpy(rede->pessoas[indice].nome, nome, 99);
    rede->pessoas[indice].nome[99] = '\0';
    rede->pessoas[indice].influencia = 0.0;
    rede->pessoas[indice].influenciado = 0.0;

    rede->qtdPessoas++;
}

void adicionarInfluencia(RedeSocial *rede,int idOrigem, int idDestino, float peso){
    if (rede->qtdConexoes >= rede->maxConexoes){
        int novacap = rede->maxConexoes*2;
        Conexao *tempMatriz = (Conexao*)realloc(rede->matriz, novacap * sizeof(Conexao));

        if(tempMatriz == NULL){
            printf("Erro Fatal: Falha ao realocar memoria para mais conexoes.\n");
            return;
        }
        rede->matriz = tempMatriz;
        rede->maxConexoes = novacap;
        printf("Aviso: Matriz esparsa redimensionada para %d arestas.\n", novacap);
    }
    
    int indice = rede->qtdConexoes;
    rede->matriz[indice].origem = idOrigem;
    rede->matriz[indice].destino = idDestino;
    rede->matriz[indice].peso = peso;

    rede->qtdConexoes++;
}

void calculaMetrica(RedeSocial *rede){
    for (int i = 0; i <  rede->qtdPessoas; i++){
        rede->pessoas[i].influencia = 0.0;
        rede->pessoas[i].influenciado = 0.0;
    }
    
    for (int i = 0; i <  rede->qtdConexoes; i++){
        int A = rede->matriz[i].origem;
        int B = rede->matriz[i].destino;
        float peso1 = rede->matriz[i].peso;

        rede->pessoas[A].influencia += peso1;
        rede->pessoas[B].influenciado += peso1;

        for (int j = 0; j < rede->qtdConexoes; j++)
        {
            if(rede->matriz[j].origem == B){
                float peso2 = rede->matriz[j].peso;
                float influenciaIndireta = peso1 * peso2;

                int C = rede->matriz[j].destino;
                rede->pessoas[A].influencia += influenciaIndireta;
                rede->pessoas[C].influenciado += influenciaIndireta;
            }
        }
    }
}

int compara(const void *a, const void *b){
    Pessoa *p1 = (Pessoa*)a;
    Pessoa *p2 = (Pessoa*)b;

    if(p1->influencia < p2->influencia) return 1;
    if(p1->influencia > p2->influencia) return -1;
    return 0;
}

void imprimeMetrica(RedeSocial *rede, char *nomeArquivo){
    FILE *arquivo = fopen(nomeArquivo, "w");
    
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel criar o arquivo %s\n", nomeArquivo);
        return;
    }

    if (rede->qtdPessoas == 0) {
        fprintf(arquivo, "Rede vazia.\n");
        fclose(arquivo);
        return;
    }
    
    Pessoa *cop = (Pessoa*)malloc(rede->qtdPessoas * sizeof(Pessoa));
    memcpy(cop, rede->pessoas, rede->qtdPessoas * sizeof(Pessoa));
    qsort(cop, rede->qtdPessoas, sizeof(Pessoa), compara);

    fprintf(arquivo, "=== RELATORIO DE INFLUENCIA ===\n\n");
    for (int i = 0; i < rede->qtdPessoas; i++){
        fprintf(arquivo,"%d. %s (Score: %.2f)\n", i+1, cop[i].nome, cop[i].influencia);
    }
    fprintf(arquivo, "--- TOP 3 MAIS INFLUENTES ---\n");
    int limit = (rede->qtdPessoas < 3) ? rede->qtdPessoas : 3;
    for (int i = 0; i < limit; i++){
        fprintf(arquivo,"%d. %s (Score: %.2f)\n", i+1, cop[i].nome, cop[i].influencia);
    }
    
    int idMenor = 0;
    float menorValor = rede->pessoas[0].influenciado;

    for(int i = 1; i < rede->qtdPessoas; i++){
        if(rede->pessoas[i].influenciado < menorValor){
            menorValor = rede->pessoas[i].influenciado;
            idMenor = i;
        }
    }

    fprintf(arquivo, "\n--- PESSOA MENOS INFLUENCIAVEL ---\n");
    fprintf(arquivo, "Nome: %s (Score Recebido: %.2f)\n", 
            rede->pessoas[idMenor].nome, menorValor);
    free(cop);
    fclose(arquivo);
}

RedeSocial* carregarRedeDeArquivo(char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o arquivo '%s'.\n", nomeArquivo);
        return NULL;
    }
  
    int qtdPessoas;
    fscanf(arquivo, "%d", &qtdPessoas); 
    RedeSocial *rede = criarRede(qtdPessoas, qtdPessoas * 2);

    char nomeTemp[100];
    int idadeTemp;

    for (int i = 0; i < qtdPessoas; i++) {
        fscanf(arquivo, "%s %d", nomeTemp, &idadeTemp);
        adicionarPessoa(rede, nomeTemp, idadeTemp);
    }

    int qtdArestas;
    fscanf(arquivo, "%d", &qtdArestas); 

    int origem, destino;
    float peso;

    for (int i = 0; i < qtdArestas; i++) {
        fscanf(arquivo, "%d %d %f", &origem, &destino, &peso);
        adicionarInfluencia(rede, origem, destino, peso);
    }

    fclose(arquivo);
    return rede;
}