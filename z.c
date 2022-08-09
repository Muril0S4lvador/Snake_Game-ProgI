#include <stdio.h>
#include <stdlib.h>

/*Defines do Programa*/

/* Define o a cobra e seu corpo */
#define CABECA_LADOE '<'
#define CABECA_LADOD '>'
#define CABECA_CIMA '^'
#define CABECA_BAIXO 'v'
#define CORPO 'o'
#define CORPO_MORTO 'X'

/* Define os movimentos da cobra */
#define CONTINUE 'c'
#define MOVE_HORARIO 'h'
#define MOVE_ANTIHORARIO 'a'

/* Define os caracteres do mapa */
#define ESPACO_VAZIO ' '
#define COMIDA '*'
#define DINHEIRO '$'
#define PAREDE '#'
#define TUNEL  '@'

/* Define o tamanho maximo permitido e END para enderecos */
#define MAX 100
#define END 1000

/* Define ints para controle de cada rodada do jogo */
#define NADA 0
#define COMIDA_INT 1
#define PONTOS 2
#define MORTE 3
#define PORTAL_INT 4
#define DIREITA 5
#define ESQUERDA 6
#define CIMA 7
#define BAIXO 8
#define MOVIMENTOS 9
#define MOVESEMPONTOS 10

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
    char endereco[END];
    tCobra cobra;
    tMapa mapa;
} tJogo;

/*Inicializacao das Funcoes do Programa*/

tJogo InicializaJogo(tJogo jogo);
tJogo RealizaJogo(tJogo jogo);
tJogo AtualizaJogo(tJogo jogo, FILE *resumo);

tCobra InicializaCobra(tMapa mapa);
tCobra DefineCharComandoMovimento(tCobra cobra, char movimento);
tCobra AlteraPosicaoAnterior(tCobra cobra);
tCobra MovimentaCobra(tMapa mapa, tCobra cobra);
tCobra MovimentaCobraHorario(tCobra cobra);
tCobra MovimentaCobraAntiHorario(tCobra cobra);
tCobra MovimentaCobraContinue(tCobra cobra);
tCobra MovimentaCorpo(tCobra cobra);
tCobra VerificaCabecaECorpo(tMapa mapa, tCobra cobra);
tCobra AumentaTamanho(tMapa mapa, tCobra cobra);
tCobra AtualizaEstatisticas(tCobra cobra, int id);
tCobra MataACobra(tCobra cobra);
tCobra AtravessaTunel(tMapa mapa, tCobra cobra);
tCobra VerificaSePrecisaAtravessarParede(tMapa mapa, tCobra cobra);
tCobra AtravessaParede(tMapa mapa, tCobra cobra);
tCobra InicializaPosicoesDaCobra(tMapa mapa, tCobra cobra);
tCobra AtualizaPosicoesDaCobra(tMapa mapa, tCobra cobra);
tCobra OrdenaDecrescentePosicoes(tCobra cobra, int tam);
tCobra TrocaVetorPosicaoAComB(tCobra cobra, int a, int b);

tCorpo InicializaCorpo(tCobra cobra);
tCorpo InicializaCorpoAtrasDeCorpo(tCorpo corpo);
tCorpo AlteraPosicaoAnteriorDeCorpos(tCorpo corpo);
tCorpo MoveCorpoAtrasDeCabeca(tCobra cobra, tCorpo corpo);
tCorpo MoveCorpoAtrasDeCorpo(tCorpo anterior, tCorpo corpo);

tMapa InicializaMapa(char endereco[]);
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

void ImprimeInicializacao(tMapa mapa, tCobra cobra, char endereco[]);
void ImprimeResultado(int num, FILE *saida, tCobra cobra);
void ImprimeMapaDepoisDoMovimento(tMapa mapa, tCobra cobra, FILE *saida);
void ImprimeEstatisticas(tCobra cobra, char endereco[]);
void Estatisticas(tEstatisticas estatisticas, FILE *estatisticasf);
void ImprimeResumo(tMapa mapa, tCobra cobra, int id, FILE *resumo);
void ImprimePosicoes(tMapa mapa, tCobra cobra, char endereco[]);
void ImprimePosicao(tPosicoes posicao, FILE *ranking);
void ImprimeHeatMap(tMapa mapa, tCobra cobra, char endereco[]);

