#include <stdio.h>

/*Defines do Programa*/

// Define o a cobra e seu corpo
#define CABECA_LADOE '<'
#define CABECA_LADOD '>'
#define CABECA_CIMA '^'
#define CABECA_BAIXO 'v'
#define CORPO 'o'
#define CORPO_MORTO 'X'

//define os movimentos da cobra
#define CONTINUE 'c'
#define MOVE_HORARIO 'h'
#define MOVE_ANTIHORARIO 'a'

//define os caracteres do mapa
#define ESPACO_VAZIO ' '
#define COMIDA '*'
#define DINHEIRO '$'
#define PAREDE '#'
#define TUNEL  '@'

//define o tamanho maximo permitido
#define MAX 100

//define ints para controle de cada rodada do jogo
#define NADA 0
#define CORPO_INT 1
#define PONTOS 2
#define MORTE 3
#define PORTAL_INT 4
#define DIREITA 5
#define ESQUERDA 6
#define CIMA 7
#define BAIXO 8
#define MOVIMENTOS 9
#define MOVESEMPONTOS 10

//ints para auxiliar na leitura se o jogo venceu ou nao
#define GANHOU 1
#define PERDEU 0

/* Structs do Programa */

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
    int linha;
    int coluna;
    int vezes;
} tPosicoes;

typedef struct{
    int mapa[MAX][MAX];
} tHeatmap;


typedef struct{
    int colunaAtual;
    int linhaAtual;
    int colunaAnterior;
    int linhaAnterior;
} tCorpo;

typedef struct{
    char cabeca;
    char corpoChar;
    char movimento;
    int tamanho;
    int linhaAnterior;
    int colunaAnterior;
    int linhaAtual;
    int colunaAtual;
    int morta;
    tCorpo corpos[MAX];
    tEstatisticas estatisticas;
    tPosicoes posicoes[MAX*MAX];

} tCobra;

typedef struct{
    int linha;
    int coluna;
} tTunel;

typedef struct{
    char mapa[MAX][MAX];
    int linhas;
    int colunas;
    int l;
    int c;
    int comidas;
    tTunel tunel1;
    tTunel tunel2;
} tMapa;

typedef struct{
    tCobra cobra;
    tMapa mapa;
    tHeatmap heatmap;
} tJogo;

/*Inicializacao das Funcoes do Programa*/

tJogo InicializaJogo(tJogo jogo);
tJogo RealizaJogo(tJogo jogo);
tJogo AtualizaJogo(tJogo jogo, FILE *resumo);

tCobra InicializaCobra(tMapa mapa);
tCobra DefineCharMovimento(tCobra cobra, char movimento);
tCobra AlteraPosicaoAnterior(tCobra cobra);
tCobra MovimentaCobra(tMapa mapa, tCobra cobra);
tCobra MovimentaCobraHorario(tCobra cobra);
tCobra MovimentaCobraAntiHorario(tCobra cobra);
tCobra MovimentaCobraContinue(tCobra cobra);
tCobra MovimentaCorpo(tCobra cobra);
tCobra AumentaTamanho(tMapa mapa, tCobra cobra);
tCobra AtualizaEstatisticas(tCobra cobra, int id);
tCobra MataACobra(tCobra cobra);
tCobra AtravessaParede(tMapa mapa, tCobra cobra);
tCobra AtravessaTunel(tMapa mapa, tCobra cobra);
tCobra InicializaPosicoesDaCobra(tMapa mapa, tCobra cobra);
tCobra AtualizaPosicoesDaCobra(tMapa mapa, tCobra cobra);
tCobra OrdenaCrescentePosicoes(tCobra cobra, int tam);
tCobra TrocaVetorPosicaoAComB(tCobra cobra, int a, int b);

tCorpo InicializaCorpo(tCobra cobra);
tCorpo InicializaCorpoAtrasDeCorpo(tCorpo corpo);
tCorpo AlteraPosicaoAnteriorDeCorpos(tCorpo corpo);
tCorpo MoveCorpoAtrasDeCabeca(tCobra cobra, tCorpo corpo);
tCorpo MoveCorpoAtrasDeCorpo(tCorpo anterior, tCorpo corpo);

