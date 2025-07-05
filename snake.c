#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>

#define TAM_CELULA 20
#define MAX_TAMANHO 100

typedef struct {
	int x, y;
} Ponto;

typedef struct {
	Ponto corpo[MAX_TAMANHO];
	int tamanho;
	int direcao;
    int speed;
} Cobrinha;

enum Direcao{
    UP,
    DOWN,
    RIGHT,
    LEFT
};

Cobrinha player;

void aumentarPlayer() {
    if (player.tamanho < MAX_TAMANHO) {
        player.corpo[player.tamanho] = player.corpo[player.tamanho - 1];
        player.tamanho++;
    }
}

void moverCobrinha(int screenWidth, int screenHeight) {
    for (int i = player.tamanho - 1; i > 0; i--) {
        player.corpo[i] = player.corpo[i-1];
    }

    switch (player.direcao) {
        case UP:
            player.corpo[0].y++;
            break;
        case DOWN:
            player.corpo[0].y--;
            break;
        case LEFT:
            player.corpo[0].x--;
            break;
        case RIGHT:
            player.corpo[0].x++;
            break;
    }

    int num_celulas_x = screenWidth / TAM_CELULA;
    int num_celulas_y = screenHeight / TAM_CELULA;

    if (player.corpo[0].x < 0) player.corpo[0].x = num_celulas_x - 1;
    if (player.corpo[0].x >= num_celulas_x) player.corpo[0].x = 0;
    if (player.corpo[0].y < 0) player.corpo[0].y = num_celulas_y - 1;
    if (player.corpo[0].y >= num_celulas_y) player.corpo[0].y = 0;
}

int hasLose(){
    for (int i = player.tamanho - 1; i > 1; i--) {
        if(player.corpo[0].x == player.corpo[i].x && 
            player.corpo[0].y == player.corpo[i].y)
            return 1;
    }
    return 0;
}
