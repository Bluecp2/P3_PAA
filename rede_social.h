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
    int conexao;// quem influencia
    int destino;//quem e influenciado
    float peso;
}Conexao;

typedef struct{
    int maxPessoas, qtdPessoas;
    int maxConexoes, qtdConexoes;
    Pessoa *pessoas;
    Conexao *matriz;
} RedeSocial;

#endif