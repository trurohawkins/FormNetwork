linkedList *ActorList;

void makeActorList() {
	ActorList = makeList();
}

void addActor(Actor *a) {
	addToList(&ActorList, a);
}

void removeActor(Actor *a) {
	removeFromList(&ActorList, a);
}

void deleteActorList() {
	linkedList *cur = ActorList;
	while (cur != NULL) {
		if (cur->data != NULL) {
			deleteActor((Actor*)(cur->data));
		}
		cur = cur->next;
	}
	freeList(&ActorList);
}
void actorListDo() {
	linkedList *cur = ActorList;
	while (cur != NULL) {
		if (cur->data != NULL) {
			doActions((Actor*)cur->data);
		}
		cur = cur->next;
	}
}
