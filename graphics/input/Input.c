#include "Input.h"
#include "Joystick.c"
linkedList *curInput;

void initInput() {
	curInput = makeList();
	Screen *screen = getWindow();
	glfwSetKeyCallback(screen->window, takeKeys);
	glfwSetMouseButtonCallback(screen->window, takeMouseButt);
	glfwSetScrollCallback(screen->window, takeScroll);
}

InpMap *makeInp(char *inp, void (*n_func)(void*,float)) {
	InpMap *im = (InpMap*)calloc(1, sizeof(InpMap));
	im->input = (char*)calloc(strlen(inp)+1, sizeof(char));
	strcpy(im->input, inp);
	im->func = n_func;
	return im;
}

void freeInp(void *i) {
	InpMap *im = (InpMap*)i;
	free(im->input);
	free(im);
}

linkedList *getCurInput() {
	return curInput;
}

void takeKeys(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action != GLFW_REPEAT) {
		processMenuKeys(key, action); //for concurrent menus and gameplay. See mousebuttons
		inpReceived *ir = (inpReceived*)malloc(sizeof(inpReceived));
		char *keyString = (char *)malloc(4 * sizeof(char));
		keyString[0] = 'K';
		keyString[1] = 48;//add player info later
		if (key == 256) { //escape key
			key = '!';
		} else if (key ==264) {
			key = '_';
		} else if (key == 265) {
			key = '^';
		} else if (key == 262) {
			key = '>';
		} else if (key ==263) {
			key = '<';
		}
		keyString[2] = key;
		keyString[3] = '\0';
		//printf("%s\n", keyString);
		ir->input = keyString;
		ir->val = action;
		addToList(&curInput, ir);
	}
}

void takeMouseButt(GLFWwindow *window, int button, int action, int mods) {
	inpReceived *ir = (inpReceived*)malloc(sizeof(inpReceived));
	char *mouseString = (char *)malloc(4 *sizeof(char));
	mouseString[0] = 'M';
	mouseString[1] = 48;
	mouseString[2] = button + 48;
	mouseString[3] = '\0';
	ir->input = mouseString;
	ir->val = action;
	addToList(&curInput, ir);
	processMenuClicks(button, action);//passes clicks whatever active menus we have going, this way we can have both function run cuncurrently
	//could be fixed by having an Input Manager of some kind
}

void takeScroll(GLFWwindow *window, double xoffset, double yoffset) {
	printf("mouse scroll %d, %d\n", xoffset, yoffset);
}

void clearInput() {
		freeCurInput();
		curInput = makeList();
}

void freeCurInput() {
	linkedList *tmp = curInput;
	while (tmp != NULL) {
		inpReceived *ir = (inpReceived*)tmp->data;
		if (ir != NULL) {
			free(ir->input);
		}
		free(ir);
		linkedList *t = tmp;
		tmp = tmp->next;
		free(t);
	}
}	

void tmpFunc(void *, float val) {
	printf("poopydoopy %f\n", val);
}

void getMouseBack() {
	Screen *screen = getWindow();
	glfwSetMouseButtonCallback(screen->window, takeMouseButt);
	glfwSetScrollCallback(screen->window, takeScroll);
}

void freeInput() {
//	freeList(&players);
	freeList(&curInput);
}
