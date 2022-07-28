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
#define MAX 100

/* Structs do Programa */

typedef struct{
    char corpo;
    int colunaAtual;
    int linhaAtual;
    int colunaAnterior;
    int linhaAnterior;
} tCorpo;

typedef struct{
    char cabeca;
    int tamanho;
    int linhaAnterior;
    int colunaAnterior;
    int linhaAtual;
    int colunaAtual;
    tCorpo corpo[MAX];
} tCobra;

typedef struct{
    int pontuacao;
    int qtdmovimentos;
    int moveSemPonto;
    int moveCima;
    int moveBaixo;
    int moveEsq;
    int moveDir;
} tEstatisticas;

typedef struct{
    char mapa[MAX][MAX];
    int linhas;
    int colunas;
    int l;
    int c;
    int comidas;
} tMapa;

typedef struct{
    tCobra cobra;
    tMapa mapa;
} tJogo;

/*Inicializacao das Funcoes do Programa*/

tJogo InicializaJogo(tJogo jogo);
tJogo RealizaJogo(tJogo jogo);

tCobra InicializaCobra(tMapa mapa);
tCobra AlteraPosicaoAnterior(tCobra cobra);
tCobra MovimentaCobraHorario(tCobra cobra, char movimento);
tCobra MovimentaCobraAntiHorario(tCobra cobra, char movimento);
tCobra MovimentaCobraContinue(tCobra cobra, char movimento);

tMapa InicializaMapa();
tMapa DefineVazioAtras(tMapa mapa, tCobra cobra);
tMapa DefineCabeca(tMapa mapa, tCobra cobra);
//tMapa ImprimeMapaDepoisDoMovimento();


void ImprimeInicializacao(tMapa mapa, tCobra cobra);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    jogo = InicializaJogo(jogo);
    jogo = RealizaJogo(jogo);

}

/* Codigo das Funcoes */

/* Funcoes tJogo */

tJogo InicializaJogo(tJogo jogo){

    jogo.mapa = InicializaMapa();
    jogo.cobra = InicializaCobra(jogo.mapa);
    ImprimeInicializacao(jogo.mapa, jogo.cobra);

    return jogo;
}

tJogo RealizaJogo(tJogo jogo){
    FILE *movimentos, *saida;
    char movimento; //variavel para auxiliar na leitura dos comandos de movimento

    movimentos = fopen("movimentos.txt", "r");
    saida = fopen("saida.txt", "w");

    while(fscanf(movimentos, "%c\n", &movimento) == 1){

        if(movimento == MOVE_HORARIO){

            //posicao atual da cobra vira anterior
            jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);

            //move a cobra e faz a sua cabeca ser imprimida no local certo
            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraHorario(jogo.cobra, movimento);
            jogo.mapa = DefineCabeca(jogo.mapa, jogo.cobra);

        } else if(movimento == MOVE_ANTIHORARIO){

            jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);

            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraAntiHorario(jogo.cobra, movimento);
            jogo.mapa = DefineCabeca(jogo.mapa, jogo.cobra);

        } else if(movimento == CONTINUE){

            jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);

            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraContinue(jogo.cobra, movimento);
            jogo.mapa = DefineCabeca(jogo.mapa, jogo.cobra);

        }

        //imprime mapa depois do movimento CRIAR FUNCAO TMAPA MAS APENAS DPS DE RESOLVER A PONTUACAO
        fprintf(saida, "\nEstado do jogo apos o movimento '%c':\n", movimento);
        for(jogo.mapa.l=0; jogo.mapa.l < jogo.mapa.linhas; jogo.mapa.l++){

            for(jogo.mapa.c=0; jogo.mapa.c < jogo.mapa.colunas; jogo.mapa.c++){

                fprintf(saida, "%c", jogo.mapa.mapa[jogo.mapa.l][jogo.mapa.c]);
            }

            fprintf(saida, "\n");
        }

        fprintf(saida, "Pontuacao: %d\n", 23);

    }

    fclose(movimentos);

    return jogo;
}

/* Funcoes tCobra */

tCobra InicializaCobra(tMapa mapa){
    tCobra cobra;

    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            if(mapa.mapa[mapa.l][mapa.c] == CABECA_LADOD){

                //define o local inicial da cobra
                cobra.linhaAtual = mapa.l;
                cobra.colunaAtual = mapa.c;

                cobra.tamanho = 0;
                cobra.cabeca = CABECA_LADOD;
            }
        }
    }

    return cobra;
}

tCobra AlteraPosicaoAnterior(tCobra cobra){

    //posicao atual da cobra vira anterior
    cobra.linhaAnterior = cobra.linhaAtual;
    cobra.colunaAnterior = cobra.colunaAtual;

    return cobra;
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

/* Funcoes tMapa */

tMapa InicializaMapa(){
    FILE *mapaf;
    tMapa mapa;

    mapaf = fopen("mapa.txt", "r");

    //define informacoes importantes do mapa
    fscanf(mapaf, "%d %d\n", &mapa.linhas, &mapa.colunas);
    mapa.comidas = 0;

    //criei uma variavel char para auxiliar a leitura do mapa
    char caractere;

    //le o mapa
    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            fscanf(mapaf, "%c", &caractere);
            mapa.mapa[mapa.l][mapa.c] = caractere;

            //contabiliza o numero de comidas no mapa
            if(caractere == COMIDA){
                mapa.comidas++;
            }
        }
    }
    
    fclose(mapaf);

    return mapa;
}

tMapa DefineVazioAtras(tMapa mapa, tCobra cobra){
    //limpa a cabeca da cobra na posicao anterior
    mapa.mapa[cobra.linhaAnterior][cobra.colunaAnterior] = ESPACO_VAZIO;

    return mapa;
}

tMapa DefineCabeca(tMapa mapa, tCobra cobra){
    //faz com que a cabeca da cobra seja imprimida na posicao atual
    mapa.mapa[cobra.linhaAtual][cobra.colunaAtual] = cobra.cabeca;

    return mapa;
}

/* Funcoes void */

void ImprimeInicializacao(tMapa mapa, tCobra cobra){
    FILE *inicializacaof;

    inicializacaof = fopen("inicializacao.txt", "w");

    for(mapa.l=0; mapa.l < mapa.linhas; mapa.l++){

        for(mapa.c=0; mapa.c < mapa.colunas; mapa.c++){

            fprintf(inicializacaof, "%c", mapa.mapa[mapa.l][mapa.c]);
        }

        fprintf(inicializacaof, "\n");
    }

    fprintf(inicializacaof, "A cobra comecara o jogo na linha %d e coluna %d\n", cobra.linhaAtual+1, cobra.colunaAtual+1);
                                                    //como comeco lendo a partir do 0, pra imprimir corretamente preciso adicionar +1

    fprintf(inicializacaof, "\n\n qtd comidas: %d  Tamanho da cobra:%d\n", mapa.comidas, cobra.tamanho);
    
    fclose(inicializacaof);
}
