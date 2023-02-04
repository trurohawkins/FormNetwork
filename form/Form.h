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
	int ***body;
	void *anim;
	void *stats;
	void *actor;
	//null terminated
	int *colMatrix;
	// float stat; // Used to store moisture
	// float stat2; // Used to store hydralic conductivity
} Form;
//Form *inert;

typedef struct Collider {
	int size[2];
	int **sides;
	int ***body;
} Collider;

Form *makeForm(float r, float g, float b, float wid, float len);
int ***makeBody(int wid, int len);
int **calcSides(int wid, int len);
Collider *makeCollider (int wid, int len);

void changeCollider(Form *f, Collider *c);
Collider *getCollider(Form *f);
//Form *checkSide(Form *f, int xd, int yd, bool collide);
int getEdge (Form *f, int side, int d);
void deleteForm(void *form);
void makeInert();
void printForm(void *f);
bool compareForms(Form *f1, Form *f2);
bool checkFormIsSolid(void *form);
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
