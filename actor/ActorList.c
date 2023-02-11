linkedList *ActorList = 0;

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

bool checkDeleteActor(void *actor) {
	return ((Actor *)actor)->deleteMe;
}

void actorListDo() {
	linkedList *cur = ActorList;
	linkedList *pre = cur;
	int count = 0;
	/*
	if (ActorList) { 
		if (ActorList->data) {
			Actor *a = ActorList->data;
			if(a->deleteMe) {
				linkedList *oh = ActorList;
				ActorList = ActorList->next;
				freeActor(a);
				free(oh);
			}
		}
	}
	*/
	while (cur != NULL) {
		if (cur->data != NULL) {
			Actor *a = cur->data;
			doActions(a);
			if (a->deleteMe) {
				linkedList *tmp = cur;
				deleteActor(a);
				cur->data = 0;
				if (pre == cur) {
					printf("head deleting\n");
					ActorList = cur->next;
				} else {
					pre->next = cur->next;
				}
				cur = cur->next;
				free(tmp);
				if (cur == 0 && count == 0) {
					printf("deleting actorlist\n");
					free(ActorList);
					ActorList = 0;//makeList();
					break;
				}
				count++;
				continue;
			}
			count++;
		}
		if (pre != cur) {
			pre = cur;
		}
		cur = cur->next;
	}
	/*
	Actor *a = 0;
	do {
		a = removeFromListCheck(&ActorList, checkDeleteActor);
		if (a) {
			deleteActor(a);
		}
	} while (a);
	*/
}

