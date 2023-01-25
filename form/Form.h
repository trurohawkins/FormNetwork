#ifndef LIB
#define LIB
#include "../helper/helper.h"
typedef struct Form {
	int id;
	float pos[2];
	float pMod[2];
	float color[3];
	int size[2];
	//int roto;
	//bool invert[2];
	float ***body;
	void *anim;
	void *stats;
	void *actor;
	// float stat; // Used to store moisture
	// float stat2; // Used to store hydralic conductivity
} Form;
//Form *inert;

Form *makeForm(float r, float g, float b, float wid, float len);
Form *checkSide(Form *f, int xd, int yd, bool collide);
int getEdge (Form *f, int side, int d);
void deleteForm(void *form);
void makeInert();
void printForm(void *f);
bool compareForms(Form *f1, Form *f2);
bool checkFormIsSolid(void *form);
bool isFormCenter(Form *f, int x, int y);
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