int IdentificaCaraceterSucessor(tMapa mapa, tCobra cobra);
int DefineGanhouOuPerdeu(tMapa mapa);
int ObtemPontuacao(tEstatisticas estatisticas);
int ObtemNumeroDoMovimento(tEstatisticas estatisticas);
int ObtemQuantidadeDeMovimentos(tEstatisticas estatisticas);
int CobraMortaOuNao(tCobra cobra);
int ObtemLinhaPosicao(tPosicoes posicao);
int ObtemColunaPosicao(tPosicoes posicao);
int ObtemFrequenciaPosicao(tPosicoes posicao);
int ObtemLinhaAtualCorpo(tCorpo corpo);
int ObtemColunaAtualCorpo(tCorpo corpo);

char ObtemCharDoMovimento(tCobra cobra);

/*Funcao Main*/

int main(int argc, char **argv){
    tJogo jogo;

    /* Obtencao do diretorio */
    if(argc <= 1){
        printf("ERRO: O diretorio de arquivos nao foi informado\n");
        exit(0);
    }
    sprintf(jogo.endereco, "%s", argv[1]);

    jogo = InicializaJogo(jogo);
    jogo = RealizaJogo(jogo);
    ImprimeEstatisticas(jogo.cobra, jogo.endereco);

    return 0;
}

/* Codigo das Funcoes */

/* Funcoes tJogo */

tJogo InicializaJogo(tJogo jogo){

    jogo.mapa = InicializaMapa(jogo.endereco);
    jogo.cobra = InicializaCobra(jogo.mapa);
    ImprimeInicializacao(jogo.mapa, jogo.cobra, jogo.endereco);

    return jogo;
}

tJogo RealizaJogo(tJogo jogo){
/* Essa funcao tem o intuito de mover a cobra e interpretar os caracteres do mapa */

    FILE *movimentos, *saida, *resumo;
    char saidaS[END+100], resumoS[END+100]; /* Variaveis para auxiliar na leitura do enderenco dos documentos, adicionei +100 pois o programa nao aceitava compilar 
    jogo.endereco[END] e saidaS[END] com o mesmo tamanho*/
    
    char movimento;    // Variavel para auxiliar na leitura dos comandos de movimento
    int ganhou, morta; // Variaveis p auxiliar na atualizacao da situacao do jogo

    sprintf(saidaS, "%s/saida/saida.txt", jogo.endereco);
    sprintf(resumoS, "%s/saida/resumo.txt", jogo.endereco);

    saida = fopen(saidaS, "w");
    resumo = fopen(resumoS, "w");

    while(1){ 
        scanf("%c", &movimento);
        
        /* Garante que apenas os cracteres especificos para o movimento da cobra sejam lidos */
        if(movimento == MOVE_ANTIHORARIO || movimento == MOVE_HORARIO || movimento == CONTINUE){

            /* A funcao DefineCharComandoMovimento salva o comando de movimento afim de utiliza-lo na impressao do resumo
            A funcao AtualizaEstatisticas utiliza*/
            jogo.cobra = DefineCharComandoMovimento(jogo.cobra, movimento);
            jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVIMENTOS);

            /* A funcao AlteraPosicaoAnterior altera a posicao anterior da cobra e, a funcao DefineVazioAtras usa a informacao
            obtida pela funcao anterior para imprimir um vazio na posicao anterior do ultimo corpo da cobra ou da cabeca, ou seja, 
            apaga a posicao anterior da cobra */
            jogo.cobra = AlteraPosicaoAnterior(jogo.cobra);
            jogo.mapa = DefineVazioAtras(jogo.mapa, jogo.cobra);

            jogo.cobra = MovimentaCobra( jogo.mapa, jogo.cobra );

            /* A funcao AtualizaJogo eh utilizada para interpretar qual o caractere que a cabeca da cobra esta em cima e,
            a partir disso, realizar a acao necessaria. Ja a AtualizaPosicoesDaCobra, salva a posicao em que a cabeca da
            cobra esta, afim de ajudar no ranking e heatmap */
            jogo = AtualizaJogo(jogo, resumo);
            jogo.cobra = AtualizaPosicoesDaCobra( jogo.mapa, jogo.cobra );

            /* Imprime mapa depois do movimento executado */
            fprintf(saida, "\nEstado do jogo apos o movimento '%c':\n", movimento);
            printf("\nEstado do jogo apos o movimento '%c':\n", movimento);
            ImprimeMapaDepoisDoMovimento(jogo.mapa, jogo.cobra, saida);


            /* A funcao a seguir verifica se a cobra ainda esta viva ou nao. Caso esteja, ela sai do while para terminar o jogo */
            morta = CobraMortaOuNao(jogo.cobra);
            if( morta )
                break;

            /* A funcao a seguir verifica se ha comidas no mapa ou nao. Caso nao tenha, ela sai do while para terminar o jogo */
            ganhou = DefineGanhouOuPerdeu(jogo.mapa);
            if( ganhou )
                break;
        }
    }

    /* As funcoes, respectivamente, imprimem o resultado no terminal e o ranking e heatmap nos seus respectivos documentos */
    ImprimeResultado(ganhou, saida, jogo.cobra);
    ImprimePosicoes(jogo.mapa, jogo.cobra, jogo.endereco);
    ImprimeHeatMap(jogo.mapa, jogo.cobra, jogo.endereco);

    /* Fecha as pastas usadas */
    fclose(resumo);
    fclose(saida);

    return jogo;
}

