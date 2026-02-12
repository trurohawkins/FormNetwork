#ifndef ACTOR
#define ACTOR
typedef struct Actor {
	Form *body;
	linkedList *actionList;
	bool deleteMe;
	bool active;

	void (*destroy)(struct Actor*);
} Actor;

Actor *makeActor(Form * f/*, Action *a*/);
void addAction(Actor *actor, Action *action);
Action *findAction(Actor *actor, char *name);
void *findActVars(Actor *actor, char *name);
void *removeAction(Actor *actor, Action *action);
void deleteMyAction(Actor *actor, Action *action);
void doActions(Actor *actor);
void deleteActor(void *actor);
// destroy called by other functions to let us know wee need to destroy this Actor and or Form, and run nay other code
void destroyActor(Actor *a);

//#include "ActorList.h"
//#include "PoopGuy.h"
#endif
