#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <ctype.h>

#define TAM_TAB 9
#define TAM_POS 4

void exibeRegras();
void exibeTabuleiro(char tab[TAM_TAB][TAM_TAB]);
char naCasa(char tab[TAM_TAB][TAM_TAB], char *);
int validaPeca(char tab[TAM_TAB][TAM_TAB], char *, char);
int validaMovimento(char tab[TAM_TAB][TAM_TAB], char *, char *);
void verificaConversao(char tab[TAM_TAB][TAM_TAB], char *, char*);
int verificaVitoria(char tab[TAM_TAB][TAM_TAB], char);

int main(){

    setlocale(LC_ALL, "portuguese");

    int i, j, x, resultado;
    char tabuleiro[TAM_TAB][TAM_TAB], peca[TAM_POS], posicao[TAM_POS], menu[3], player, aliado, turno, contagem;
    srand(time(NULL));

    while(1){

        turno = 1;
        contagem = 1;

        //Menu do jogo. Aperte 1 para começar, 2 para ler as regras e 3 para sair.
        do{
            printf("/--------------------\\\n|  ROBERTO'S GAME    |\n\\--------------------/\n\n");
            printf("Aperte:\n\n-1 para começar;\n\n-2 para ver as regras;\n\n-3 para sair.\n\n");
            fgets(menu, 3, stdin);
            fflush(stdin);

            //Verifica se o número digitado é válido.
            while(1){
                if(menu[0] >= '1' && menu[0] <= '3' && menu[1] == '\n')
                    break;
                system("cls");
                printf("/--------------------\\\n|  ROBERTO'S GAME    |\n\\--------------------/\n\n");
                printf("Numero invalido!\nAperte:\n\n-1 para comecar;\n\n-2 para ver as regras;\n\n-3 para sair.\n\n");
                fgets(menu, 3, stdin);
                fflush(stdin);
            }

            //Executa as ações de acordo com o número digitado pelo jogador.
            if(menu[0] == '1')
                break;
            else if(menu[0] == '2')
                exibeRegras();
            else{
                //Encerra o jogo.
                for(i = 0; i < 3; i++){
                    system("cls");
                    printf("\n\nEncerrando");
                    for(j = 0; j < 5; j++){
                        printf(".");
                        usleep(500000);
                    }
                }
                printf("\n\n");
                return 0;
            }
        } while(1);


        //Preenche a matriz com espaços vazios.
        for(i=0; i<TAM_TAB; i++){
            for(j=0; j<TAM_TAB; j++){
                tabuleiro[i][j]=' ';
            }
        }

        //Preenche a matriz com as peças do jogo de forma aleatória.
        for(x=0; x<10 ; x++){
            do{
                i=rand()%TAM_TAB;
                j=rand()%TAM_TAB;
            } while(tabuleiro[i][j]!=' ');

            if(x<5)
                tabuleiro[i][j]='X';
            else
                tabuleiro[i][j]='O';
        }


        do{
            //Começa o jogo.
            //As variáveis contagem e player determinam os turnos e a vez de cada jogador, quando player for O, o turno muda.
            if(contagem%2==0){
                player='O';
                aliado='o';
            }
            else{
                player='X';
                aliado='×';
            }


            //Pede para o jogador escolher uma peça.
            system("cls");
            printf("TURNO %d\nJOGADOR %c:\n\n", turno, player);
            exibeTabuleiro(tabuleiro);
            printf("Digite a posição da peça! (Ex.: 5e): ");
            fgets(peca, TAM_POS, stdin);
            fflush(stdin);
            peca[1] = tolower(peca[1]);

            //Verifica se a peça escolhida pelo joador é válida.
            while(validaPeca( tabuleiro, peca, player)){
                system("cls");
                printf("TURNO %d\nJOGADOR %c:\n\n", turno, player);
                exibeTabuleiro(tabuleiro);
                if(peca[0] - '1' < 0 || peca[0] - '1' > 8 || peca[1] - 'a' < 0 || peca[1] - 'a' > 8 || peca[2] != '\n')
                    printf("ERRO!\nPosição escolhida está fora do tabuleiro, digite novamente: ");
                else if(naCasa(tabuleiro, peca) == ' ')
                    printf("ERRO!\nPosição vazia, digite novamente: ");
                else if(naCasa(tabuleiro, peca) == 'X' || naCasa(tabuleiro, peca) == 'O')
                    printf("ERRO!\nPosição escolhida tem a peça '%c', digite novamente: ", naCasa(tabuleiro, peca));
                else if(naCasa(tabuleiro, peca) == '×' || naCasa(tabuleiro, peca) == 'o')
                    printf("ERRO!\nPosição escolhida tem o aliado '%c', digite novamente: ", naCasa(tabuleiro, peca));
                fgets(peca, TAM_POS, stdin);
                fflush(stdin);
                peca[1] = tolower(peca[1]);
            }


            //Pede para o jogador escolher a posição para se mover.
            system("cls");
            printf("TURNO %d\nJOGADOR %c:\n\n", turno, player);
            exibeTabuleiro(tabuleiro);
            printf("Digite a posição para mover a peça! (Ex.: 5e): ");
            fgets(posicao, TAM_POS, stdin);
            fflush(stdin);
            posicao[1] = tolower(posicao[1]);

            //Verifica se a posição escolhida é válida.
            while(validaMovimento(tabuleiro, peca, posicao)){
                x = validaMovimento(tabuleiro, peca, posicao);
                system("cls");
                printf("TURNO %d\nJOGADOR %c:\n\n", turno, player);
                exibeTabuleiro(tabuleiro);
                if(x == 1)
                    printf("ERRO!\nPosição escolhida está fora do tabuleiro, digite novamente: ");
                else if(x == 2)
                    printf("ERRO!\nNenhum movimento realizado, digite novamente: ");
                else if(x == 3)
                    printf("ERRO!\nPosição escolhida está ocupada, digite novamente: ");
                else if(x == 4)
                    printf("ERRO!\nNão é permitido pular sobre outras peças, digite novamente: ");
                else if(x == 5)
                    printf("ERRO!\nO movimento só pode ser na horizontal, vertical ou diagonal, digite novamente: ");
                fgets(posicao, TAM_POS, stdin);
                fflush(stdin);
                posicao[1] = tolower(posicao[1]);
            }


            //Realiza o movimento e faz a conversão dos aliados.
            tabuleiro[posicao[0]-'1'][posicao[1]-'a']=player;
            tabuleiro[peca[0]-'1'][peca[1]-'a']=aliado;
            verificaConversao(tabuleiro, peca, posicao);

            //Altera o turno.
            if(player=='O')
                turno++;
            contagem++;

            //Verifica vitória.
            resultado = verificaVitoria(tabuleiro, player);
        } while(resultado == 0);

        do{
            //Informa o resultado final.
            system("cls");
            printf("FIM DE JOGO!\nTabuleiro final:\n\n");
            exibeTabuleiro(tabuleiro);
            if(resultado == 1)
                printf("\n\nA partida deu empate. Pois ambos os jogadores formaram uma sequência de 5 aliados concecutivos.\n");
            else if(resultado == 2)
                printf("\n\nO jogador 'O' venceu, pois conseguiu formar uma sequência de 5 aliados concecutivos.\n");
            else if(resultado == 3)
                printf("\n\nO jogador 'X' venceu, pois conseguiu formar uma sequência de 5 aliados concecutivos.\n");
            else if(resultado == 4){
                    if(player == 'X')
                        printf("\n\nO jogador 'X' venceu, pois o jogador 'O' não tem mais movimentos disponíveis.\n");
                    else
                        printf("\n\nO jogador 'O' venceu, pois o jogador 'X' não tem mais movimentos disponíveis.\n");
            }

            //Pergunta se o jogador quer sair ou voltar para o menu.
            printf("\n\nAperte 1 para retornar ao menu ou 2 para sair: ");
            fgets(menu, 3, stdin);
            fflush(stdin);
            if((menu[0] == '1' || menu[0] == '2') && menu[1] == '\n')
                break;
        }while(1);

        //Executa a ação do jogador.
        if(menu[0] == '1')
            system("cls");
        else{
            //Encerra o jogo.
            for(i = 0; i < 3; i++){
                system("cls");
                printf("\n\nEncerrando");
                for(j = 0; j < 5; j++){
                    printf(".");
                    usleep(500000);
                }
            }
                printf("\n\n");
                return 0;
            }
        }
}

