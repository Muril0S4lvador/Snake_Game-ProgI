#include <stdio.h>

/* Structs do Programa */

typedef struct{
    char cabeca;
    char corpo;
    int tam;
    int ilinha;
    int icoluna;
} tCobra;

typedef struct{
    char mapa[101][101];
    int linhas;
    int colunas;
    int l;
    int c;
} tMapa;

typedef struct{
    tCobra cobra;
    tMapa mapa;
} tJogo;

/*Inicializacao das Funcoes do Programa*/

tJogo InicializaJogo(tJogo jogo);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    jogo = InicializaJogo(jogo);
}

/* Codigo das Funcoes */

tJogo InicializaJogo(tJogo jogo){
    FILE *mapaf, *inicializacaof;

    mapaf = fopen("mapa.txt", "r");
    inicializacaof = fopen("inicializacao.txt", "w");

    fscanf(mapaf, "%d %d\n", &jogo.mapa.linhas, &jogo.mapa.colunas);

    //criei uma variavel char para auxiliar a leitura do mapa
    char caractere;

    //le o mapa
    for(jogo.mapa.l=0; jogo.mapa.l <= jogo.mapa.linhas; jogo.mapa.l++){
        
        for(jogo.mapa.c=0; jogo.mapa.c <= jogo.mapa.colunas; jogo.mapa.c++){

            fscanf(mapaf, "%c", &caractere);
            jogo.mapa.mapa[jogo.mapa.l][jogo.mapa.c] = caractere;

            if(caractere == '>'){

                /*armazena as coordenadas iniciais da cobra e a cabeca
                como comeco lendo a partir do 0, entao para ficar 
                certo preciso somar +1 em cada eixo*/
                jogo.cobra.icoluna = jogo.mapa.c+1;
                jogo.cobra.ilinha = jogo.mapa.l+1;

                jogo.cobra.cabeca = caractere;
            }
        }
    }
    
    fclose(mapaf);

    //imprime o mapa no arquivo de inicializacao, juntamente com as coordenadas iniciais da cobra
    for(jogo.mapa.l=0; jogo.mapa.l < jogo.mapa.linhas; jogo.mapa.l++){

        for(jogo.mapa.c=0; jogo.mapa.c < jogo.mapa.colunas; jogo.mapa.c++){

            fprintf(inicializacaof, "%c", jogo.mapa.mapa[jogo.mapa.l][jogo.mapa.c]);
        }

        fprintf(inicializacaof, "\n");
    }

    fprintf(inicializacaof, "A cobra comecara o jogo na linha %d e coluna %d\n", jogo.cobra.ilinha, jogo.cobra.icoluna);

    fclose(inicializacaof);

    return jogo;
}