tMapa InicializaMapa();
tMapa DefineVazioAtras(tMapa mapa, tCobra cobra);
tMapa DefineVazioAtrasDeCorpo(tMapa mapa, tCorpo corpo);
tMapa DefineCabecaECorpo(tMapa mapa, tCobra cobra);
tMapa DiminuiComidasMapa(tMapa mapa);
tMapa DefineMaisUmCorpo(tMapa mapa, tCobra cobra);
tMapa DefineMaisUmCorpoAtrasDeCorpo(tMapa mapa, tCorpo corpo, tCobra cobra);
tMapa DefinePosicaoCorpo(tMapa mapa, tCobra cobra);
tMapa DefinePosicaoCorpoAtrasDeCabeca(tMapa mapa, tCobra cobra, tCorpo corpo);
tMapa DefinePosicaoCorpoAtrasDeCorpo(tMapa mapa, tCorpo corpoAnterior, tCorpo corpo, tCobra cobra);

tTunel InicializaTunel( int l, int c );
tTunel IdentificaTunelSaida( int l, int c, tTunel tunel1, tTunel tunel2);

tEstatisticas InicializaEstatisticas(tEstatisticas estatistica);
tEstatisticas DefineEstatisticas(tEstatisticas estatistica, int id);

tPosicoes InicializaPosicao(int linha, int coluna);
tPosicoes AtualizaPosicao(tPosicoes posicao, int linha, int coluna);

void ImprimeInicializacao(tMapa mapa, tCobra cobra);
void ImprimeResultado(int num, FILE *saida, tCobra cobra);
void ImprimeMapaDepoisDoMovimento(tMapa mapa, tCobra cobra, FILE *saida);
void ImprimeEstatisticas(tCobra cobra);
void Estatisticas(tEstatisticas estatisticas, FILE *estatisticasf);
void ImprimeResumo(tMapa mapa, tCobra cobra, int id, FILE *resumo);
void ImprimePosicoes(tMapa mapa, tCobra cobra);
void ImprimePosicao(tPosicoes posicao, FILE *ranking);
void ImprimeHeatMap(tMapa mapa, tCobra cobra);

int IdentificaCaraceterSucessor(tMapa mapa, tCobra cobra);
int DefineGanhouOuPerdeu(tMapa mapa);
int ObtemPontuacao(tEstatisticas estatisticas);
int ObtemNumeroDoMovimento(tEstatisticas estatisticas);
int ObtemQuantidadeDeMovimentos(tEstatisticas estatisticas);
int CobraMortaOuNao(tCobra cobra);
int ObtemLinhaPosicao(tPosicoes posicao);
int ObtemColunaPosicao(tPosicoes posicao);
int ObtemFrequenciaPosicao(tPosicoes posicao);

char ObtemCharDoMovimento(tCobra cobra);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    jogo = InicializaJogo(jogo);
    jogo = RealizaJogo(jogo);
    ImprimeEstatisticas(jogo.cobra);

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
    /* Essa funcao tem o intuito de mover a cobra e interpretar os caracteres do mapa */

    FILE *movimentos, *saida, *resumo;
    char movimento; //variavel para auxiliar na leitura dos comandos de movimento
    int ganhou, morta; //variaveis p auxiliar na atualizacao da situacao do jogo

    movimentos = fopen("movimentos.txt", "r");
    saida = fopen("saida.txt", "w");
    resumo = fopen("resumo.txt", "w");

    while(fscanf(movimentos, "%c\n", &movimento) == 1){
        jogo.cobra = DefineCharMovimento(jogo.cobra, movimento);
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVIMENTOS);

        //posicao atual da cobra vira anterior
        jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);
        jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);

        jogo.cobra = MovimentaCobra( jogo.mapa, jogo.cobra );

        jogo = AtualizaJogo(jogo, resumo);

        //imprime mapa depois do movimento
        fprintf(saida, "\nEstado do jogo apos o movimento '%c':\n", movimento);
        ImprimeMapaDepoisDoMovimento(jogo.mapa, jogo.cobra, saida);

        morta = CobraMortaOuNao(jogo.cobra);
        if( morta )
            break;

        ganhou = DefineGanhouOuPerdeu(jogo.mapa);
        if( ganhou )
            break;
    }

    //informa se ganhou ou perdeu o jogo
    ImprimeResultado(ganhou, saida, jogo.cobra);
    ImprimePosicoes(jogo.mapa, jogo.cobra);
    ImprimeHeatMap(jogo.mapa, jogo.cobra);

    fclose(movimentos);
    fclose(resumo);
    fclose(saida);

    return jogo;
}

