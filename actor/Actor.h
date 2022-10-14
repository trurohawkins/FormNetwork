typedef struct Actor {
	Form *body;
	linkedList *actionList;
} Actor;

Actor *makeActor(Form * f/*, Action *a*/);
void addAction(Actor *actor, Action *action);
void *removeAction(Actor *actor, Action *action);
void deleteMyAction(Actor *actor, Action *action);
void doActions(Actor *actor);
void deleteActor(Actor *actor);

#include "ActorList.h"
//#include "PoopGuy.h"
