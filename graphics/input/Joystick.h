#ifndef JOY
#define JOY
#include "../glfwMain.h"
//#include <stdio.h>
//#include <GL/glew.h>//loader of functions for backwards compatibility**
//#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include "../../helper/helper.h"

typedef struct Joypad {
	int jid;
	bool *buttState;
	float *axeState;
} Joypad;

void initJoyList();
Joypad *makeJoypad(int j);
void freeJoyList();
void addJoystick(int j);
void joystickCallback(int jid, int event);
void readJoysticks();
void checkControllerInput();
char *joyButtString(int jid, int butt, int onoff);
void joyAxeString(int jid, int axes, float val);
char *getJoyButtString(int jid, char butt);
char *getJoyAxeString(int jid, char axe);
Joypad *getJoypad(int jid);
void removeJoypad(linkedList **cur, int jid);
void freeJoypad(Joypad *jp);

#endif
