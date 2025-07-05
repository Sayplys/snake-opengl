#ifndef UTILS_H
#define UTILS_H

void desenhaQuadrado(int x, int y, int tamCelula);
void desenhaTexto(float x, float y, const char *string, int winWidth, int winHeight);
void storeScore(int score, char* fileName, int limit);
int* readScore(const char* fileName, int limit);

#endif