#ifndef COBRA_H
#define COBRA_H

#include <stdlib.h>
#include <time.h>

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

extern Cobrinha player;

void aumentarPlayer();

void moverCobrinha(int screenWidth, int screenHeight);

int hasLose();

#endif