tJogo AtualizaJogo(tJogo jogo, FILE *resumo){
    int atualiza;

    //funcao para atualizar o jogo e as consequencias do movimento da cobra
    atualiza = IdentificaCaraceterSucessor(jogo.mapa, jogo.cobra);

    if( !atualiza ){
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVESEMPONTOS);

    } else if( atualiza == CORPO_INT ){
        jogo.cobra = AumentaTamanho(jogo.mapa, jogo.cobra);
        jogo.mapa = DefineMaisUmCorpo(jogo.mapa, jogo.cobra);
        jogo.mapa = DiminuiComidasMapa(jogo.mapa);

        jogo.cobra = AtualizaEstatisticas(jogo.cobra, CORPO_INT);

        ImprimeResumo(jogo.mapa, jogo.cobra, CORPO_INT, resumo);

    } else if( atualiza == PONTOS ){
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, PONTOS);

        ImprimeResumo(jogo.mapa, jogo.cobra, PONTOS, resumo);

    } else if( atualiza == MORTE ){
        jogo.cobra = MataACobra(jogo.cobra);
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVESEMPONTOS);

        ImprimeResumo(jogo.mapa, jogo.cobra, MORTE, resumo);

    } else if( atualiza == PORTAL_INT ){
        jogo.cobra = AtravessaTunel(jogo.mapa, jogo.cobra);

        jogo = AtualizaJogo(jogo, resumo);

    }
    jogo.mapa = DefineCabecaECorpo(jogo.mapa, jogo.cobra);

    return jogo;
}

/* Funcoes tCobra */

tCobra InicializaCobra(tMapa mapa){
    tCobra cobra;

    cobra = InicializaPosicoesDaCobra( mapa, cobra );

    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            if(mapa.mapa[mapa.l][mapa.c] == CABECA_LADOD){

                //define o local inicial da cobra
                cobra.linhaAtual = mapa.l;
                cobra.colunaAtual = mapa.c;

                cobra.tamanho = 0;
                cobra.morta = 0;
                cobra.cabeca = CABECA_LADOD;
                cobra.corpoChar = CORPO;
                cobra = AtualizaPosicoesDaCobra( mapa, cobra );
            }
        }
    }

    cobra.estatisticas = InicializaEstatisticas(cobra.estatisticas);

    return cobra;
}

tCobra DefineCharMovimento(tCobra cobra, char movimento){
    cobra.movimento = movimento;

    return cobra;
}

tCobra AlteraPosicaoAnterior(tCobra cobra){
    int i;

    //posicao atual da cobra vira anterior
    cobra.linhaAnterior = cobra.linhaAtual;
    cobra.colunaAnterior = cobra.colunaAtual;

    if( cobra.tamanho > 0 ){
        for(i = 0; i < cobra.tamanho; i++){
            cobra.corpos[i] = AlteraPosicaoAnteriorDeCorpos(cobra.corpos[i]);
        }
    }

    return cobra;
}

tCobra MovimentaCobra(tMapa mapa, tCobra cobra){
    
    if( cobra.movimento == MOVE_HORARIO ){

            //move a cobra e faz a sua cabeca ser imprimida no local certo
            cobra = MovimentaCobraHorario( cobra );

        } else if( cobra.movimento == MOVE_ANTIHORARIO ){

            cobra = MovimentaCobraAntiHorario( cobra );

        } else if( cobra.movimento == CONTINUE ){

            cobra = MovimentaCobraContinue( cobra );
        }

        if( cobra.linhaAtual < 0 || cobra.colunaAtual < 0 || cobra.linhaAtual >= mapa.linhas || cobra.colunaAtual >= mapa.colunas )
            cobra = AtravessaParede( mapa, cobra );

        cobra = AtualizaPosicoesDaCobra( mapa, cobra );
        cobra = MovimentaCorpo( cobra );

    return cobra;
}

