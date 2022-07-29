#include <stdio.h>

/*Defines do Programa*/

#define CABECA_LADOE '<'
#define CABECA_LADOD '>'
#define CABECA_CIMA '^'
#define CABECA_BAIXO 'v'
#define CORPO 'o'
#define CONTINUE 'c'
#define MOVE_HORARIO 'h'
#define MOVE_ANTIHORARIO 'a'
#define ESPACO_VAZIO ' '
#define COMIDA '*'
#define DINHEIRO '$'
#define PAREDE '#'
#define PORTAL '@'
#define MAX 100
#define NADA 5
#define CORPO_INT 1
#define PONTOS 2
#define MORTE 3
#define PORTAL_INT 4
#define GANHOU 1
#define PERDEU 0

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
    tCorpo corpos[MAX];
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
tCobra AumentaTamanho(tCobra cobra);

tMapa InicializaMapa();
tMapa DefineVazioAtras(tMapa mapa, tCobra cobra);
tMapa DefineCabeca(tMapa mapa, tCobra cobra);
tMapa DiminuiComidasMapa(tMapa mapa);
tMapa ImprimeMaisUmCorpo(tMapa mapa, tCobra cobra);
//tMapa ImprimeMapaDepoisDoMovimento();

void ImprimeInicializacao(tMapa mapa, tCobra cobra);
void ImprimeResultado(int num, FILE *saida);

int AtualizaJogo(tMapa mapa, tCobra cobra);
int DefineGanhouOuPerdeu(tMapa mapa);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    jogo = InicializaJogo(jogo);
    jogo = RealizaJogo(jogo);

    return 0;
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
    /* Essa funcao tem o intuitod emover a cobra e interpretar os caracteres do mapa */

    FILE *movimentos, *saida;
    char movimento; //variavel para auxiliar na leitura dos comandos de movimento
    int atualiza, ganhou; //variaveis p auxiliar na atualizacao da situacao do jogo

    movimentos = fopen("movimentos.txt", "r");
    saida = fopen("saida.txt", "w");

    while(fscanf(movimentos, "%c\n", &movimento) == 1){

        //posicao atual da cobra vira anterior
        jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);

        if(movimento == MOVE_HORARIO){

            //move a cobra e faz a sua cabeca ser imprimida no local certo
            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraHorario(jogo.cobra, movimento);

        } else if(movimento == MOVE_ANTIHORARIO){

            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraAntiHorario(jogo.cobra, movimento);

        } else if(movimento == CONTINUE){

            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);
            jogo.cobra = MovimentaCobraContinue(jogo.cobra, movimento);
        }

        //funcao para atualizar o jogo e as consequencias do movimento da cobra
        atualiza = AtualizaJogo(jogo.mapa, jogo.cobra);
        if( atualiza == NADA ){
            //moveSemPontos++

        } else if( atualiza == CORPO_INT ){
            jogo.cobra = AumentaTamanho(jogo.cobra);
            jogo.mapa = ImprimeMaisUmCorpo(jogo.mapa, jogo.cobra);
            jogo.mapa = DiminuiComidasMapa(jogo.mapa);
            //pontos++;

        } else if( atualiza == PONTOS ){
            //pontos += 10;

        } else if( atualiza == MORTE ){
            break;

        } else if( atualiza == PORTAL_INT ){
            //funcao portal

        }
        jogo.mapa = DefineCabeca(jogo.mapa, jogo.cobra);

        //imprime mapa depois do movimento CRIAR FUNCAO TMAPA MAS APENAS DPS DE RESOLVER A PONTUACAO
        fprintf(saida, "\nEstado do jogo apos o movimento '%c':\n", movimento);
        for(jogo.mapa.l=0; jogo.mapa.l < jogo.mapa.linhas; jogo.mapa.l++){

            for(jogo.mapa.c=0; jogo.mapa.c < jogo.mapa.colunas; jogo.mapa.c++){

                fprintf(saida, "%c", jogo.mapa.mapa[jogo.mapa.l][jogo.mapa.c]);
            }

            fprintf(saida, "\n");
        }
        //fprintf(saida, "Pontuacao: %d\n", 23);
        fprintf(saida, "Tamanho: %d\n", jogo.cobra.tamanho);
    }

    //CRIAR FUNCAO P IMPRIMIR SE GANHOU OU NAO
    ganhou = DefineGanhouOuPerdeu(jogo.mapa);
    ImprimeResultado(ganhou, saida);

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
        //movimentoEsquerda++

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;
        //movimentoDireita++
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;
        //movimentoBaixo++
        
    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;
        //movimentoCima++
    }

    return cobra;
}

tCobra MovimentaCobraAntiHorario(tCobra cobra, char movimento){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;
        //movimentoDireita++

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual--;
        cobra.cabeca = CABECA_LADOE;
        //movimentoEsquerda++
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;
        //movimentoCima++

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;
        //movimentoBaixo++
    }

    return cobra;
}

tCobra MovimentaCobraContinue(tCobra cobra, char movimento){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.linhaAtual++;
        //movimentoBaixo++

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.linhaAtual--;
        //movimentoCima++

    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.colunaAtual++;
        //movimentoDireita++

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.colunaAtual--;
        //movimentoEsquerda++
    }

    return cobra;
}

tCobra AumentaTamanho(tCobra cobra){
    cobra.tamanho++;

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

tMapa DiminuiComidasMapa(tMapa mapa){
    mapa.comidas--;

    return mapa;
}

tMapa ImprimeMaisUmCorpo(tMapa mapa, tCobra cobra){
    mapa.mapa[cobra.linhaAnterior][cobra.colunaAnterior] = CORPO;

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

void ImprimeResultado(int num, FILE *saida){
    if( num ){
        fprintf(saida, "Voce venceu!\nPontuacao final: %d\n", 24);
    } else{
        fprintf(saida, "Game over!\nPontuacao final: %d\n", 24);
    }
}

/* Funcoes int */

int AtualizaJogo(tMapa mapa, tCobra cobra){
    char atualiza; //variavel p auxiliar na atualizacao da situacao do jogo

    atualiza = mapa.mapa[cobra.linhaAtual][cobra.colunaAtual];

    if( atualiza == COMIDA )
        return 1;
    else if( atualiza == DINHEIRO )
        return 2;
    else if( atualiza == PAREDE )
        return 3;
    else if( atualiza == PORTAL )
        return 4;
    else
        return 5;
}

int DefineGanhouOuPerdeu(tMapa mapa){
    if( !mapa.comidas ){
        return 1;
    } else 
        return 0;
}
