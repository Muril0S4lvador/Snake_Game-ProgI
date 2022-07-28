#include <stdio.h>

/*Defines do Programa*/

#define CABECA_LADOE '<'
#define CABECA_LADOD '>'
#define CABECA_CIMA '^'
#define CABECA_BAIXO 'v'
#define CONTINUE 'c'
#define MOVE_HORARIO 'h'
#define MOVE_ANTIHORARIO 'a'
#define ESPACO_VAZIO ' '
#define COMIDA '*'
#define DINHEIRO '$'
#define PAREDE '#'
#define MAX 101

/* Structs do Programa */

typedef struct{
    char cabeca;
    int tam;
    int linhaAnterior;
    int colunaAnterior;
    int linhaAtual;
    int colunaAtual;
} tCobra;

typedef struct{
    char mapa[MAX][MAX];
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
tJogo RealizaJogo(tJogo jogo);
tCobra MovimentaCobraHorario(tCobra cobra, char movimento);
tCobra MovimentaCobraAntiHorario(tCobra cobra, char movimento);
tCobra MovimentaCobraContinue(tCobra cobra, char movimento);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    jogo = InicializaJogo(jogo);
    jogo = RealizaJogo(jogo);

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

            if(caractere == CABECA_LADOD){

                /*armazena as coordenadas iniciais da cobra e a cabeca
                como comeco lendo a partir do 0, entao para ficar 
                certo preciso somar +1 em cada eixo*/
                jogo.cobra.colunaAtual = jogo.mapa.c;
                jogo.cobra.linhaAtual = jogo.mapa.l;

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

    fprintf(inicializacaof, "A cobra comecara o jogo na linha %d e coluna %d\n", jogo.cobra.linhaAtual+1, jogo.cobra.colunaAtual+1);

    fclose(inicializacaof);

    return jogo;
}

tJogo RealizaJogo(tJogo jogo){
    FILE *movimentos, *saida;
    char movimento; //variavel para auxiliar na leitura dos comandos de movimento

    movimentos = fopen("movimentos.txt", "r");
    saida = fopen("saida.txt", "w");

    while(fscanf(movimentos, "%c\n", &movimento) == 1){

        if(movimento == MOVE_HORARIO){

            //posicao da cobra atual vira anterior
            jogo.cobra.linhaAnterior = jogo.cobra.linhaAtual;
            jogo.cobra.colunaAnterior = jogo.cobra.colunaAtual;

            //move a cobra e faz a sua cabeca ser imprimida no local certo
            jogo.mapa.mapa[jogo.cobra.linhaAnterior][jogo.cobra.colunaAnterior] = ESPACO_VAZIO;
            jogo.cobra = MovimentaCobraHorario(jogo.cobra, movimento);
            jogo.mapa.mapa[jogo.cobra.linhaAtual][jogo.cobra.colunaAtual] = jogo.cobra.cabeca;

        } else if(movimento == MOVE_ANTIHORARIO){

            jogo.cobra.linhaAnterior = jogo.cobra.linhaAtual;
            jogo.cobra.colunaAnterior = jogo.cobra.colunaAtual;

            jogo.mapa.mapa[jogo.cobra.linhaAnterior][jogo.cobra.colunaAnterior] = ESPACO_VAZIO;
            jogo.cobra = MovimentaCobraAntiHorario(jogo.cobra, movimento);
            jogo.mapa.mapa[jogo.cobra.linhaAtual][jogo.cobra.colunaAtual] = jogo.cobra.cabeca;

        } else if(movimento == CONTINUE){

            jogo.cobra.linhaAnterior = jogo.cobra.linhaAtual;
            jogo.cobra.colunaAnterior = jogo.cobra.colunaAtual;

            jogo.mapa.mapa[jogo.cobra.linhaAnterior][jogo.cobra.colunaAnterior] = ESPACO_VAZIO;
            jogo.cobra = MovimentaCobraContinue(jogo.cobra, movimento);
            jogo.mapa.mapa[jogo.cobra.linhaAtual][jogo.cobra.colunaAtual] = jogo.cobra.cabeca;

        }

        //imprime mapa depois do movimento
        fprintf(saida, "\nEstado do jogo apos o movimento '%c':\n", movimento);
        for(jogo.mapa.l=0; jogo.mapa.l < jogo.mapa.linhas; jogo.mapa.l++){

            for(jogo.mapa.c=0; jogo.mapa.c < jogo.mapa.colunas; jogo.mapa.c++){

                fprintf(saida, "%c", jogo.mapa.mapa[jogo.mapa.l][jogo.mapa.c]);
            }

            fprintf(saida, "\n");
        }

        fprintf(saida, "Pontuacao: %d\n", 23); //ARRUMAR

    }

    fclose(movimentos);

    return jogo;
}

tCobra MovimentaCobraHorario(tCobra cobra, char movimento){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.colunaAtual--;
        cobra.cabeca = CABECA_LADOE;

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;
        
    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;
    }

    return cobra;
}

tCobra MovimentaCobraAntiHorario(tCobra cobra, char movimento){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual--;
        cobra.cabeca = CABECA_LADOE;
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;
    }

    return cobra;
}

tCobra MovimentaCobraContinue(tCobra cobra, char movimento){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.linhaAtual++;

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.linhaAtual--;

    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.colunaAtual++;

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.colunaAtual--;
    }

    return cobra;
}
