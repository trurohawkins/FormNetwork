#include "GL/freeglut.h"


void glutFunctions(void (*drawFunc)(void), void (*updateFunc)(int),
void (*keyD)(unsigned char, int, int), void (*keyR)(unsigned char, int ,int));
void joystick(unsigned int buttonmask, int x, int y, int z);
void keyDown(unsigned char key, int mx, int my);
void keyUp(unsigned char key, int mx, int my);
void updateGlut();
