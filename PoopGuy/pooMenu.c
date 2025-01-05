#include "pooMenu.h"

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
	
	setMenuActive(startMenu, true);
	FormLoop(menuLogic);
	fileVal = 0;
	setMenuActive(startMenu, false);
	return started;
}

void setUpPauseMenu() {
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


char *getFileName() {
	return fileName;
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
