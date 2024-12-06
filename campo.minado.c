#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_LINHAS 30
#define MAX_COLUNAS 30

//Definicao de matrizes globais utilizadas no codigo,sao usadas "matriz_do_jogo" para derterminar a matriz criada para o campo.
//Matriz global "matriz_visivel" eh a matriz que sera visivel ao jogador e simula o jogo.

int **matriz_do_jogo;
int **matriz_visivel;
int linhas, colunas, minas;

// Função para colocar minas na matriz do jogo,utiliza a funcao rand() que determina numeros em locais aleatorios do campo minado.
//O if verifica se na coordenada ja tem uma mina,alocando-a dependendo do caso e ao final incrementa o contador das minas colocadas.
//Funcao verifica os locais "x,y"
void coloca_minas(int linhas, int colunas, int minas) {
    srand(time(NULL));
    int minas_colocadas = 0;
    while (minas_colocadas < minas) {
        int x = rand() % linhas;
        int y = rand() % colunas;
        if (matriz_do_jogo[x][y]!= -1){
            matriz_do_jogo[x][y]= -1;
            minas_colocadas++;
        }
    }
}

//Funcao libera_matriz: libera a memoria alocada dinamicante para a matriz do jogo e nesse caso,percorre cada linha da matriz e libera a memoria indivualmente.
void libera_matriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}
// Função para contar e atualizar as minas adjacentes ao redor de minas especificas selecionadas.
//A funcao indica as minas proximas e percorre os lados.
//Como a matriz eh bidimensonal,utilizo ladox,ladoy para representar os pontos da grade.
//Os lados sao reiniciados e de acordo com as minas alocadas ele calcula a posicao que ficara os numeros.
//A funcao verifica se nao ha mina e incrementa ao lado de acordo com o resultado da funcao rand(),que determina isso.
void atualiza_minas_adjacentes(int x, int y, int linhas, int colunas) {
   int ladoX = -1;
    int ladoY = -1;

    while (ladoX <= 1) {
        ladoY = -1;
        while (ladoY <= 1) {
            if (ladoX == 0 && ladoY == 0) {
                ladoY++;
                continue;
            }
            int novoX = x + ladoX;
            int novoY = y + ladoY;


            if (novoX >= 0 && novoX < linhas && novoY >= 0 && novoY < colunas && matriz_do_jogo[novoX][novoY] != -1) {
                matriz_do_jogo[novoX][novoY]++;
            }
            ladoY++;
        }
        ladoX++;
    }
}

//A funcao percorre a matriz e quando encontra "-1"(mina),chama a funcao "atualiza_minas_adjacentes" para realizar a implementacao das numeracoes.
void minas_adjacentes() {
    for (int x = 0; x < linhas; x++) {
        for (int y = 0; y < colunas; y++) {
            if (matriz_do_jogo[x][y] == -1) {
                atualiza_minas_adjacentes(x, y,linhas,colunas);
            }
        }
    }
}
// Inicializa a matriz do jogo e aloca memória primeiramente.
// Inicializa as células do jogo,linha e coluna como 0 e chama a funcao coloca_minas para colocar minas aleatoriamente.
//Chama a funcao "minas_adjacentes" para calcular as minas adjacentes e iniciar a matriz visivel do jogo.
//Inicia a matriz que sera criada com base no numero de linhas e colunas fornecidas pelo usuario.
int inicia_matriz_do_jogo() {
    matriz_do_jogo = (int **)malloc(linhas * sizeof(int *));
    if (matriz_do_jogo == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(1);
    }

    for (int i = 0; i < linhas; i++) {
        matriz_do_jogo[i] = (int *)malloc(colunas * sizeof(int));
        if (matriz_do_jogo[i] == NULL) {
            fprintf(stderr, "Erro na alocação de memória\n");
            exit(1);
        }
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            matriz_do_jogo[i][j] = 0;
        }
    }


    coloca_minas(linhas, colunas, minas);

    minas_adjacentes( );
}


