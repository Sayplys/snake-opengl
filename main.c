#include <stdio.h>
#include <string.h>
#include <GL/glut.h>
#include "utils.h"
#include "game.h"

#define LARGURA_JANELA 600
#define ALTURA_JANELA 600

void teclado(unsigned char tecla, int x, int y) {
  if (gameState == MENU) { // Handle difficulty selection in MENU state
        int chosenDifficulty = 0;
        switch (tecla) {
            case '1': chosenDifficulty = 1; break; // Worm (Slowest)
            case '2': chosenDifficulty = 2; break; // Snake (Normal)
            case '3': chosenDifficulty = 3; break; // Jiboia (Fast)
            case '4': chosenDifficulty = 4; break; // Jörmungandr (Extreme)
        }
        if (chosenDifficulty > 0) {
            inicializar(LARGURA_JANELA, ALTURA_JANELA, chosenDifficulty);
        }
    } else if (gameState == GAME) {
        switch (tecla) {
            case 'a':
            case 'A':
                if (player.direcao != RIGHT) player.direcao = LEFT;
                break;
            case 'd':
            case 'D':
                if (player.direcao != LEFT) player.direcao = RIGHT;
                break;
            case 'w':
            case 'W':
                if (player.direcao != DOWN) player.direcao = UP;
                break;
            case 's':
            case 'S':
                if (player.direcao != UP) player.direcao = DOWN;
                break;
        }
    } else if (gameState == END) {
        if (tecla == 13) { // Enter
            gameState = MENU;
        }
    }
    if (tecla == 27) { // ESC
        exit(0);
    }
   glutPostRedisplay();
}

void desenha() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LARGURA_JANELA, 0, ALTURA_JANELA);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

   switch(gameState){
      case MENU:
         startMenu(LARGURA_JANELA, ALTURA_JANELA);
         break;
      case END:
         finishMenu(LARGURA_JANELA, ALTURA_JANELA);
      case GAME:
          glColor3f(1.0, 0.0, 0.0);
         desenhaQuadrado(comida.x, comida.y, TAM_CELULA);

         for (int i = 0; i < player.tamanho; i++) {
            if (i == 0) { 
                  glColor3f(0.0, 1.0, 0.0);
            } else {
                  glColor3f(0.0, 0.0, 1.0);
            }
            desenhaQuadrado(player.corpo[i].x, player.corpo[i].y, TAM_CELULA);
         }

         glColor3f(1.0, 1.0, 1.0);
         char scoreText[50];
         sprintf(scoreText, "Pontos: %d", score);
         desenhaTexto(10, ALTURA_JANELA - 30, scoreText, LARGURA_JANELA, ALTURA_JANELA);

         if (difficulty > 2) {
            char timerText[50];
            sprintf(timerText, "Proxima comida em: %.1f s", (float)timeUntilNextFoodSpawn / 1000.0);
            desenhaTexto(10, ALTURA_JANELA - 50, timerText, LARGURA_JANELA, ALTURA_JANELA);
         }
         break;
   }

   glutSwapBuffers();
}

int main ( int argc, char** argv ) {
    srand(time(NULL)); 

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_RGB | GLUT_DOUBLE );
    glutInitWindowSize( LARGURA_JANELA, ALTURA_JANELA );
    glutCreateWindow( "Jogo da Cobrinha" );
    
    glutDisplayFunc( desenha);
    glutKeyboardFunc( (void(*)(unsigned char, int, int))teclado );
    glutTimerFunc( 0, timer, 0 );

    glutMainLoop();
    return 0;
}