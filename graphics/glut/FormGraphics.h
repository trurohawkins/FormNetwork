#include "form/Form.h"
#include "graphicsSource/Graphics.h"
//#include "../graphicsSource/Graphics.h"
void drawForm(Form * f, int x, int y, int size);
void drawWorld();
void setCenter(float cp[2]);
void setInitTime(int newTime);
void setGrid(bool on);
void keyDown(unsigned char key, int mx, int my);
void keyUp(unsigned char key, int mx, int my);
void update();
//void drawFunc();
