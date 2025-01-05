#ifndef INP
#define INP
#include "Joystick.h"
#include "../Anim.h"//so we have access to UI mouse fnctions,
//#include "../glfwMain.h"

typedef struct InpMap
{
	char *input;
	float val;
	void (*func)(void*, float);
} InpMap;

typedef struct inpReceived
{
	char *input;
	float val;
} inpReceived;

InpMap *makeInp(char *inp, void (*n_func)(void*,float));
void freeInp(void *im);
void initInput();
linkedList *getCurInput();
//void addControl(char *inp, void(*n_func)(float));
void takeKeys(GLFWwindow *window, int key, int scancode, int action, int mods);
void takeMouseButt(GLFWwindow *window, int button, int action, int mods);
void takeScroll(GLFWwindow *window, double xoffset, double yoffset);
void clearInput();
void freeCurInput();
//InpMap *makeInputMap(char *str, void (*func)(void));
void tmpFunc(void*, float);
void getMouseBack();

void freeInput();

#endif
