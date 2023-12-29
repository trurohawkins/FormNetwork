#include "GameLogic.h"

WorldView *cam;
WorldView *wv;
World *w;

PoopGuy **poopers;
int numPlayers = 1;
bool gridOn = false;
bool paused = false;
bool godMode = false;
bool debugDraw = false;
float *godPos;
Menu *pauseMenu;
GOD *god = 0;

int started = false;
int fileVal = 0;
char *fileName = 0;

void menuLogic() {
		drawActiveMenu();
		updateMenu();
		renderTextInput();
		if (fileVal == 1) {
			if (strlen(fileName) > 0) {
				if (loadWorld(fileName)) {
					started = 2;
				} else {
					free(fileName);
					fileName = (char*)calloc(sizeof(char), 100);
				}
			}
		}

}

int mainMenu() {
	Screen *screen = getWindow();
	Menu *startMenu = makeMenu(1000);
	cam = getDefaultView();
	int buttSizeX = 1;
	int buttSizeY = 1;
	float textSize = 0.5;
	Button *loadButt = makeButton("resources/button.png", 0, 2, 1, loadGame);
	setScale(loadButt->graphics->a, buttSizeX, buttSizeY);
	Text *load = makeText("LOAD", textSize, true, 0.5, 0.5, 0.5, 0.5);
	addButtonText(loadButt, load);
	addButtonSubColor(loadButt, 1,1,1,1);
	placeUI(loadButt->graphics, 0, 0.35);
	addButton(startMenu, loadButt);

	Button *newButt = makeButton("resources/button.png", 0, 2, 1, playGame);
	setScale(newButt->graphics->a, buttSizeX, buttSizeY);
	Text *new = makeText("NEW", textSize, true, 0.5, 0.5, 0.5, 0.5);
	addButtonText(newButt, new);
	addButtonSubColor(newButt, 1,1,1,1);
	placeUI(newButt->graphics, 0, 0);
	addButton(startMenu, newButt);


	Button *exitButt = makeButton("resources/button.png", 0, 2, 1, quitGame);
	setScale(exitButt->graphics->a, buttSizeX, buttSizeY);
	Text *exit = makeText("EXIT", textSize, true, 0.5, 0.5, 0.5, 0.5);
	addButtonText(exitButt, exit);
	addButtonSubColor(exitButt, 1,1,1,1);
	placeUI(exitButt->graphics, 0, -0.35);
	addButton(startMenu, exitButt);
	
	fileName = (char*)calloc(sizeof(char), 100);
	
	//textInput(true);

	setMenuActive(startMenu, true);
		FormLoop(menuLogic);
		/*
	while(started == 0) {
		glfwPollEvents();
		checkControllerInput();
		processKeys();
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawActiveMenu();
		updateMenu();
		renderTextInput();
		if (fileVal == 1) {
			if (strlen(fileName) > 0) {
				if (loadWorld(fileName)) {
					started = 2;
				} else {
					free(fileName);
					fileName = (char*)calloc(sizeof(char), 100);
				}
			}
		}
		glfwSwapBuffers(screen->window);
	}
*/
	fileVal = 0;
	setMenuActive(startMenu, false);
	return started;
}


char *getFileName() {
	return fileName;
}

void setGrid(bool state) {
	gridOn = state;
}

void playGame() {
	printf("lets play\n");
	started = true;
}


void loadGame() {
	textInput(true, &fileName);
	fileVal = 1;
}


void quitGame() {
	started = -1;
}

void saveGame() {
	World *w = getWorld();
	//arrayToFile("mapSave.txt", x->map);
}

