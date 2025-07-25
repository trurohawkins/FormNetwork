#ifndef HELP
#define HELP
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include<stdbool.h>
#include<string.h>
#include <math.h>
#include <limits.h>
//#include "file.h"

void initDirections();
void freeDirections();
int **getDirs();
int **getDirs8();
float getAngle(int dir);
int *getDir8(int dir);
int *getDir4(int dir);
int d8Tod4(int);
int xyToDir4(int *xy);
int xyToDir8(int x, int y);
int convertInvert(bool inv);
float randPercent();
int randomInt(int upper);
int randRange(int mmin, int max);
int coinFlip(int a, int b);
int clamp(int, int, int);
double clampF(double, double, double);
float max(float a, float b);
float min(float a, float b);
int sign(int);
int signF(float num);
bool equal(float a, float b);
float lerp(float a, float b, float t);
//int abs(int);
float distance(float x1, float y1, float x2, float y2);
char *fileToString(char *txt);
bool strCompare(char *str1, char *str2);
void printArray(int**, int, int);
bool compareColor(void *c1, void *c2);
int decPlace(int d);
float intToFrac(int i, int pow);

#include "list.h"
#include "intList.h"
#include "sortedList.h"
#include "binaryWriter.h"
#endif