//Exibe as regras.
void exibeRegras(){

    system("cls");
    printf("REGRAS:\n\n1 - Cada jogador tem 5 peças.\n\n");
    printf("2 - Os jogadores alternam seus turnos durante a partida, durante seu turno o jogador deverá escolher\numa peça e movê-la.\n\n");
    printf("3 - O movimento é igual à rainha do xadrez, seja seguindo uma coluna, uma linha ou diagonal do tabuleiro.\n\n");
    printf("4 - Não é permitido pular sobre suas peças ou sobre as peças do adversário.\n\n");
    printf("5 - Após o movimento, é deixado um aliado na posição onde o jogador estava.\n\n");
    printf("6 - Após cada movimento, os aliados que o jogador passa por cima se invertem, isto é, seus alidos viram\ndo oponente e os aliados do oponente viram seus.\n\n");
    printf("7 - Ganha aquele que conseguir formar uma sequência de 5 aliados concecutivos, podendo ser na horizontal,\nvertical ou diagonal.\n\n");
    printf("8 - É possível alcançar a vitória também caso o oponente não possua mais movimentos válidos.\n\n\n\n\n");
    system("pause");
    system("cls");

}


//Exibe o tabuleiro.
void exibeTabuleiro(char tab[TAM_TAB][TAM_TAB]){
    int i, j;

    printf("-------------------------------------\n");
    for(i=0; i<TAM_TAB; i++){
        for(j=0; j<TAM_TAB; j++){
            if(j == 0 && i != 0)
                printf("----+---+---+---+---+---+---+---+----\n");
            printf("| %c ", tab[i][j]);
        }
        printf("|  %d\n", i+1);
    }

    printf("-------------------------------------\n  A   B   C   D   E   F   G   H   I\n\n");
}


