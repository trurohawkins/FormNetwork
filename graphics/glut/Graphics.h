#ifndef GLUT
#define GLUT

#include "GL/freeglut.h"
#include "GlutProcess.h"
//#include "GL/gl.h"

void initializeGLUT(int argc, char **argv, int winSX, int winSY);
int getFX();
int getFY();
void drawSquare(float *color, int x, int y, int size);
void drawLine(float r, float g, float b, int x1, int y1, int x2, int y2);

#endif
