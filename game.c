#include <stdio.h>
#include <time.h>
#include <GL/glut.h>
#include "utils.h"
#include "snake.h"

#define DELAY 200

#define INITIAL_SIZE 3
#define INITIAL_SPEED 1
#define POINTS_PER_FOOD 7

#define SCORE_FILE "score.txt"
#define SCORE_LIMIT 10

enum gameState{
   MENU,
   GAME,
   END
} typedef GameState;

Ponto comida;
GameState gameState = MENU;
int score;

int screenW, screenH, difficulty;

int foodRespawnInterval = 0;
int lastFoodRespawnTime = 0;
int timeUntilNextFoodSpawn = 0;

void gerarComida() {
    int num_celulas_x = screenW / TAM_CELULA;
    int num_celulas_y = screenH / TAM_CELULA;

    comida.x = rand() % num_celulas_x;
    comida.y = rand() % num_celulas_y;

    lastFoodRespawnTime = glutGet(GLUT_ELAPSED_TIME);
    timeUntilNextFoodSpawn =foodRespawnInterval;
}

int eat() {
    if (comida.x == player.corpo[0].x && comida.y == player.corpo[0].y) {
        aumentarPlayer();
        gerarComida();
        score += POINTS_PER_FOOD * difficulty / 2;
        return 1;
    }
    return 0;
}

void inicializar(int screenWidth, int screenHeight, int chosenDifficulty) {
    score = 0;
    gameState = GAME;
    difficulty = chosenDifficulty;

    if ((difficulty - 2) > 0) foodRespawnInterval = 5000 / (difficulty - 2);
    else foodRespawnInterval = 0;

    player.speed = chosenDifficulty;
    player.tamanho = INITIAL_SIZE;
    player.direcao = RIGHT;

    player.corpo[0].x = screenWidth / 2 / TAM_CELULA; 
    player.corpo[0].y = screenHeight / 2 / TAM_CELULA;

    player.corpo[1].x = player.corpo[0].x - 1;
    player.corpo[1].y = player.corpo[0].y;
    player.corpo[2].x = player.corpo[0].x - 2;
    player.corpo[2].y = player.corpo[0].y;

    screenW = screenWidth; 
    screenH = screenHeight;

    gerarComida();
}

void runGame(){
   eat();
   moverCobrinha(screenW, screenH);
   if(hasLose()){
        gameState = END;
        char file[] = SCORE_FILE;
        storeScore(score, file, SCORE_LIMIT);
   }
   glutPostRedisplay();
}

void timer(int value) {
   if(gameState == GAME){
        runGame();
        if(foodRespawnInterval > 0){
            int currentTime = glutGet(GLUT_ELAPSED_TIME);
            if (currentTime - lastFoodRespawnTime >= foodRespawnInterval) {
                gerarComida();
            } else {
                timeUntilNextFoodSpawn = foodRespawnInterval - (currentTime - lastFoodRespawnTime);
            }
        } else {
            timeUntilNextFoodSpawn = 0;
        }
    }

   int delay = DELAY;

   if (player.speed > 0) delay /= player.speed;
   else delay = 1000;

   glutTimerFunc(delay, timer, 0);
}