tCobra MovimentaCobraHorario(tCobra cobra){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.colunaAtual--;
        cobra.cabeca = CABECA_LADOE;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, ESQUERDA);

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, DIREITA);
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;

        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, BAIXO);
        
    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, CIMA);
    }

    return cobra;
}

tCobra MovimentaCobraAntiHorario(tCobra cobra){

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.colunaAtual++;
        cobra.cabeca = CABECA_LADOD;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, DIREITA);

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.colunaAtual--;
        cobra.cabeca = CABECA_LADOE;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, ESQUERDA);
        
    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.linhaAtual--;
        cobra.cabeca = CABECA_CIMA;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, CIMA);

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.linhaAtual++;
        cobra.cabeca = CABECA_BAIXO;

        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, BAIXO);
    }

    return cobra;
}

tCobra MovimentaCobraContinue(tCobra cobra){
    int i;

    if(cobra.cabeca == CABECA_BAIXO){
        cobra.linhaAtual++;

        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, BAIXO);

    } else if(cobra.cabeca == CABECA_CIMA){
        cobra.linhaAtual--;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, CIMA);

    } else if(cobra.cabeca == CABECA_LADOD){
        cobra.colunaAtual++;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, DIREITA);

    } else if(cobra.cabeca == CABECA_LADOE){
        cobra.colunaAtual--;
        
        cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, ESQUERDA);
    }

    return cobra;
}

tCobra MovimentaCorpo(tCobra cobra){
    int i;

    if( cobra.tamanho > 0 ){
        for(i = 0; i < cobra.tamanho; i++){
            if(i == 0)
                cobra.corpos[i] = MoveCorpoAtrasDeCabeca(cobra, cobra.corpos[i]);
            else
                cobra.corpos[i] = MoveCorpoAtrasDeCorpo(cobra.corpos[i-1], cobra.corpos[i]);
        }
    }

    return cobra;
}

tCobra AumentaTamanho(tMapa mapa, tCobra cobra){
   
    cobra.tamanho++;
    cobra.corpos[cobra.tamanho-1] = InicializaCorpo(cobra);

    return cobra;
}

tCobra AtualizaEstatisticas(tCobra cobra, int id){
    cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, id);

    return cobra;
}

tCobra MataACobra(tCobra cobra){
    cobra.morta = 1;
    cobra.cabeca = CORPO_MORTO;
    cobra.corpoChar = CORPO_MORTO;

    return cobra;
}

tCobra AtravessaParede(tMapa mapa, tCobra cobra){

    if( cobra.linhaAtual < 0 ){
        cobra.linhaAtual = mapa.linhas-1;

    } else if( cobra.colunaAtual < 0 ){
        cobra.colunaAtual = mapa.colunas-1;

    } else if( cobra.linhaAtual >= mapa.linhas ){
        cobra.linhaAtual = 0;

    } else if( cobra.colunaAtual >= mapa.colunas ){
        cobra.colunaAtual = 0;

    }

    return cobra;
}

tCobra AtravessaTunel(tMapa mapa, tCobra cobra){
    tTunel tunelsaida = IdentificaTunelSaida(cobra.linhaAtual, cobra.colunaAtual, mapa.tunel1, mapa.tunel2);

    if( cobra.cabeca == CABECA_CIMA ){
        cobra.linhaAtual = tunelsaida.linha-1;
        cobra.colunaAtual = tunelsaida.coluna;

    } else if( cobra.cabeca == CABECA_LADOD ){
        cobra.linhaAtual = tunelsaida.linha;
        cobra.colunaAtual = tunelsaida.coluna+1;
        
    } else if( cobra.cabeca == CABECA_BAIXO ){
        cobra.linhaAtual = tunelsaida.linha+1;
        cobra.colunaAtual = tunelsaida.coluna;
        
    } else if( cobra.cabeca == CABECA_LADOE ){
        cobra.linhaAtual = tunelsaida.linha;
        cobra.colunaAtual = tunelsaida.coluna-1;
        
    }

    return cobra;
}

