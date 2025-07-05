#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>

void desenhaQuadrado(int x, int y, int tamCelula) {
    glBegin(GL_QUADS);
        glVertex2f(x * tamCelula, y * tamCelula);
        glVertex2f((x + 1) * tamCelula, y * tamCelula);
        glVertex2f((x + 1) * tamCelula, (y + 1) * tamCelula);
        glVertex2f(x * tamCelula, (y + 1) * tamCelula);
    glEnd();
}

void desenhaTexto(float x, float y, const char *string, int winWidth, int winHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, winWidth, 0, winHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y);

    for (const char *c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

int compareScoresAscending(const void *a, const void *b) {
    int scoreA = *(const int *)a;
    int scoreB = *(const int *)b;

    if (scoreA == -1 && scoreB != -1) return 1;
    if (scoreA != -1 && scoreB == -1) return -1;
    if (scoreA == -1 && scoreB == -1) return 0;

    return scoreB - scoreA;
}

int* readScore(const char* fileName, int limit) {
    FILE *file;
    int* scores = (int*)malloc(sizeof(int) * limit);
    if (scores == NULL) {
        perror("Failed to allocate memory for scores in readStore");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < limit; i++) {
        scores[i] = -1;
    }

    file = fopen(fileName, "r");
    if (file == NULL) {
        return scores;
    }

    char line[256];
    if (fgets(line, sizeof(line), file) != NULL) {
        char* token = strtok(line, ";");
        int i = 0;
        while (token != NULL && i < limit) {
            scores[i] = atoi(token);
            token = strtok(NULL, ";");
            i++;
        }
    }

    fclose(file);
    return scores;
}

void storeScore(int score, char* fileName, int limit) {
    FILE *file;
    int* scores = readScore(fileName, limit);
    if (scores == NULL) {
        perror("Failed to read scores in storeScore");
        exit(EXIT_FAILURE);
    }

    int lowestScoreIndex = -1;
    int lowestScoreValue = -1;

    int added = 0;
    for (int i = 0; i < limit; i++) {
        if (scores[i] == -1) {
            scores[i] = score;
            added = 1;
            break;
        }
        if (scores[i] != -1) {
            if (lowestScoreIndex == -1 || scores[i] < lowestScoreValue) {
                lowestScoreValue = scores[i];
                lowestScoreIndex = i;
            }
        }
    }

    if (!added && score > lowestScoreValue) {
        scores[lowestScoreIndex] = score;
    }

    qsort(scores, limit, sizeof(int), compareScoresAscending);

    file = fopen(fileName, "w");
    if (file == NULL) {
        perror("Failed to open score file for writing in storeScore");
        free(scores);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < limit; i++) {
        if (scores[i] != -1) {
            fprintf(file, "%d;", scores[i]);
        }
    }

    fclose(file);
    free(scores);
}