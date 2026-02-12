#ifndef INP
#define INP
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
//linkedList
void *getCurInput();
//void addControl(char *inp, void(*n_func)(float));
void clearInput();
void freeCurInput();
//InpMap *makeInputMap(char *str, void (*func)(void));
void tmpFunc(void*, float);
void getMouseBack();

void freeInput();

#endif
