//#include "../formglfw/FormGlfw.h"
#include "../FormNetwork.h"
#include <stdbool.h>
#include "dirt.c"
#include "acts/actList.c"
#include "poopPlayer.c"
#include "Eco.c"
#include "procGen.c"
#include "Cloud.c"
#include "GameLogic.c"


int main(int argc, char **argv) {
	bool showMenu = false;
	if (argc > 1) {
		if (argv[1][1] == 'g') {
			setGrid(true);
		} else if (argv[1][1] == 'm') {
			showMenu = true;
		}
	}
/*
	int arr[3] = {1, 2, 3};
	writeBinaryInt("poo.bin", arr, 3);
	writeBinaryInt("poo.bin", arr, 3);
	//fput32le(420, "poo.bin");
	printf("int size %li\n", sizeof(int));
	int *poo = readBinaryInt("poo.bin", 6);
	if (poo != 0) {
		for (int i = 0; i < 6; i++) {
			printf("saved and recieved %i\n", poo[i]);
		}
	}
	\*/
	initFormGlfw();
	//initAudio();
	int worldY = 100;
	int worldX = 100; 
	initRecipes(3, 10);
	addRecipe(makePoopPlayer, savePoopPlayer, 0);
	addRecipe(makeDirt, saveDirt, 1);
	addRecipe(makeStone, saveForm, 2);
	makeActorList();
	initPoopers();
	int menuVal = 1;
	if (showMenu) {
		menuVal = mainMenu();
		if (menuVal < 0) {
			freeUILists();
			deleteTextureManager();
			freePlayerManager();
			freeJoyList();
			freeInput();
			glfwTerminate();
			return 0;
		}
	}
	//if (!loadWorld("world.bin")) {
	createWorld(worldX, worldY, 10, 10);
	if (menuVal != 2) {
		printf("generating world\n");
		//int **map = genMap();
		int **map = squareWorld();
		//genRain(map);
		genWorld(map);
		//fillWorld();
		//arrayToFile("mapSave.txt", map);
		freeMap(map);
		spawnPoopers(worldX * 0.5, worldY - 3);
		//printArray(map, worldX, worldY);
		//arrayToFile("mapSave.txt", map);
		//map = fileToArray("mapSave.txt");//
		//printArray(map, worldX, worldY);
	}
	//setCamFunction(calculateFrameData);
	//initWorldView(frameX, frameY);
	//initWorldDrawing();
	setCamFunction(resizeScreen);
	pooGame();
	return 0;
}