tCobra InicializaPosicoesDaCobra(tMapa mapa, tCobra cobra){
    int i=0;


        for(mapa.l = 0; mapa.l < mapa.linhas; mapa.l++){
            for(mapa.c = 0, i; mapa.c < mapa.colunas; mapa.c++, i++){
                cobra.posicoes[i] = InicializaPosicao(mapa.l, mapa.c);
            }
        }
    

    return cobra;
}

tCobra AtualizaPosicoesDaCobra(tMapa mapa, tCobra cobra){
    int i;

    for(i = 0; i < (mapa.linhas*mapa.colunas); i++){
        cobra.posicoes[i] = AtualizaPosicao(cobra.posicoes[i], cobra.linhaAtual, cobra.colunaAtual);
    }

    return cobra;
}

tCobra OrdenaCrescentePosicoes(tCobra cobra, int tam){
    int i=0, j=0, frequenciaj, frequenciai, linhai, linhaj, colunai, colunaj;

    for(i = 0; i <= tam; i++){
        for(j=i+1; j <= tam+1; j++){

            frequenciai = ObtemFrequenciaPosicao( cobra.posicoes[i] );
            frequenciaj = ObtemFrequenciaPosicao( cobra.posicoes[j] );

            if( frequenciaj > frequenciai ){
                cobra = TrocaVetorPosicaoAComB(cobra, j, i);

            } else if( frequenciaj == frequenciai ){
                
                linhai = ObtemLinhaPosicao( cobra.posicoes[i] );
                linhaj = ObtemLinhaPosicao( cobra.posicoes[j] );

                if( linhaj < linhai ){
                    cobra = TrocaVetorPosicaoAComB(cobra, i, j);
                }
            } else if( linhaj == linhai ){
                
                colunai = ObtemColunaPosicao( cobra.posicoes[i] );
                colunaj = ObtemColunaPosicao( cobra.posicoes[j] );

                if( colunaj < colunai )
                    cobra = TrocaVetorPosicaoAComB(cobra, i, j);         
            }
        }
    }

    return cobra;
}

tCobra TrocaVetorPosicaoAComB(tCobra cobra, int a, int b){
    tPosicoes aux;

    aux = cobra.posicoes[a];
    cobra.posicoes[a] = cobra.posicoes[b];
    cobra.posicoes[b] = aux;

    return cobra;
}

/* Funcoes tCorpo */

tCorpo InicializaCorpo(tCobra cobra){
    tCorpo corpo;

    if(cobra.tamanho == 1){
        corpo.linhaAtual = cobra.linhaAnterior;
        corpo.colunaAtual = cobra.colunaAnterior;

    } else if(cobra.tamanho > 1) {
        corpo = InicializaCorpoAtrasDeCorpo(cobra.corpos[cobra.tamanho-2]);

    }

    return corpo;
}

tCorpo InicializaCorpoAtrasDeCorpo(tCorpo corpo){
    tCorpo corpocriado;

    corpocriado.linhaAtual = corpo.linhaAnterior;
    corpocriado.colunaAtual = corpo.colunaAnterior;

    return corpocriado;
}

tCorpo AlteraPosicaoAnteriorDeCorpos(tCorpo corpo){

    corpo.linhaAnterior = corpo.linhaAtual;
    corpo.colunaAnterior = corpo.colunaAtual;

    return corpo;
}

tCorpo MoveCorpoAtrasDeCabeca(tCobra cobra, tCorpo corpo){

    corpo.linhaAtual = cobra.linhaAnterior;
    corpo.colunaAtual = cobra.colunaAnterior;

    return corpo;
}

