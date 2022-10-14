#include "FormGlfw.h"
#include "Player.c"
#include "god.c"
#include "WorldView.c"
#include "DrawWorld.c"
#include "../controllerDB.c"

bool doGL = true;

int frameX = 50;
int frameY = 50;
int defaultFrameY = -1;
int defaultFrameX = -1;

void initFormGlfw() {
	srand(time(NULL));
	initDirections();
	// 0 means use monitors x and y and start in full screen mode
	int windowX = 0;//960;
	int windowY = 0;//640;
	initializeGLFW(windowX, windowY);
	//setAspectRatio(16, 9);
	//setScreenMax(worldX);
	//sizeScreen(windowX);
	glfwSetJoystickCallback(joystickCallback);
	initJoyList();
	initText();
	initTexInts();
	initInput();
	makeTextureManager();
	initUILists();
	makePlayerManager();
	makeAnimList();
	initTileSets();
	Camera *defCam = makeCamera();
	setCamera(defCam);
	free(defCam);
	Screen *s = getWindow();
	glfwWindowSizeCallback(s->window, s->width, s->height);
	glfwUpdateGamepadMappings(gamecontrollerdb);
}

void createWorld(int worldX, int worldY, int frameX, int frameY) {
	makeWorld(worldX, worldY);
	initWorldView(frameX, frameY);
	initWorldDrawing();
}

void setAnim(Form *f, Anim *a) {
	f->anim = a;
	addAnim(a);
}

void exitGame() {
	printf("exiting\n");
	freeUILists();
	deleteAnimList();
	deleteTextureManager();
	freePlayerManager();
	freeWorld();
	freeJoyList();
	freeInput();
	freeRecipes();
	glfwTerminate();
}