void pooGame() {
		if (!paused) {
			AnimListAnimate();
			actorListDo();
			groundWater();
			if (!godMode && poopers[0] != 0) {
				//centerOnForm(poopers[0]->me->body);
				//setCenter(wv, w->x / 2, w->y / 2);
				followForms(wv);
				//lerpView(wv);
				//printf("currently at %f, %f\n", pooper->me->body->pos[0], pooper->me->body->pos[1]);
				//followForm(poopers[0]->me->body);
			} else {// if (god == 0) {
				//setCenter(wv, godPos[0], godPos[1]);
			}
			//printf("poopguy index: %i\n",((Anim*)poopers[0]->me->body->anim)->sprite);	
			//glClearColor(0, 0, 0, 1.0);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (debugDraw) {
				drawWorldDebug(w);
			} else {
				drawWorld(w);
			}
			if (gridOn) {
				drawGrid();
			}
			//glfwSwapBuffers(screen->window);
		
		} else {
			//glClearColor(0.1, 0.1, 0.1, 1.0);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawActiveMenu();
			updateMenu();
			renderTextInput();
			if (fileVal == 1) {
				if (strlen(fileName) > 0) {
					writeWorld(fileName);
					free(fileName);
					fileName = (char*)calloc(sizeof(char), 100);
				}
			}
		}

}

int defaultFrame = 0;

void gameLoop() {
	Screen *screen = getWindow();
	// why?? defaultFrame = screen->frame;
	cam = getDefaultView();
	wv = getDefaultView();
	setFrameMin(wv, 40);
	w = getWorld();
	processAudioFile("../resources/SchoolBoyLoop.wav");
	//setCameraSize(mainCam, 1);
	
	//makeStoneSquare((w->x/2), (w->y/2) - 40, 10);
	//poopers = (PoopGuy **)calloc(numPlayers, sizeof(PoopGuy*));	
	poopers = getPoopers();
	for (int i = 0; i < getNumPoopers(); i++) {
		followForm(poopers[i]->me->body);
	}
	//unFollowForm(poopers[1]->me->body);
	//set up offsets for rendering instances
	wvMakeBackground(wv, "resources/skyy.png");
	Button *demonButt = makeButton("resources/demonghost.png", 0, 2, 1, tmpButtFunc);
	Button *faceButt = makeButton("resources/faceghost.png", 0, 2, 1, exitMenu);
	Text *resume = makeText("RESUME", 1, true, 0.3, 0.6, 0.1, 0.8);
	addButtonText(faceButt, resume);
	addButtonSubColor(faceButt, 1, 1, 1, 1);
	Text *exit = makeText("EXIT", 1, true, 0.7, 0.4, 0.2, 0.8);
	addButtonText(demonButt, exit);
	addButtonSubColor(demonButt, 1, 0.8, 0.9, 1);
	placeUI(demonButt->graphics, -0.5, -0.1);
	placeUI(faceButt->graphics, 0.5, -0.1);
	pauseMenu = makeMenu(50);
	addButton(pauseMenu, demonButt);
	addButton(pauseMenu, faceButt);
	
	Button *saveButt = makeButton("resources/button.png", 0, 2, 1, saveMap);
	//setScale(saveButt->graphics->a, 0.7, 0.5);
	Text *save = makeText("SAVE", 1, true, 0.5, 0.5, 0.5, 0.5);
	addButtonText(saveButt, save);
	addButtonSubColor(saveButt, 1,1,1,1);
	placeUI(saveButt->graphics, 0, 0.5);
	addButton(pauseMenu, saveButt);
	
	makeCloud(10, 50, 20);
	
	Anim *dirt = makeAnimSheet("resources/dirtTileSheet.png", 1, 15, 5);
	GLuint sv = makeSpriteVao(1,1);
	animAddVao(dirt, sv);
	TileSet *dirtTiles = makeTileSet(dirt, wv->frameX, wv->frameY, w->x, w->y);

	Anim *stone = makeAnimSheet("resources/rockSheet.png", 1, 15, 1);
	GLuint ss = makeSpriteVao(1,1);
	animAddVao(stone, ss);
	TileSet *stoneTiles = makeTileSet(stone, wv->frameX, wv->frameY, w->x, w->y);

	//glfwUpdateGamepadMappings(gamecontrollerdb);
	god = makeGodPlayer(w->x * 0.5, w->y * 0.5, 20, 20);//wv->frameX, wv->frameY);
	Player *nullPlayer = makePlayer(0, -1, 0);
	addControl(nullPlayer, "K0G", toggleGod);
	addControl(nullPlayer, "K0!", togglePauseMenu);//S
	addControl(nullPlayer, "J07", togglePauseMenu);
	addControl(nullPlayer, "K0B", toggleDebugDraw);//B for Boxes
	godPos =  (float*)calloc(2, sizeof(float));
	godPos[0] = getWorld()->x /2;
	godPos[1] = getWorld()->y /2;
	FormLoop(pooGame);
	/*
	while(!glfwWindowShouldClose(screen->window)) {
		glfwPollEvents();
		checkControllerInput();
		processKeys();
		if(glfwGetKey(screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			//glfwSetWindowShouldClose(screen->window, 1);
		}
		if (!paused) {
			AnimListAnimate();
			actorListDo();
			groundWater();
			if (!godMode && poopers[0] != 0) {
				//centerOnForm(poopers[0]->me->body);
				//setCenter(wv, w->x / 2, w->y / 2);
				followForms(wv);
				//lerpView(wv);
				//printf("currently at %f, %f\n", pooper->me->body->pos[0], pooper->me->body->pos[1]);
				//followForm(poopers[0]->me->body);
			} else {// if (god == 0) {
				//setCenter(wv, godPos[0], godPos[1]);
			}
			//printf("poopguy index: %i\n",((Anim*)poopers[0]->me->body->anim)->sprite);	
			glClearColor(0, 0, 0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (debugDraw) {
				drawWorldDebug(w);
			} else {
				drawWorld(w);
			}
			if (gridOn) {
				drawGrid();
			}
			glfwSwapBuffers(screen->window);
		
		} else {
			glClearColor(0.1, 0.1, 0.1, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			drawActiveMenu();
			updateMenu();
			renderTextInput();
			if (fileVal == 1) {
				if (strlen(fileName) > 0) {
					writeWorld(fileName);
					free(fileName);
					fileName = (char*)calloc(sizeof(char), 100);
				}
			}
			glfwSwapBuffers(screen->window);
		}
	}
	*/
	free(godPos);
	exitGame();
}