void startMenu(int currentScreenWidth, int currentScreenHeight){
    char titleText[] = "SNAKE GAME";
    char promptText[] = "CHOOSE DIFFICULTY:";
    char wormText[] = "1. WORM (SLOW)";
    char snakeText[] = "2. SNAKE (NORMAL)";
    char jiboiaText[] = "3. JIBOIA (FAST)";
    char jormungandrText[] = "4. JORMUNGANDR (EXTREME)";

    // Draw Title
    glColor3f(0, 1.0, 0); // Green color
    int titleTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)titleText);
    int titleXPos = (currentScreenWidth - titleTextWidth) / 2;
    desenhaTexto(titleXPos , currentScreenHeight / 2 + 80, titleText, currentScreenWidth, currentScreenHeight);

    // Draw Difficulty Prompt
    glColor3f(1.0, 1.0, 1.0); // White color
    int promptTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)promptText);
    int promptXPos = (currentScreenWidth - promptTextWidth) / 2;
    desenhaTexto(promptXPos, currentScreenHeight / 2 + 40, promptText, currentScreenWidth, currentScreenHeight);

    // Display Difficulty Options
    float optionYOffset = currentScreenHeight / 2; // Starting Y position for the first option

    // Option 1: Worm
    int wormTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)wormText);
    int wormXPos = (currentScreenWidth - wormTextWidth) / 2;
    desenhaTexto(wormXPos, optionYOffset, wormText, currentScreenWidth, currentScreenHeight);
    optionYOffset -= 25; // Move down for next option

    // Option 2: Snake
    int snakeTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)snakeText);
    int snakeXPos = (currentScreenWidth - snakeTextWidth) / 2;
    desenhaTexto(snakeXPos, optionYOffset, snakeText, currentScreenWidth, currentScreenHeight);
    optionYOffset -= 25;

    // Option 3: Jiboia
    int jiboiaTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)jiboiaText);
    int jiboiaXPos = (currentScreenWidth - jiboiaTextWidth) / 2;
    desenhaTexto(jiboiaXPos, optionYOffset, jiboiaText, currentScreenWidth, currentScreenHeight);
    optionYOffset -= 25;

    // Option 4: Jörmungandr
    int jormungandrTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)jormungandrText);
    int jormungandrXPos = (currentScreenWidth - jormungandrTextWidth) / 2;
    desenhaTexto(jormungandrXPos, optionYOffset, jormungandrText, currentScreenWidth, currentScreenHeight);
}

void finishMenu(int currentScreenWidth, int currentScreenHeight){ 
    char finishText[] = "GAME OVER";
    char restartText[] = "PRESS ENTER TO RESTART";
    char scoresText[] = "HIGH SCORES";

    glColor3f(1.0, 0, 0); 
    int finishTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)finishText);
    int finishXPos = (currentScreenWidth - finishTextWidth) / 2;
    desenhaTexto(finishXPos , currentScreenHeight / 2 + 50, finishText, currentScreenWidth, currentScreenHeight);

    glColor3f(1.0, 1.0, 1.0);
    int restartTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)restartText);
    int restartXPos = (currentScreenWidth - restartTextWidth) / 2;
    desenhaTexto(restartXPos, currentScreenHeight / 2 + 20, restartText, currentScreenWidth, currentScreenHeight);

    int scoresTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)scoresText);
    int scoresXPos = (currentScreenWidth - scoresTextWidth) / 2;
    desenhaTexto(scoresXPos, currentScreenHeight / 2 - 30, scoresText, currentScreenWidth, currentScreenHeight);

    int* scores = readScore(SCORE_FILE, SCORE_LIMIT);
    if (scores == NULL) {
        char errorText[] = "Error loading scores.";
        int errorTextWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)errorText);
        int errorXPos = (currentScreenWidth - errorTextWidth) / 2;
        desenhaTexto(errorXPos, currentScreenHeight / 2 - 60, errorText, currentScreenWidth, currentScreenHeight);
        return; 
    }

    float scoreYOffset = currentScreenHeight / 2 - 60;
    glColor3f(0.0, 1.0, 1.0); 
    for(int i = 0; i < SCORE_LIMIT; i++){
        if(scores[i] != -1){ 
            char text[20];
            sprintf(text, "%d", scores[i]);
            
            int scoreNumWidth = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)text);
            int scoreNumXPos = (currentScreenWidth - scoreNumWidth) / 2;
            
            desenhaTexto(scoreNumXPos, scoreYOffset, text, currentScreenWidth, currentScreenHeight);
            scoreYOffset -= 20;
        }
    }
    free(scores);
}