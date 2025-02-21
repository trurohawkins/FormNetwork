#include "helper.h"

//#include "file.c"
int **dirs;
int **dir8;
float *angles;

void initDirections() {
	printf("initilaizing dirs");
	dirs = (int**)calloc(4, sizeof(int*));
	for (int i = 0; i < 4; i++) {
		dirs[i] = (int*)calloc(2, sizeof(int));
	}
	dir8 = (int**)calloc(8, sizeof(int*));
	for (int i = 0; i < 8; i++) {
		dir8[i] = (int*)calloc(2, sizeof(int));
	}
	angles = (float*)calloc(8, sizeof(float));
	dirs[0][0] = 0;
	dirs[0][1] = 1;
	dirs[1][0] = -1;
	dirs[1][1] = 0;
	dirs[2][0] = 0;
	dirs[2][1] = -1;
	dirs[3][0] = 1;
	dirs[3][1] = 0;

	dir8[0][0] = 0;
	dir8[0][1] = 1;
	dir8[1][0] = -1;
	dir8[1][1] = 1;
	dir8[2][0] = -1;
	dir8[2][1] = 0;
	dir8[3][0] = -1;
	dir8[3][1] = -1;
	dir8[4][0] = 0;
	dir8[4][1] = -1;
	dir8[5][0] = 1;
	dir8[5][1] = -1;
	dir8[6][0] = 1;
	dir8[6][1] = 0;
	dir8[7][0] = 1;
	dir8[7][1] = 1;

	angles[0] = 0;
	angles[1] = 45;
	angles[2] = 90;
	angles[3] = 135;
	angles[4] = 180;
	angles[5] = 225;
	angles[6] = 270;
	angles[7] = 315;
}

void freeDirections() {
	for (int i = 0; i < 4; i++) {
		free(dirs[i]);
	}
	free(dirs);
	for (int i = 0; i < 8; i++) {
		free(dir8[i]);
	}
	free(dir8);
	free(angles);
}

int convertInvert(bool inv) {
	if (inv) {
		return -1;
	} else {
		return 1;
	}
}

int **getDirs() {
	return dirs;
}

int *getDir4(int dir) {
	if (dir > -1 && dir < 4) {
		return dirs[dir];
	}
	return dirs[0];
}

int *getDir8(int dir) {
		if (dir > -1 && dir < 8) {
			return dir8[dir];
		}
		return dir8[0];
}

int d8Tod4(int dir) {
	if (dir % 2 == 1) {
		if (dir == 1 || dir == 3) {
			dir = 2;
		} else if (dir == 5 || dir == 7) {
			dir = 6;
		}
	}
	dir /= 2;
	return dir;
}

int xyToDir4(int *xy) {
	for (int i = 0; i < 4; i++) {
		if (xy[0] == dirs[i][0] && xy[1] == dirs[i][1]) {
			return i;
		}
	}
	printf("(%i, %i), did not mmatch a cardinal direction\n", xy[0], xy[1]);
}


int **getDirs8() {
	return dir8;
}

float getAngle(int dir) {
	if (dir > -1 && dir < 8) {
		return angles[dir];
	} else {
		return -1;
	}
}

float randPercent() {
	return (float)rand()/(float)(RAND_MAX);
}

int randomInt(int upper) {
	return rand() % upper;
}

int randRange(int min, int max) {
	int amnt = max - min + 1;
	if (amnt == 0) {
		amnt = 1;
	}
	return (rand() % amnt) + min;
}

float distance(float x1, float y1, float x2, float y2) {
	float d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	return d;
}

int clamp(int val, int min, int max) {
	if (min < max) {
		if (val < min) {
			return min;
		}	else if (val > max) {
			return max;
		} else {
			return val;
		}
	} else {
		return val;
	}
}

double clampF(double val, double min, double max) {
  const double t = val < min ? min : val;
  return t > max ? max : t;
}

float max(float a, float b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

float min(float a, float b) {
	if (a < b) {
		return a;
	} else {
		return b;
	}
}

int sign(int num) {
	if (num > 0) {
		return 1;
	} else if (num < 0) {
		return -1;
	} else {
		return 0;
	}
}

int signF(float num) {
	if (num > 0) {
		return 1;
	} else if (num < 0) {
		return -1;
	} else {
		return 0;
	}
}

int abs(int num) {
	if (num < 0) {
		return num * -1;
	} else {
		return num;
	}
}

char *fileToString(char *txt)
{
	FILE *fptr;
	int n = 0;
	int c;

	fptr = fopen(txt, "r");
	char *t = NULL;
	if (fptr != NULL) {
		fseek(fptr, 0, SEEK_END);
		long fSize = ftell(fptr);
		printf("file size: %i\n", fSize);
		fseek(fptr, 0, SEEK_SET);
		t = (char*)malloc(fSize * sizeof(char));
		while ((c = fgetc(fptr)) != EOF) {
			t[n++] = (char)c;
		}
		printf("wrote into: %i\n", n);
		t[fSize-1] = '\0';
		fseek(fptr, 0, SEEK_SET);
		fclose(fptr);
	}
	return t;
}

bool strCompare(char *str1, char *str2) {
	while (*str1 != '\0') {
		if (*str1 != *str2) {
			return false;
		}
		str1++;
		str2++;
	}
	return true;
}

void printArray(int **array, int sizeX, int sizeY) {
	printf("\n");

   for( int  y=sizeY-1;y>-1;y--) {
      for(int x=0; x<sizeX; x++) {
         printf("%d ", array[x][y]);
		 if( array[x][y] == 0){
			 printf(" ");
		 }
         if(x==sizeX-1){
            printf("\n");
         }
      }
   }
}

bool compareColor(void *c1, void *c2) {
	unsigned char *col1 = (unsigned char*)c1;
	unsigned char *col2 = (unsigned char*)c2;
	if (col1[0] == col2[0]) {
		if (col1[1] == col2[1]) {
			if (col1[2] == col2[2]) {
				return true;
			}
		}
	}
	return false;
}

int decPlace(int d) {
	int num = 0;
	while (d >= 10) {
		d/=10;
		num++;
	}
	return num;
}

float intToFrac(int i, int pow) {
	int num = (i / pow) * pow;//pow(10, decPlace(i));
	//printf("%i\n", num);
	int res = i - num;//((i / 10) * num);
	//printf("pulled %i from %i\n", res, i);
	return (float)res / pow;
}

#include "list.c"
#include "intList.h"
#include "sortedList.c"
#include "binaryWriter.c"