void exitMenu() {
	paused = false;
	setMenuActive(pauseMenu, false);
	getMouseBack();
}

void saveMap() {
	/*
	int **map = worldToMap();
	printArray(map, w->x, w->y);
	arrayToFile("mapSave.txt", map);
	map = fileToArray("mapSave.txt");
	printArray(map, w->x, w->y);
	*/
	textInput(true, &fileName);
	fileVal = 1;

	//writeWorld("world.bin");
}

void togglePauseMenu(void *, float poo) {
	printf("poo\n");
	if (poo > 0) {
		paused = !paused;
		setMenuActive(pauseMenu, paused);
		if (!paused) {
			getMouseBack();
		}
	}
}

void toggleDebugDraw(void *, float poo) {
	if (poo > 0) {
		debugDraw = !debugDraw;
	}
}

void centerOnForm(Form* f) {
	WorldView *wv = getDefaultView();
	setCenter(wv, f->pos[0], f->pos[1]);
}

void toggleGod(void *, float poo) {
	if (poo > 0) {
		printf("toggle god %i\n", godMode);
		WorldView *wv = getDefaultView();
		if (godMode) {
		/*
			//godOff(god);
			if (poopers[0] != 0) {
				centerOnForm(poopers[0]->me->body);
			}
			sizeScreen(defaultFrame);
			*/
			godOff(god);
			godMode = false;
		} else {
			Screen *screen = getWindow();
			godMode= true;
			//setGod(god, getCenterX(), getCenterY(), wv->frameX, wv->frameY);
			godOn(god);
		}
		//setCameraSize(mainCam, mainCam->z + 0.1);
	}
}


