#ifndef GAME_H
#define GAME_H

#include "snake.h"

enum gameState{
   MENU,
   GAME,
   END
} typedef GameState;

extern Ponto comida;
extern GameState gameState;
extern int timeUntilNextFoodSpawn;
extern int score;
extern int difficulty;

void inicializar(int screenWidth, int screenHeight, int chosenDifficulty);
void timer(int value);
void finishMenu(int currentScreenWidth, int currentScreenHeight);
void startMenu(int screenWidth, int screenHeight);
void incressScore();
void gerarComida();

#endif