tCorpo MoveCorpoAtrasDeCorpo(tCorpo anterior, tCorpo corpo){
    corpo.linhaAtual = anterior.linhaAnterior;
    corpo.colunaAtual = anterior.colunaAnterior;

    return corpo;
}

/* Funcoes tMapa */

tMapa InicializaMapa(){
    FILE *mapaf;
    tMapa mapa;

    mapaf = fopen("mapa.txt", "r");

    //define informacoes importantes do mapa
    fscanf(mapaf, "%d %d%*c", &mapa.linhas, &mapa.colunas);
    mapa.comidas = 0;

    //criei uma variavel char e int para auxiliar a leitura do mapa
    char caractere;
    int tunel = 0;

    //le o mapa
    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            fscanf(mapaf, "%c", &caractere);
            mapa.mapa[mapa.l][mapa.c] = caractere;

            //contabiliza o numero de comidas no mapa
            if(caractere == COMIDA){
                mapa.comidas++;
            }

            if( caractere == TUNEL ){
                tunel++;

                if(tunel == 1){
                    mapa.tunel1 = InicializaTunel( mapa.l, mapa.c );

                } else if( tunel == 2 ){
                    mapa.tunel2 = InicializaTunel( mapa.l, mapa.c );
                }
            }

        }
    }
    
    fclose(mapaf);

    return mapa;
}

tMapa DefineVazioAtras(tMapa mapa, tCobra cobra){
    //limpa a cobra na posicao anterior
    if(cobra.tamanho > 0){
        mapa = DefineVazioAtrasDeCorpo( mapa, cobra.corpos[cobra.tamanho-1] );

    } else
        mapa.mapa[cobra.linhaAnterior][cobra.colunaAnterior] = ESPACO_VAZIO;

    return mapa;
}

tMapa DefineVazioAtrasDeCorpo(tMapa mapa, tCorpo corpo){
    mapa.mapa[corpo.linhaAnterior][corpo.colunaAnterior] = ESPACO_VAZIO;
    
    return mapa;
}

tMapa DefineCabecaECorpo(tMapa mapa, tCobra cobra){
    //faz com que a cabeca da cobra seja imprimida na posicao atual
    mapa.mapa[cobra.linhaAtual][cobra.colunaAtual] = cobra.cabeca;

    mapa = DefinePosicaoCorpo(mapa, cobra);

    return mapa;
}

tMapa DiminuiComidasMapa(tMapa mapa){
    mapa.comidas--;

    return mapa;
}

tMapa DefineMaisUmCorpo(tMapa mapa, tCobra cobra){
    if( cobra.tamanho == 1 ){
        mapa.mapa[cobra.linhaAnterior][cobra.colunaAnterior] = cobra.corpoChar;
    } else {
        mapa = DefineMaisUmCorpoAtrasDeCorpo(mapa, cobra.corpos[ cobra.tamanho-2 ], cobra);
    }

    return mapa;
}

tMapa DefineMaisUmCorpoAtrasDeCorpo(tMapa mapa, tCorpo corpo, tCobra cobra){
    mapa.mapa[corpo.linhaAnterior][corpo.colunaAnterior] = cobra.corpoChar;

    return mapa;
}

tMapa DefinePosicaoCorpo(tMapa mapa, tCobra cobra){
    int i;

    //faz com que o corpo da cobra seja imprimida na posicao atual
    for(i = 0; i < cobra.tamanho; i++){
        if(i == 0){
            mapa = DefinePosicaoCorpoAtrasDeCabeca(mapa, cobra, cobra.corpos[i]);
        } else {
            mapa = DefinePosicaoCorpoAtrasDeCorpo(mapa, cobra.corpos[i-1], cobra.corpos[i], cobra);
        }
    }

    return mapa;
}

tMapa DefinePosicaoCorpoAtrasDeCabeca(tMapa mapa, tCobra cobra, tCorpo corpo){
    mapa.mapa[corpo.linhaAtual][corpo.colunaAtual] = cobra.corpoChar;

    return mapa;
}

tMapa DefinePosicaoCorpoAtrasDeCorpo(tMapa mapa, tCorpo corpoAnterior, tCorpo corpo, tCobra cobra){
    mapa.mapa[corpo.linhaAtual][corpo.colunaAtual] = cobra.corpoChar;

    return mapa;
}

