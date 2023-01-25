#ifndef PROC
#define PROC

//#include "../helper/helper.h"
void arrayToFile(char*, int**);
int **fileToArray(char*);
int **genMap(int*);
int **genRain(int**);
void fillWorld();
void genWorld(int**);
int **worldToMap();
void freeMap(int **);
#endif
