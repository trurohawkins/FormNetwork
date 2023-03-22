//#include "../formglfw/FormGlfw.h"
#include "../FormNetwork.h"
#include <stdbool.h>
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
	initAudio();
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
	createWorld(worldX, worldY, 4, 4);
	if (menuVal != 2) {
		int Seedstring[4] = {1,2,3,4};
		printf("generating world\n");
		int **map = genMap(Seedstring);
		//genRain(map);
		genWorld(map);
		//fillWorld();
		//arrayToFile("mapSave.txt", map);
		freeMap(map);
		int xPos = (worldX * 0.5);
		int yPos = worldY / 2;
		for (int i = 0; i < getNumPoopers(); i++) {
			Form *pp = makePoopPlayer(i);
			int xp = xPos + i * 4;
			linkedList *res = checkPos(pp, xp, yPos, true);
			linkedList *r = res;
			while (res) {
				if (res->data) {
					removeForm(res->data);
					deleteForm(res->data);
				}
				res = res->next;
			}
			freeListSaveObj(&r);
			placeForm(xp,  yPos, pp);//makePoopPlayer(i));
			//poopers[i] = makePoopPlayer(xPos + (i*4), 1, i);
		}
		//printArray(map, worldX, worldY);
		//arrayToFile("mapSave.txt", map);
		//map = fileToArray("mapSave.txt");//
		//printArray(map, worldX, worldY);
	}
	//setCamFunction(calculateFrameData);
	//initWorldView(frameX, frameY);
	//initWorldDrawing();
	setCamFunction(resizeScreen);
	gameLoop();
	return 0;
}