tJogo AtualizaJogo(tJogo jogo, FILE *resumo){
/* Essa funcao tem o intuito de identificar qual o caractere que a cobra esta indo e, assim, fazer as atualizacoes necessarias */

    int atualiza;

    /* Retorna um numero de acordo com o caracter lido */
    atualiza = IdentificaCaraceterSucessor(jogo.mapa, jogo.cobra);

    if( !atualiza ){
        /* Cobra moveu e nao teve nada a frente */
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVESEMPONTOS);

    } else if( atualiza == COMIDA_INT ){
        /* Cobra moveu e teve comida a frente */

        jogo.cobra = AumentaTamanho(jogo.mapa, jogo.cobra);
        jogo.mapa = DefineMaisUmCorpo(jogo.mapa, jogo.cobra);
        jogo.mapa = DiminuiComidasMapa(jogo.mapa);

        jogo.cobra = AtualizaEstatisticas(jogo.cobra, COMIDA_INT);

        ImprimeResumo(jogo.mapa, jogo.cobra, COMIDA_INT, resumo);

    } else if( atualiza == PONTOS ){
        /* Cobra moveu e teve dinheiro a frente */

        jogo.cobra = AtualizaEstatisticas(jogo.cobra, PONTOS);

        ImprimeResumo(jogo.mapa, jogo.cobra, PONTOS, resumo);

    } else if( atualiza == MORTE ){
        /* Cobra moveu e teve obstaculos a frente */

        jogo.cobra = MataACobra(jogo.cobra);
        jogo.cobra = AtualizaEstatisticas(jogo.cobra, MOVESEMPONTOS);

        ImprimeResumo(jogo.mapa, jogo.cobra, MORTE, resumo);

    } else if( atualiza == PORTAL_INT ){
        /* Cobra moveu e teve portal a frente */

        jogo.cobra = AtravessaTunel(jogo.mapa, jogo.cobra);

        jogo = AtualizaJogo(jogo, resumo);

    }

    jogo.mapa = DefineCabecaECorpo(jogo.mapa, jogo.cobra);
    /* A funcao DefineCabecaECorpo define no mapa onde vai ser a cabeca e o corpo da cobra, para ser impresso depois por outra funcao */
    /* No geral, a maioria das minhas funcoes "Define" tem esse intuito de apenas definir algo para ser utilizado por outra funcao depois */
    
    jogo.cobra = VerificaCabecaECorpo(jogo.mapa, jogo.cobra);
    /* A funcao VerificaCabecaECorpo serve para acertar um bug que estava dando, como meu corpo eh definido depois da cabeca, quando a cobra entrava
    em um portal com 1 bloco de distancia e 1 corpo ela nao morria. Agora, quando isso acontece, ela morre */

    return jogo;
}