/* Funcoes tTunel */

tTunel InicializaTunel( int l, int c ){
    tTunel tunel;

    tunel.linha = l;
    tunel.coluna = c;

    return tunel;
}

tTunel IdentificaTunelSaida( int l, int c, tTunel tunel1, tTunel tunel2){
    if( l == tunel1.linha && c == tunel1.coluna )
        return tunel2;
    
    if( l == tunel2.linha && c == tunel2.coluna )
        return tunel1;
}

/* Funcoes tEstatisticas */

tEstatisticas InicializaEstatisticas(tEstatisticas estatistica){

    estatistica.moveBaixo = 0;
    estatistica.moveCima = 0;
    estatistica.moveDir = 0;
    estatistica.moveEsq = 0;
    estatistica.moveSemPonto = 0;
    estatistica.pontuacao = 0;
    estatistica.qtdmovimentos = 0;

    return estatistica;
}

tEstatisticas DefineEstatisticas(tEstatisticas estatistica, int id){

    if( id == MOVESEMPONTOS ){
        estatistica.moveSemPonto++;

    } else if( id == CORPO_INT ){
        estatistica.pontuacao++;

    } else if( id == PONTOS ){
        estatistica.pontuacao += 10;

    }  else if( id == DIREITA ){
        estatistica.moveDir++;

    } else if( id == ESQUERDA ){
        estatistica.moveEsq++;

    } else if( id == CIMA ){
        estatistica.moveCima++;

    } else if( id == BAIXO ){
        estatistica.moveBaixo++;

    } else if( id == MOVIMENTOS ){
        estatistica.qtdmovimentos++;
    }

    return estatistica;
}

/* Funcoes tPosicoes */

tPosicoes InicializaPosicao(int linha, int coluna){
    tPosicoes posicao;

    posicao.linha = linha;
    posicao.coluna = coluna;
    posicao.vezes = 0;

    return posicao;
}

tPosicoes AtualizaPosicao(tPosicoes posicao, int linha, int coluna){

    if( linha == posicao.linha && coluna == posicao.coluna ){
        posicao.vezes++;
    }

    return posicao;
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
    
    fclose(inicializacaof);
}

void ImprimeResultado(int num, FILE *saida, tCobra cobra){
    int pontuacao;

    pontuacao = ObtemPontuacao(cobra.estatisticas);

    if( num ){
        fprintf(saida, "Voce venceu!\nPontuacao final: %d\n", pontuacao);
    } else{
        fprintf(saida, "Game over!\nPontuacao final: %d\n", pontuacao);
    }
}

void ImprimeMapaDepoisDoMovimento(tMapa mapa, tCobra cobra, FILE *saida){
    int pontuacao = ObtemPontuacao(cobra.estatisticas);

    for(mapa.l=0; mapa.l < mapa.linhas; mapa.l++){

            for(mapa.c=0; mapa.c < mapa.colunas; mapa.c++){

                fprintf(saida, "%c", mapa.mapa[mapa.l][mapa.c]);
            }

            fprintf(saida, "\n");
        }
        fprintf(saida, "Pontuacao: %d\n", pontuacao);
}

void ImprimeEstatisticas(tCobra cobra){
    FILE *estatisticas;
    estatisticas = fopen("estatisticas.txt", "w");

    Estatisticas(cobra.estatisticas, estatisticas);

    fclose(estatisticas);
}

void Estatisticas(tEstatisticas estatisticas, FILE *estatisticasf){
    fprintf(estatisticasf, "Numero de movimentos: %d\n", estatisticas.qtdmovimentos);
    fprintf(estatisticasf, "Numero de movimentos sem pontuar: %d\n", estatisticas.moveSemPonto);
    fprintf(estatisticasf, "Numero de movimentos para baixo: %d\n", estatisticas.moveBaixo);
    fprintf(estatisticasf, "Numero de movimentos para cima: %d\n", estatisticas.moveCima);
    fprintf(estatisticasf, "Numero de movimentos para esquerda: %d\n", estatisticas.moveEsq);
    fprintf(estatisticasf, "Numero de movimentos para direita: %d\n", estatisticas.moveDir);

}

