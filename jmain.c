#include <stdlib.h>
#include <stdio.h>
#include "form/procGen.c"

bool doGL = true;

int main() {
    srand(time(NULL));
    int worldX = 50;
	int worldY = 50;
    int Seedstring[4] = {1,2,3,4};
    int** map = mapGen(Seedstring, worldX, worldY);
	printArray(map, worldX, worldY);
    printf("\n");
	return 0;
}
