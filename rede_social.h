#ifndef REDE_SOCIAL_H
#define REDE_SOCIAL_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
    int id, idade;
    char nome[100];

    float influencia;
    float influenciado;   
}Pessoa;

typedef struct {
    int origem;// quem influencia
    int destino;//quem e influenciado
    float peso;
}Conexao;

typedef struct{
    int maxPessoas, qtdPessoas;
    int maxConexoes, qtdConexoes;
    Pessoa *pessoas;
    Conexao *matriz;
} RedeSocial;

RedeSocial* criaRede(int maxPessoas, int maxConexoes);
void apagarRede(RedeSocial *rede);
void adicionarPessoa(RedeSocial *rede, char *nome, int idade);
void adicionarInfluencia(RedeSocial *rede,int idOrigem, int idDestino, float peso);

void calculaMetrica(RedeSocial *rede);
void imprimeMetrica(RedeSocial *rede);
#endif