void ImprimeResumo(tMapa mapa, tCobra cobra, int id, FILE *resumo){
    char movichar = ObtemCharDoMovimento(cobra);
    int movint = ObtemNumeroDoMovimento(cobra.estatisticas);

    if( id == MORTE ){
        fprintf(resumo, "Movimento %d (%c) resultou no fim de jogo por conta de colisao\n", movint, movichar);

    } else if( id == CORPO_INT ){
        if( mapa.comidas == 0){
            fprintf(resumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", movint, movichar, cobra.tamanho+1);

        } else {
            fprintf(resumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", movint, movichar, cobra.tamanho+1);

        }
    } else if( id == PONTOS ){
        fprintf(resumo, "Movimento %d (%c) gerou dinheiro\n", movint, movichar);

    }
}

void ImprimePosicoes(tMapa mapa, tCobra cobra){
    int i, qtdmovimentos = ObtemQuantidadeDeMovimentos(cobra.estatisticas);
    FILE *ranking;
    ranking = fopen("ranking.txt", "w");


    cobra = OrdenaCrescentePosicoes( cobra, qtdmovimentos );

    for(i = 0; i < (mapa.linhas*mapa.colunas); i++){
        
        ImprimePosicao(cobra.posicoes[i], ranking);
    }

    fclose(ranking);
}

void ImprimePosicao(tPosicoes posicao, FILE *ranking){

    if( posicao.vezes > 0){

        fprintf(ranking, "(%d, %d) - %d\n", posicao.linha, posicao.coluna, posicao.vezes);
    }
}

void ImprimeHeatMap(tMapa mapa, tCobra cobra){
    tHeatmap heatmap;
    int frequencia, i = 0;
    FILE *heatmapf;
    heatmapf = fopen("heatmap.txt", "w");

    for(mapa.l = 0; mapa.l < mapa.linhas; mapa.l++){
        for(mapa.c = 0, i; mapa.c < mapa.colunas; mapa.c++, i++){

            frequencia = ObtemFrequenciaPosicao(cobra.posicoes[i]);

            fprintf(heatmapf, "%d", frequencia);

            if(mapa.c >= 0 && mapa.c < mapa.colunas-1)
                fprintf(heatmapf, " ");
        }
        fprintf(heatmapf, "\n");
    }

    fclose(heatmapf);
}

/* Funcoes int */

int IdentificaCaraceterSucessor(tMapa mapa, tCobra cobra){
    char caracter; //variavel p auxiliar na atualizacao da situacao do jogo

    caracter = mapa.mapa[cobra.linhaAtual][cobra.colunaAtual];

    if( caracter == COMIDA )
        return 1;
    else if( caracter == DINHEIRO )
        return 2;
    else if( caracter == PAREDE || caracter == cobra.corpoChar )
        return 3;
    else if( caracter == TUNEL )
        return 4;
    else
        return 0;
}

int DefineGanhouOuPerdeu(tMapa mapa){
    if( !mapa.comidas ){
        return 1;
    } else 
        return 0;
}

int ObtemPontuacao(tEstatisticas estatisticas){
    return estatisticas.pontuacao;
}

int ObtemNumeroDoMovimento(tEstatisticas estatisticas){
    return estatisticas.qtdmovimentos;
}

int ObtemQuantidadeDeMovimentos(tEstatisticas estatisticas){
    return estatisticas.qtdmovimentos;
}

int CobraMortaOuNao(tCobra cobra){
    return cobra.morta;
}

int ObtemLinhaPosicao(tPosicoes posicao){
    return posicao.linha;
}

int ObtemColunaPosicao(tPosicoes posicao){
    return posicao.coluna;
}

int ObtemFrequenciaPosicao(tPosicoes posicao){
    return posicao.vezes;
}

/* Funcao char */

char ObtemCharDoMovimento(tCobra cobra){
    return cobra.movimento;
}