//A funcao verifica se a coordenada inserida eh valida e verifica as coordenadas inseridas.
//Imprime a matriz com as coorenadas reveladas ou com as minas colocadas.
void imprime_matriz(int **matriz, int linhas, int colunas, int revelar) {
    printf("   ");
    for (int j = 0; j < colunas; j++) {
        printf("%3d", j + 1);
    }
    printf("\n");

    for (int i = 0; i < linhas; i++) {
        printf("%2d ", i + 1);
        for (int j = 0; j < colunas; j++) {
            if (revelar) {
                printf("%3d", matriz[i][j]);
            } else {
                if (matriz[i][j] == -2) {
                    printf("  x");
                } else {
                    printf("%3d", matriz[i][j]);
                }
            }
        }
        printf("\n");
    }
}


// Função do jogo que aloca a matriz e inicializa a matriz visivel ao usuario.
// Lê as dimensões do campo minado e o número de minas.
//Inicializa como matriz nao revelada(-2).
//Verifica se as dimensões inseridas são validas.
//Verifica se a funcao ja teve a coordenada revelada.
//Verifica se o jogador ganhou a partida("parabens vc eh fera").
//Verifica se o usuario perdeu ("game over").
//Por fim,libera as matrizes alocadas dinamicamente.

void jogo_inicia() {
    \
    matriz_visivel = malloc(linhas * sizeof(int *));
    if (matriz_visivel == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(1);
    }

    for (int i = 0; i < linhas; i++) {
        matriz_visivel[i] = malloc(colunas * sizeof(int));
        if (matriz_visivel[i] == NULL) {
            fprintf(stderr, "Erro na alocação de memória\n");
            exit(1);
        }
        for (int j = 0; j < colunas; j++) {
            matriz_visivel[i][j] = -2;
        }
    }

    int coordenadas_reveladas = 0;
    int total = linhas * colunas - minas;


    while (1) {

        printf("\nCampo Minado:\n");
        imprime_matriz(matriz_visivel, linhas, colunas, 0);


        int x, y;
        printf("Digite coordenada(linha,coluna): ");

        while(scanf("%d,%d", &x, &y) != 2) {
            printf("insira dois números separados por vírgula.\n");
            while(getchar() != '\n');
            printf("digite sua coordenada (linha,coluna): ");
        }

        x--; y--;

        if (x < 0 || x >= linhas || y < 0 || y >= colunas) {
            printf("essas coordenadas sao invalidas\n");
            continue;
        }


        if (matriz_visivel[x][y] != -2) {
            printf("Coordenada já  revelada,escolha outra\n");
            continue;
        }


        if (matriz_do_jogo[x][y] == -1) {
            printf("game Over\n");
            printf("Matriz Completa:\n");
            imprime_matriz(matriz_do_jogo, linhas, colunas, 1);
            break;
        }


        matriz_visivel[x][y] = matriz_do_jogo[x][y];
        coordenadas_reveladas++;


        if (coordenadas_reveladas == total) {
            printf("parabens,vc eh fera\n");
            printf("matriz Completa:\n");
            imprime_matriz(matriz_do_jogo, linhas, colunas, 1);
            break;
        }
    }


    libera_matriz(matriz_do_jogo, linhas);
    libera_matriz(matriz_visivel, linhas);
}

// Função main consulta o nivel de dificuldade que o jogador deseja,verifica se eh valida ou nao.
//Funciona a inicializacao do jogo com as dimensoes e numero de minas do nivel escolhido.
//Inicia o jogo e para o loop principal do jogo.
int main() {

        int nivel;

        while(1) {
            printf("Escolha o nível do jogo:\n1 - Fácil\n2 - Médio\n3 - Difícil\n");

            if (scanf("%d", &nivel) != 1) {
                printf("Por favor, digite apenas números (1, 2 ou 3).\n");

                scanf("%*c");     
                continue;
            }

            switch(nivel) {
                case 1:
                    linhas = 10;
                    colunas = 10;
                    break;

                case 2:
                    linhas = 20;
                    colunas = 20;
                    break;

                case 3:
                    linhas = MAX_LINHAS;
                    colunas = MAX_COLUNAS;
                    break;

                default:
                    printf("Nível inválido. Escolha 1, 2 ou 3.\n");
                    continue;
            }

            minas = (int)((linhas * colunas) * 0.15);
            break;
        }

        inicia_matriz_do_jogo();
        jogo_inicia();

        return 0;
}