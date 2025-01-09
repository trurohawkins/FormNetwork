#ifndef LIB
#define LIB
#include "../helper/helper.h"
typedef struct Form {
	int id;
	float pos[2];
	float pMod[2];
	float color[3];
	int size[2];
	
	int **sides;
	//int roto;
	//bool invert[2];
	int *body;
	int bLen; //body length
	void *anim;
	int aCount;
	void *stats;
	void *actor;
	//neg terminated
	int *colMatrix;
	bool solid;
	void (*render)(struct Form*, int xp, int yp, int buffX, int buffY);
	bool terrain;
	// float stat; // Used to store moisture
	// float stat2; // Used to store hydralic conductivity
} Form;
//Form *inert;

typedef struct Collider {
	int size[2];
	int **sides;
	int *body;
	int bLen;
} Collider;

Form *makeForm(float r, float g, float b, float wid, float len);
Form *makeIrregularForm(float r, float g, float b, int *body, int bLen);
int *squareBody(int wid, int len);
int **squareSides(int wid, int len);
Collider *squareCollider (int wid, int len);
int **calcSides(int *body, int len);
void addToBody(Form *f, int *newGrowth, int amount);
void freeCollider(Collider *c);
void changeCollider(Form *f, Collider *c);
Collider *getCollider(Form *f);
//Form *checkSide(Form *f, int xd, int yd, bool collide);
int getEdge (Form *f, int side, int d);
void deleteForm(void *form);
void makeInert();
void printForm(void *f);
bool compareForms(Form *f1, Form *f2);
//bool checkFormIsSolid(void *form);
bool isFormCenter(Form *f, int x, int y);
//if num != 0, will calloc a new array and copy in
int* addNoCol(Form *a, int *id, int num);
bool canCollide(Form *a, Form *b);
/*
void setRoto(Form *f, int degree);
void setInvert(Form *f, int axis, bool flipped);
*/
#include "../actor/Action.h"
#include "../actor/Actor.h"
#include "Cell.h"
#include "World.h"
#include "Value.h"
#include "FormSpawner.h"
#endif
