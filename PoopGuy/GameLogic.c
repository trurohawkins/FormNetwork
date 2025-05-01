#include "GameLogic.h"

WorldView *cam;
WorldView *wv;
World *w;

PoopGuy **poopers;
int numPlayers = 1;
bool gridOn = false;
bool paused = false;
bool debugDraw = false;
float *godPos;
Menu *pauseMenu;
GOD *god = 0;

#include "pooMenu.c"


void setGrid(bool state) {
	gridOn = state;
}
int defaultFrame = 0;

void pooGame() {
	Screen *screen = getWindow();
	// why?? defaultFrame = screen->frame;
	cam = getDefaultView();
	wv = getDefaultView();
	setFrameMin(wv, 30);
	w = getWorld();
	//processAudioFile("../resources/SchoolBoyLoop.wav");
	//setCameraSize(mainCam, 1);
	
	//makeStoneSquare((w->x/2), (w->y/2) - 40, 10);
	//poopers = (PoopGuy **)calloc(numPlayers, sizeof(PoopGuy*));	
	god = makeGodPlayer(w->x * 0.5, w->y * 0.5, w->x, w->y);//wv->frameX, wv->frameY);
	addControl(god->p, "K0G", toggleGod);
	addControl(god->p, "K0!", togglePauseMenu);//S
	addControl(god->p, "J07", togglePauseMenu);
	addControl(god->p, "K0B", toggleDebugDraw);//B for Boxes
	godPos =  (float*)calloc(2, sizeof(float));
	godPos[0] = getWorld()->x /2;
	godPos[1] = getWorld()->y /2;
	if (numPoopers > 0) {
		poopers = getPoopers();
		for (int i = 0; i < getNumPoopers(); i++) {
			followForm(poopers[i]->me->body);
		}
	} else {
		godOn(god);
	}
	//unFollowForm(poopers[1]->me->body);
	//set up offsets for rendering instances
	wvMakeBackground(wv, "resources/skyy.png");
	setBackgroundColor(0.4, 0.6, 0.8, 1);
	//setUpPauseMenu();
	makeCloud(10, 50, 20);
	
	
	Anim *dirt = makeAnimSheet("resources/dirtTileSheet.png", 1, 15, 5);
	GLuint sv = makeSpriteVao(1,1);
	animAddVao(dirt, sv);
	TileSet *dirtTiles = makeTileSet(dirt, wv->frameX, wv->frameY, w->x, w->y, wv->objSX, wv->objSY);
	dirtTiles->tileSprites = &tileCell;

	Anim *stone = makeAnimSheet("resources/rockSheet.png", 1, 15, 1);
	GLuint ss = makeSpriteVao(1,1);
	animAddVao(stone, ss);
	TileSet *stoneTiles = makeTileSet(stone, wv->frameX, wv->frameY, w->x, w->y, wv->objSX, wv->objSY);
	
	//newTileSet("resources/dirtTileSheet.png", 15, 5, 1);
	//newTileSet("resources/rockSheet.png", 15, 1, -1);

	//glfwUpdateGamepadMappings(gamecontrollerdb);
	FormLoop(pooLoop);
	free(godPos);
	exitGame();
}


void pooLoop() {
		if (!paused) {
			AnimListAnimate();
			actorListDo();
			groundWater();
			if (!god->on && poopers[0] != 0) {
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

void togglePauseMenu(void *, float poo) {
	stopLoop();
	return;
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
		WorldView *wv = getDefaultView();
		if (god->on) {
		/*
			//godOff(god);
			if (poopers[0] != 0) {
				centerOnForm(poopers[0]->me->body);
			}
			sizeScreen(defaultFrame);
			*/
			godOff(god);
		} else {
			Screen *screen = getWindow();
			//setGod(god, getCenterX(), getCenterY(), wv->frameX, wv->frameY);
			godOn(god);
		}
		//setCameraSize(mainCam, mainCam->z + 0.1);
	}
}


