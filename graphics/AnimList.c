linkedList *AnimList;

void makeAnimList() {
	AnimList = makeList();
}
void addAnim(Anim *a) {
	printf("adding to anim list\n");
	addToList(&AnimList, a);
}

void removeAnim(Anim *a) {
	removeFromList(&AnimList, a);
}

void deleteAnimList() {
	linkedList *cur = AnimList;
	while (cur != NULL) {
		linkedList *next = cur->next;
		if (cur->data != NULL) {
			freeAnim((Anim*)(cur->data));
		}
		free(cur);
		cur = next;
	}
}

void AnimListAnimate() {
	linkedList *cur = AnimList;
	while (cur != NULL) {
		if (cur->data != NULL) {	
			animate((Anim*)cur->data);
		}
		cur = cur->next;
	}
}
