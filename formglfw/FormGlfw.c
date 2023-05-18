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
float bgColor[4];
bool running = true;

void initFormGlfw() {
	srand(time(NULL));
	initDirections();
	// 0 means use monitors x and y and start in full screen mode
	int windowX = 960;
	int windowY = 640;
	initializeGLFW(windowX, windowY);
	//setAspectRatio(16, 9);
	//setScreenMax(worldX);
	//sizeScreen(windowX);
	glfwSetJoystickCallback(joystickCallback);
	initJoyList();
	initText();
	initTexInts();
	initInput();
	//initAudio();
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
	//bgColor = {0, 0 ,0, 0};
	for (int i = 0; i < 4; i++) {
		bgColor[i] = 0;
	}	
}

void FormLoop(void (game)(void)) {
	Screen *screen = getWindow();
	while(!glfwWindowShouldClose(screen->window) && running) {
		glfwPollEvents();
		checkControllerInput();
		processKeys();
		/*
		if(glfwGetKey(screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(screen->window, 1);
		}
		*/
		glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game();
		cleanUpPlayedAudio();
		glfwSwapBuffers(screen->window);
	}
}

void stopLoop() {
	running =  false;
}

void setBackgroundColor(float r, float g, float b, float a) {
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
	bgColor[3] = a;
}

void createWorld(int worldX, int worldY, int frameX, int frameY) {
	makeWorld(worldX, worldY);
	initWorldView(frameX, frameY);
	initWorldDrawing();
}

void setAnim(Form *f, Anim *a) {
	/*
	f->anim = a;
	f->aCount = 1;
	addAnim(a);
	*/
	Anim **as = calloc(f->aCount + 1, sizeof(Anim*));
	Anim **cur = f->anim;
	for (int i = 0; i < f->aCount; i++) {
		//((Anim*)(f->anim))[i] 
		as[i] = cur[i];//((Anim**)(f->anim))[i];
	}
	if (f->aCount > 0) {
		free(f->anim);
	}
	as[f->aCount] = a;
	f->aCount++;
	f->anim = as;
	addAnim(a);
}

void changeSprites(Form *f, int index) {
	Anim **a = f->anim;
	for (int i = 0; i < f->aCount; i++) {
		changeSprite(a[i], index);
	}
}

void setOffsets(Form *f, float x, float y) {
	Anim **a = f->anim;
	for (int i = 0; i < f->aCount; i++) {
		setOffset(a[i], x, y);
	}
}

void setInverts(Form *f, int axis, bool flipped) {
	Anim **a = f->anim;
	for (int i = 0; i < f->aCount; i++) {
		setInvert(a[i], axis, flipped);
	}
}

void setRotos(Form *f, int roto) {
	Anim **a = f->anim;
	for (int i = 0; i < f->aCount; i++) {
		setRoto(a[i], roto);
	}
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
	endAudio();
	endGlfw();
}