/* Funcoes tCobra */

tCobra InicializaCobra(tMapa mapa){
/* Inicializa tudo referente a cobra, no intuito de facilitar a manipulacao das informacoes no decorrer do jogo */

    tCobra cobra;

    cobra = InicializaPosicoesDaCobra( mapa, cobra );

    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            if(mapa.mapa[mapa.l][mapa.c] == CABECA_LADOD){

                /* Define todas as informacoes da cobra importantes para o decorrer do jogo */
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

tCobra DefineCharComandoMovimento(tCobra cobra, char movimento){
    cobra.movimento = movimento;

    return cobra;
}

tCobra AlteraPosicaoAnterior(tCobra cobra){
    int i;

    /* Altera a posicao anterior da cabeca da cobra */
    cobra.linhaAnterior = cobra.linhaAtual;
    cobra.colunaAnterior = cobra.colunaAtual;

    /* Altera a posicao dos corpos da cobra, se tiver */
    if( cobra.tamanho > 0 ){
        for(i = 0; i < cobra.tamanho; i++){
            cobra.corpos[i] = AlteraPosicaoAnteriorDeCorpos(cobra.corpos[i]);
        }
    }

    return cobra;
}

tCobra MovimentaCobra(tMapa mapa, tCobra cobra){
/* Pega o comando de movimento da cobra(obtido com a funcao DefineCharComandoMovimento) e movimenta de acordo com ele */

    if( cobra.movimento == MOVE_HORARIO ){

            cobra = MovimentaCobraHorario( cobra );

        } else if( cobra.movimento == MOVE_ANTIHORARIO ){

            cobra = MovimentaCobraAntiHorario( cobra );

        } else if( cobra.movimento == CONTINUE ){

            cobra = MovimentaCobraContinue( cobra );
        }
        cobra = VerificaSePrecisaAtravessarParede( mapa, cobra );
        cobra = MovimentaCorpo( cobra );

    return cobra;
}

tCobra MovimentaCobraHorario(tCobra cobra){
/* De acordo com a cabeca da cobra, realiza o movimento pedido */

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
    /* As DefineEstatisticas usadas aqui e nas outras servem para atualizar as estatisticas de movimento da cobra, como os movimentos para baixo, esquerda, etc. */

    return cobra;
}

tCobra MovimentaCobraAntiHorario(tCobra cobra){
/* De acordo com a cabeca da cobra, realiza o movimento pedido */

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
/* De acordo com a cabeca da cobra, realiza o movimento pedido */

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
/* Movimenta o corpo, oprimeiro de acordo com a cabeca e os restantes de acordo com o corpo antecessor */

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

tCobra VerificaCabecaECorpo(tMapa mapa, tCobra cobra){
    int linhaatualdocorpo = ObtemLinhaAtualCorpo(cobra.corpos[cobra.tamanho-1]),
    colunaatualdocorpo = ObtemColunaAtualCorpo(cobra.corpos[cobra.tamanho-1]);

    if( cobra.tamanho > 0 ){
        if( cobra.linhaAtual == linhaatualdocorpo && cobra.colunaAtual == colunaatualdocorpo )
            cobra = MataACobra( cobra );
    }

    return cobra;
}

tCobra AumentaTamanho(tMapa mapa, tCobra cobra){
   
    cobra.tamanho++;
    cobra.corpos[cobra.tamanho-1] = InicializaCorpo(cobra);

    return cobra;
}

tCobra AtualizaEstatisticas(tCobra cobra, int id){
/* Funcao apenas de ponte para utilizar o tipo tEstatisticas */

    cobra.estatisticas = DefineEstatisticas(cobra.estatisticas, id);

    return cobra;
}

tCobra MataACobra(tCobra cobra){
/* Mata a cobra e muda seu corpo e cabeca */

    cobra.morta = 1;
    cobra.cabeca = CORPO_MORTO;
    cobra.corpoChar = CORPO_MORTO;

    return cobra;
}

tCobra AtravessaTunel(tMapa mapa, tCobra cobra){
/* Identifica o tunel de saida e, de acordo com a cabeca da cobra, define a posicao da cobra corretamente */

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
    cobra = VerificaSePrecisaAtravessarParede( mapa, cobra );

    return cobra;
}

tCobra VerificaSePrecisaAtravessarParede(tMapa mapa, tCobra cobra){
/* Verifica se a cobra esta alem dos limites do mapa e, se estiver, ela atravessara a parede */

    if( cobra.linhaAtual < 0 || cobra.colunaAtual < 0 || cobra.linhaAtual >= mapa.linhas || cobra.colunaAtual >= mapa.colunas )
        cobra = AtravessaParede( mapa, cobra );

    return cobra;       
}

tCobra AtravessaParede(tMapa mapa, tCobra cobra){
/* Basicamente, essa funcao funciona assim:
Se esta alem da linha/coluna maxima do mapa , a cabeca da cobra retorna a linha/coluna 0
Se esta antes da linha/coluna 0 do mapa, a cabeca retorna a linha/coluna limite do mapa*/

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

tCobra InicializaPosicoesDaCobra(tMapa mapa, tCobra cobra){
/* Cria um vetor com todas as posicoes possiveis pela cobra e zera a posicao.vezes, que representa a frequencia que a cobra foi naquela posicao */
    int i;

    for(mapa.l = 0; mapa.l < mapa.linhas; mapa.l++){
        for(mapa.c = 0, i; mapa.c < mapa.colunas; mapa.c++, i++){
            cobra.posicoes[i] = InicializaPosicao(mapa.l, mapa.c);
        }
    }    

    return cobra;
}

tCobra AtualizaPosicoesDaCobra(tMapa mapa, tCobra cobra){
/* Varre o vetor com as posicoes possiveis pela cobra e, se encontra uma coordenada igual a posicao que ela esta atualmente aumenta em 1 a frequencia naquela posicao*/
    int i;

    for(i = 0; i < (mapa.linhas*mapa.colunas); i++){
        cobra.posicoes[i] = AtualizaPosicao(cobra.posicoes[i], cobra.linhaAtual, cobra.colunaAtual);
    }

    return cobra;
}

tCobra OrdenaDecrescentePosicoes(tCobra cobra, int tam){
/* Usa as variabeis compara1 e compara2 para ordenar as posicoes
Se a frequencia eh igual verifica as linhas,
Se as linhas sao iguais verifica a coluna e, de acordo com qua for menor salva a posicao no lugar adequado */

    int i, j, compara1, compara2;
    tPosicoes aux;

    for(i = 0; i <= tam-1; i++){
        for(j=i+1; j <= tam; j++){

            compara1 = ObtemFrequenciaPosicao(cobra.posicoes[i]);
            compara2 = ObtemFrequenciaPosicao(cobra.posicoes[j]);

            if(compara2 > compara1){
                cobra = TrocaVetorPosicaoAComB(cobra, i, j); 

            } else if(compara2 == compara1){

                compara1 = ObtemLinhaPosicao(cobra.posicoes[i]);
                compara2 = ObtemLinhaPosicao(cobra.posicoes[j]);

                if(compara2 < compara1){
                    cobra = TrocaVetorPosicaoAComB(cobra, i, j);  

                } else if(compara2 == compara1){

                    compara1 = ObtemColunaPosicao(cobra.posicoes[i]);
                    compara2 = ObtemColunaPosicao(cobra.posicoes[j]);

                    if(compara2 < compara1){
                        cobra = TrocaVetorPosicaoAComB(cobra, i, j); 
                    }
                }
            }
        }
    }

    return cobra;
}

tCobra TrocaVetorPosicaoAComB(tCobra cobra, int a, int b){
/* Troca as posicoes dos vetores */

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
/* Mesmo intuito da funcao AlteraPosicaoAnterior explicada anteriormente */

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

tMapa InicializaMapa(char endereco[]){
    FILE *mapaf;
    tMapa mapa;
    char mapaS[END];

    /* Abertura do mapa por meio de arquivos */
    sprintf(mapaS, "%s/mapa.txt", endereco);
    mapaf = fopen(mapaS, "r");

    if( !mapaf ){
        printf("Nao existe o arquivo mapa.txt no diretorio %s.\n", endereco);
        exit(0);
    }

    /* Define o tamanho do mapa e inicializa as comidas */
    fscanf(mapaf, "%d %d%*c", &mapa.linhas, &mapa.colunas);
    mapa.comidas = 0;

    /* Variaveis char e int para auxiliar a leitura do mapa */ 
    char caractere;
    int tunel = 0;

    for(mapa.l=0; mapa.l <= mapa.linhas; mapa.l++){
        
        for(mapa.c=0; mapa.c <= mapa.colunas; mapa.c++){

            fscanf(mapaf, "%c", &caractere);
            mapa.mapa[mapa.l][mapa.c] = caractere;

            /* Contabiliza o numero de comidas no mapa */
            if(caractere == COMIDA){
                mapa.comidas++;
            }

            /* Identifica os tuneis pelo mapa, se tiver */
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
/* Limpa a cobra na posicao anterior, caso tenha corpo, sera feito pela funcao DefineVazioAtrasDeCorpo */

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
/* Faz com que a cabeca da cobra seja imprimida na posicao atual. Se houver corpo, faz o mesmo */

    mapa.mapa[cobra.linhaAtual][cobra.colunaAtual] = cobra.cabeca;

    mapa = DefinePosicaoCorpo(mapa, cobra);

    return mapa;
}

tMapa DiminuiComidasMapa(tMapa mapa){
    mapa.comidas--;

    return mapa;
}

tMapa DefineMaisUmCorpo(tMapa mapa, tCobra cobra){
/* Quando a cobra come uma comida e precisa aumentar de corpo, essa funcao realiza isso. Se nao tiver corpo antes, inicializa de acordo com a cabeca */

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
/* Faz com que o corpo da cobra seja imprimida na posicao atual */
    int i;
    
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
/* Recebe as coordenadas da cobra e, de acordo com elas, identifica o tunel da saida */

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
/* Todas as estatisticas sao atualizadas aqui */

    if( id == MOVESEMPONTOS ){
        estatistica.moveSemPonto++;

    } else if( id == COMIDA_INT ){
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

void ImprimeInicializacao(tMapa mapa, tCobra cobra, char endereco[]){
    FILE *inicializacaof;
    char inicializacaoS[END];

    sprintf(inicializacaoS, "%s/saida/inicializacao.txt", endereco);
    inicializacaof = fopen(inicializacaoS, "w");

    for(mapa.l=0; mapa.l < mapa.linhas; mapa.l++){

        for(mapa.c=0; mapa.c < mapa.colunas; mapa.c++){

            fprintf(inicializacaof, "%c", mapa.mapa[mapa.l][mapa.c]);
            //printf("%c", mapa.mapa[mapa.l][mapa.c]);
        }

        fprintf(inicializacaof, "\n");
        //printf("\n");
    }

    fprintf(inicializacaof, "A cobra comecara o jogo na linha %d e coluna %d\n", cobra.linhaAtual+1, cobra.colunaAtual+1);                                                
                                    /* Como comeco lendo a partir do 0, pra imprimir corretamente preciso adicionar +1 */
    
    fclose(inicializacaof);
}

void ImprimeResultado(int num, FILE *saida, tCobra cobra){
/* Imprime se a pessoa ganhou o jogo ou nao, junto com a pontuacao */
    int pontuacao;

    pontuacao = ObtemPontuacao(cobra.estatisticas);

    if( num ){
        fprintf(saida, "Voce venceu!\nPontuacao final: %d\n", pontuacao);
        printf("Voce venceu!\nPontuacao final: %d\n", pontuacao);

    } else{
        fprintf(saida, "Game over!\nPontuacao final: %d\n", pontuacao);
        printf("Game over!\nPontuacao final: %d\n", pontuacao);
    }
}

void ImprimeMapaDepoisDoMovimento(tMapa mapa, tCobra cobra, FILE *saida){
    int pontuacao = ObtemPontuacao(cobra.estatisticas);

    for(mapa.l=0; mapa.l < mapa.linhas; mapa.l++){

            for(mapa.c=0; mapa.c < mapa.colunas; mapa.c++){

                fprintf(saida, "%c", mapa.mapa[mapa.l][mapa.c]);
                printf("%c", mapa.mapa[mapa.l][mapa.c]);
            }

            fprintf(saida, "\n");
            printf("\n");
        }
        fprintf(saida, "Pontuacao: %d\n", pontuacao);
        printf("Pontuacao: %d\n", pontuacao);
}

void ImprimeEstatisticas(tCobra cobra, char endereco[]){
    FILE *estatisticas;
    char estatisticasS[END];

    sprintf(estatisticasS, "%s/saida/estatisticas.txt", endereco);
    estatisticas = fopen(estatisticasS, "w");

    Estatisticas(cobra.estatisticas, estatisticas);

    fclose(estatisticas);
}

void Estatisticas(tEstatisticas estatisticas, FILE *estatisticasf){
/* Imprime estatisticas, nao fiz isso em uma unica funcao, pois nao conseguiria acessar tEstatisticas */

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

    } else if( id == COMIDA_INT ){
        if( mapa.comidas == 0){
            fprintf(resumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d, terminando o jogo\n", movint, movichar, cobra.tamanho+1);

        } else {
            fprintf(resumo, "Movimento %d (%c) fez a cobra crescer para o tamanho %d\n", movint, movichar, cobra.tamanho+1);

        }
    } else if( id == PONTOS ){
        fprintf(resumo, "Movimento %d (%c) gerou dinheiro\n", movint, movichar);

    }
}

void ImprimePosicoes(tMapa mapa, tCobra cobra, char endereco[]){
/* Ordena as posicoes de acordo com os requisitos pedidos o as imprime em ordem */

    int i, qtdmovimentos = ObtemQuantidadeDeMovimentos(cobra.estatisticas);
    FILE *ranking;
    char rankingS[END];

    sprintf(rankingS, "%s/saida/ranking.txt", endereco);
    ranking = fopen(rankingS, "w");

    cobra = OrdenaDecrescentePosicoes( cobra, mapa.linhas*mapa.colunas );

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

void ImprimeHeatMap(tMapa mapa, tCobra cobra, char endereco[]){
    int frequencia, i = 0;
    FILE *heatmapf;
    char heatmapS[END];
    
    sprintf(heatmapS, "%s/saida/heatmap.txt", endereco);
    heatmapf = fopen(heatmapS, "w");

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
/* Retorna um inteiro de acordo com cada caracter , para que a funcao AtualizaJogo faca sua determinada funcao*/
    char caracter;

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
/* Se tem comidas no mapa, retorna 0, caso nao tenha, retorna 1, o que faz com que ganhe e termine o jogo */
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

int ObtemLinhaAtualCorpo(tCorpo corpo){
    return corpo.linhaAtual;
}

int ObtemColunaAtualCorpo(tCorpo corpo){
    return corpo.colunaAtual;
}

/* Funcao char */

char ObtemCharDoMovimento(tCobra cobra){
    return cobra.movimento;
}
