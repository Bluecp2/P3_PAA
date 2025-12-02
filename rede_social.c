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

