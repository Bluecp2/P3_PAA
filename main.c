#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rede_social.h"

int main(){
    RedeSocial *rede = carregarRedeDeArquivo("Entrada_rede1.txt");
    calculaMetrica(rede);
    imprimeMetrica(rede,"saida1.txt");
    apagarRede(rede);

    RedeSocial *rede1 = carregarRedeDeArquivo("Entrada_rede2.txt");
    calculaMetrica(rede1);
    imprimeMetrica(rede1,"saida2.txt");
    apagarRede(rede1);

    RedeSocial *rede2 = carregarRedeDeArquivo("Entrada_rede3.txt");
    calculaMetrica(rede2);
    imprimeMetrica(rede2,"saida3.txt");
    apagarRede(rede2);


    return 0;
}
