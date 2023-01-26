Actor *makeActor(Form * f/*, Action *a*/) {
	Actor *newActor = (Actor*)calloc(1, sizeof(Actor));
	newActor->body = f;
	newActor->actionList = makeList();
	f->actor = newActor;
	/*
	if (a != 0) {
		addToList(newActor->actionList, a);
	}
	*/
	return newActor;
}


void addAction(Actor *actor, Action *action) {
	addToList(&(actor->actionList), action);
}

Action *findAction(char *name, Actor *actor) {
	linkedList *a = actor->actionList;
	int len = strlen(name);
	while (a != 0) {
		Action *act = a->data;
		if (act->name) {
			if (memcmp(act->name, name, len) == 0) {
				return act;
			}
		}
		a = a->next;
	}
}

void *removeAction(Actor *actor, Action *action) {
	return removeFromList(&(actor->actionList), action);
}

void deleteMyAction(Actor *actor, Action *action) {
	deleteAction(removeAction(actor, action));
}

void doActions(Actor *actor) {
	linkedList *a = actor->actionList;
	while (a != 0) {
		Action *act = (Action*) a->data;
		if (act->active == 1) {
			(*(act->fun))(actor->body, act);
		}
		a = a->next;
	}
}

void freeActionList(Actor *actor) {
	linkedList *a = actor->actionList;
	while (a != 0) {
		deleteAction((Action*)a->data);
		a = a->next;
	}
	freeList(&(actor->actionList));

}

void deleteActor(Actor *actor) {
	if (actor->body != NULL) {
		if (checkFormIsSolid(actor->body)) {
			//if form is not solid(it has a body larger than 1 Cell), then its easier if the actor takes care of it, t oprevent cyclical calls involving removing the different parts when deleting world
			removeForm(actor->body);
			deleteForm(actor->body);
		}
	}
	freeActionList(actor);
	//free(actor);
}
#include "ActorList.c"
//#include "PoopGuy.c"
