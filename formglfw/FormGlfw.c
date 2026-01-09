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
bool freeze = false;
bool printFPS = false;

void initFormGlfw() {
	srand(time(NULL));
	initDirections();
	// 0 means use monitors x and y and start in full screen mode
	int windowX = 960;
	int windowY = 640;
	initializeGLFW(windowX, windowY);
	/*
	FILE* f = fopen("gamecontrollerdb.txt", "rb");
	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	rewind(f);

	char* data = malloc(size + 1);
	fread(data, 1, size, f);
	data[size] = '\0';
	fclose(f);

	int added = glfwUpdateGamepadMappings(data);
	printf("added mappings: %i\n", added);
	free(data);
	*/
	glfwUpdateGamepadMappings(gamecontrollerdb);
	glfwSetJoystickCallback(joystickCallback);
	initJoyList();
	initText();
	initTexInts();
	initInput();
	//initAudio();
	makeTextureManager();
	initUILists();
	makeAnimList();
	initTileSets();
	Screen *s = getWindow();
	glfwWindowSizeCallback(s->window, s->width, s->height);
	Camera *defCam = makeCamera();
	setCamera(defCam);
	free(defCam);
	makePlayerManager();
	//bgColor = {0, 0 ,0, 0};
	for (int i = 0; i < 4; i++) {
		bgColor[i] = 0;
	}	
}

void FormLoop(void (game)(void)) {
	Screen *screen = getWindow();
	int frameCount = 0;
	int initialTime = time(NULL);
	while(!glfwWindowShouldClose(screen->window) && running) {
		glfwPollEvents();
		checkControllerInput();
		processKeys(freeze);
		if (!freeze) {
			glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game();
			glfwSwapBuffers(screen->window);
		} // maybe we want to include the next 2 lines, should test with sound
		cleanUpPlayedAudio();
		frameCount++;
		int curTime = time(NULL);
		if (curTime - initialTime >= 1) {
			int fps = frameCount / (curTime - initialTime);
			if (printFPS) {
				printf("FPS: %i\n", fps);
			}
			initialTime = curTime;
			frameCount = 0;
		}
	}
}

void stopLoop() {
	running =  false;
}

void togglePause() {
	freeze = !freeze;
}

void toggleFPS() {
	printFPS = !printFPS;
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
		//removeAnim(cur[i]);
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

TileSet *newTileSet(char *sheet, int rows, int cols, int id) {
	World *w = getWorld();
	WorldView *wv = getDefaultView();
	Anim *newSheet = makeAnimSheet(sheet, 1, rows, cols);
	GLuint dv = makeSpriteVao(1,1);
	animAddVao(newSheet, dv);
	TileSet *ts = makeTileSet(newSheet, wv->frameX, wv->frameY, w->x, w->y, wv->objSX, wv->objSY);
	ts->typeID = id;
	ts->tileSprites = &tileCell;
	return ts;
}