//Retorna a peça que está na posição selecionada.
char naCasa(char tab[TAM_TAB][TAM_TAB], char *posicaoPeca){
    int i, j;

    i=(posicaoPeca[0]-'1');
    j=(posicaoPeca[1]-'a');


    return tab[i][j];
}


//Verifica se a posição escolhida contém uma peça válida.
int validaPeca(char tab[TAM_TAB][TAM_TAB], char *posicaoPeca, char jogador){
    int i, j;

    //i e j recebem a posição digitada convertida em número.
    i = (posicaoPeca[0] - '1');
    j = (posicaoPeca[1] - 'a');

    //Verifica se a posição escolhida está dentro do tabuleiro e se contém uma peça do jogador.
    if(jogador != naCasa(tab, posicaoPeca) || (i < 0 || i > 8) || (j < 0 || j > 8) || posicaoPeca[2] != '\n')
        return 1;
    else
        return 0;
}


//Verifica se o movimento é válido.
int validaMovimento(char tab[TAM_TAB][TAM_TAB], char *posicaoPeca, char *novaPosicao){

    char i, j, k, l;
    int m, n, a, c, d;

    //i e j recebem a posição da peça.
    //k e l recebem a nova posição.
    //m recebe a quantidade de movimento na vertical.
    //n recebe a quantidade de movimento na horizontal.
    // c e d recebem a posição  da peça convertida em número.
    i = posicaoPeca[0];
    j = posicaoPeca[1];
    k = novaPosicao[0];
    l = novaPosicao[1];
    m = i - k;
    n = j - l;
    c = i - '1';
    d = j - 'a';


    //RESPECTIVAMENTE:
    //Verifica se a posição digitada está dentro do tabuleiro.
    //Verifica se a posição digitada está vazia.
    //Verifica se algum movimento foi realizado.
    //OS PRÓXIMOS VERIFICAM O TIPO DE MOVIMENTO E SE A PEÇA PULA OUTRA PEÇA.
    //Verifica se o movimento na horizontal da direita para a esquerda é válido.
    //Verifica se o movimento na horizontal da esquerda para a direita é válido.
    //Verifica se o movimento na vertical de baixo para cima é válido.
    //Verifica se o movimento na vertical de cima para baixo é válido.
    //Verifica se o movimento na diagonal principal da esquerda para a direita é válido.
    //Verifica se o movimento na diagonal principal da direita para a esquerda é válido.
    //Verifica se o movimento na diagonal secundária da esquerda para a direita é válido.
    //Verifica se o movimento na diagonal secundária da direita para a esquerda é válido.
    //O último verifica se o movimento ocorre na vertical, horiontal ou diagonal.
    if(k - '1' < 0 || k - '1' > 8 || l - 'a' < 0 || l - 'a' > 8 || novaPosicao[2] != '\n')
        return 1;
    else if(m == 0 && n == 0)
        return 2;
    else if(naCasa(tab, novaPosicao)!=' ')
        return 3;
    else if(m == 0 && n > 0){
              for(a = 1; a < n; a++){
                  if(tab[c][d - a] == 'X' || tab[c][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m == 0 && n < 0){
              for(a = n+1; a < 0 ; a++){
                  if(tab[c][d - a] == 'X' || tab[c][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m > 0 && n == 0){
              for(a = 1; a < m; a++){
                  if(tab[c - a][d] == 'X' || tab[c - a][d] == 'O')
                      return 4;
              }
              return 0;
      } else if(m < 0 && n == 0){
              for(a = m+1; a < 0; a++){
                  if(tab[c - a][d] == 'X' || tab[c - a][d] == 'O')
                      return 4;
              }
              return 0;
      } else if(m == n && n > 0){
              for(a = 1; a < n; a++){
                  if(tab[c - a][d - a] == 'X' || tab[c - a][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m == n && n < 0){
              for(a = n+1; a < 0; a++){
                  if(tab[c - a][d - a] == 'X' || tab[c - a][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m == n*(-1) && n > 0){
              for(a = 1; a < n; a++){
                  if(tab[c + a][d - a] == 'X' || tab[c + a][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m == n*(-1) && n < 0){
              for(a = n+1; a < 0; a++){
                  if(tab[c + a][d - a] == 'X' || tab[c + a][d - a] == 'O')
                      return 4;
              }
              return 0;
      } else if(m != n && m != n*(-1))
           return 5;
        else
           return 0;
}


//Realiza a conversão dos aliados.
void verificaConversao(char tab[TAM_TAB][TAM_TAB], char *posicaoPeca, char *novaPosicao){

    int m, n, a, c, d;

    //m recebe a quantidade de movimento na vertical.
    //n recebe a quantidade de movimento na horizontal.
    // c e d recebem a posição  da peça convertida em número.
    //a é um contador para o for.
    c = posicaoPeca[0] - '1';
    d = posicaoPeca[1] - 'a';
    m = posicaoPeca[0] - novaPosicao[0];
    n = posicaoPeca[1] - novaPosicao[1];

    //RESPECTIVAMENTE:
    //Cada if verifica o tipo do movimento, se foi na horizontal, vertical ou diagonal.
    //Os dois primeiros convertem os aliados que estão na horizontal.
    //Os dois seguintes convertem os aliados que estão na vertical.
    //Os dois seguintes convertem os aliados que estão na diagonal principal.
    //Os dois útimos convertem os aliados que estão na diagonal secundária.
    if(m == 0 && n > 0){
            for(a = 1; a < n; a++){
                  if(tab[c][d - a] == '×')
                      tab[c][d - a] = 'o';
                  else if(tab[c][d - a] == 'o')
                      tab[c][d - a] = '×';
                  else
                      continue;
              }
              return;
      } else if(m == 0 && n < 0){
              for(a = n+1; a < 0 ; a++){
                    if(tab[c][d - a] == '×')
                        tab[c][d - a] = 'o';
                    else if(tab[c][d - a] == 'o')
                        tab[c][d - a] = '×';
                    else
                        continue;
              }
              return;
      } else if(m > 0 && n == 0){
              for(a = 1; a < m; a++){
                    if(tab[c - a][d] == '×')
                        tab[c - a][d] = 'o';
                    else if(tab[c - a][d] == 'o')
                        tab[c - a][d] = '×';
                    else
                        continue;
              }
              return;
      } else if(m < 0 && n == 0){
              for(a = m+1; a < 0; a++){
                    if(tab[c - a][d] == '×')
                        tab[c - a][d] = 'o';
                    else if(tab[c - a][d] == 'o')
                        tab[c - a][d] = '×';
                    else
                        continue;
              }
              return;
      } else if(m == n && n > 0){
              for(a = 1; a < n; a++){
                    if(tab[c - a][d - a] == '×')
                        tab[c - a][d - a] = 'o';
                    else if(tab[c - a][d - a] == 'o')
                        tab[c - a][d - a] = '×';
                    else
                        continue;
              }
              return;
      } else if(m == n && n < 0){
              for(a = n+1; a < 0; a++){
                    if(tab[c - a][d - a] == '×')
                        tab[c - a][d - a] = 'o';
                    else if(tab[c - a][d - a] == 'o')
                        tab[c - a][d - a] = '×';
                    else
                        continue;
              }
              return;
      } else if(m == n*(-1) && n > 0){
              for(a = 1; a < n; a++){
                    if(tab[c + a][d - a] == '×')
                        tab[c + a][d - a] = 'o';
                    else if(tab[c + a][d - a] == 'o')
                        tab[c + a][d - a] = '×';
                    else
                        continue;
              }
              return;
      } else if(m == n*(-1) && n < 0){
              for(a = n+1; a < 0; a++){
                    if(tab[c + a][d - a] == '×')
                        tab[c + a][d - a] = 'o';
                    else if(tab[c + a][d - a] == 'o')
                        tab[c + a][d - a] = '×';
                    else
                        continue;
              }
              return;
        }
}


//Verifica vitótia.
int verificaVitoria(char tab[TAM_TAB][TAM_TAB], char jogador){

    //Essa função é dividida em duas partes.
    //A primeira verifica se a vitória ocorreu pela acumulação de aliados concecutivos.
    //A segunda verifica se a vitória ocorreu pela falta de movimentos do oponente.
    int i, j, k, l, vitoriaX=0, vitoriaO=0;
    char player, peca[TAM_POS], movimento[TAM_POS];

    //verifica qual peça deve ser testada na segunda parte da função.
    if(jogador == 'X')
        player = 'O';
    else
        player = 'X';

    //PRIMEIRA PARTE DA FUNÇÃO

    //Os primeiros 4 duplos FOR verificam se o jogador X venceu pela acumulação de 5 aliados concecutivos.

    //Verifica se a acumulação de 5 aliados ocorreu na horizontal.
    for(i = 0; i < TAM_TAB; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][j] == '×' && tab[i][j + 1] == '×' && tab[i][j + 2] == '×' && tab[i][j + 3] == '×' && tab[i][j + 4] == '×')
                vitoriaX = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na vertical.
    for(i = 0; i < 5; i++){
        for(j = 0; j < TAM_TAB; j++){
            if(tab[i][j] == '×' && tab[i + 1][j] == '×' && tab[i + 2][j] == '×' && tab[i + 3][j] == '×' && tab[i + 4][j] == '×')
                vitoriaX = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na diagonal principal.
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][j] == '×' && tab[i + 1][j + 1] == '×' && tab[i + 2][j + 2] == '×' && tab[i + 3][j + 3] == '×' && tab[i + 4][j + 4] == '×')
                vitoriaX = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na diagonal secundária.
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][8 - j] == '×' && tab[i + 1][8 - (j + 1)] == '×' && tab[i + 2][8 - (j + 2)] == '×' && tab[i + 3][8 - (j + 3)] == '×' && tab[i + 4][8 - (j + 4)] == '×')
                vitoriaX = 1;
        }
    }


    //Os últimos 4 duplos FOR verifcam se o jogador O venceu pela acumulação de 5 peões concecutivos.

    //Verifica se a acumulação de 5 aliados ocorreu na horizontal.
    for(i = 0; i < TAM_TAB; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][j] == 'o' && tab[i][j + 1] == 'o' && tab[i][j + 2] == 'o' && tab[i][j + 3] == 'o' && tab[i][j + 4] == 'o')
                vitoriaO = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na vertical.
    for(i = 0; i < 5; i++){
        for(j = 0; j < TAM_TAB; j++){
            if(tab[i][j] == 'o' && tab[i + 1][j] == 'o' && tab[i + 2][j] == 'o' && tab[i + 3][j] == 'o' && tab[i + 4][j] == 'o')
                vitoriaO = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na diagonal principal.
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][j] == 'o' && tab[i + 1][j + 1] == 'o' && tab[i + 2][j + 2] == 'o' && tab[i + 3][j + 3] == 'o' && tab[i + 4][j + 4] == 'o')
                vitoriaO = 1;
        }
    }

    //Verifica se a acumulação de 5 aliados ocorreu na diagonal secundária.
    for(i = 0; i < 5; i++){
        for(j = 0; j < 5; j++){
            if(tab[i][8 - j] == 'o' && tab[i + 1][8 - (j + 1)] == 'o' && tab[i + 2][8 - (j + 2)] == 'o' && tab[i + 3][8 - (j + 3)] == 'o' && tab[i + 4][8 - (j + 4)] == 'o')
                vitoriaO = 1;
        }
    }

    //Verifica se a condição de vitória por acúmulo de aliados concecutivos aconteceu.
    if(vitoriaO == 1 && vitoriaX == 1)
        return 1;
    else if(vitoriaO == 1)
        return 2;
    else if(vitoriaX == 1)
        return 3;

    //SEGUNDA PARTE DA FUNÇÃO

    //Procura todas as peças do jogador e verifica se existe movimento possível em alguma direção.
    //Os dois primeiros FOR verificam se há movimento disponível na horizontal.
    //Os dois FOR seguintes verificam se há movimento disponível na vertical.
    //Os dois FOR seguintes verificam se há movimento disponível na diagonal pricipal.
    //Os dois FOR seguintes verificam se há movimento disponível na diagonal secundária.
    //Caso seja detectado algum movimento disponível a função encerra e retorna o valor 0.
    //Caso não seja detectado nenhum movimento disponível a função retorna o valor 4.
    peca[2] = '\n';
    movimento[2] = '\n';
    for(i = 0; i < TAM_TAB; i++){
        for(j = 0; j < TAM_TAB; j++){
            if(tab[i][j] == player){
                peca[0] = i + '1';
                peca[1] = j + 'a';
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0];
                    movimento[1] = peca[1] + l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                    for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0];
                    movimento[1] = peca[1] - l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] + l;
                    movimento[1] = peca[1];
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] - l;
                    movimento[1] = peca[1];
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] + l;
                    movimento[1] = peca[1] + l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] - l;
                    movimento[1] = peca[1] - l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] - l;
                    movimento[1] = peca[1] + l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
                for(l = 1; l <= TAM_TAB - 1; l++){
                    movimento[0] = peca[0] + l;
                    movimento[1] = peca[1] - l;
                    if(validaMovimento(tab, peca, movimento) == 0)
                        return 0;
                }
            }
        }
    }
    return 4;
}
