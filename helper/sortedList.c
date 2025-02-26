SortedList *makeSlist(void *data, int val) {
	SortedList *slist = calloc(1, sizeof(SortedList));
	slist->data = data;
	slist->val = val;
	return slist;
}

void sortedAdd(SortedList **head, void *item, int val) {
	if ((*head) == 0) {
		(*head) = makeSlist(item, val);
	} else {
		SortedList *cur = *head;
		while(cur) {
			if (cur->val > val) {
				//printf("%i > %i\n", cur->val, val);
				//insert here and return
				SortedList *slist = makeSlist(item, val);
				if (cur->pre) {
					//printf("in betwenn\n");
					cur->pre->next = slist;
					slist->pre = cur->pre;
				} else {
					//printf("new head\n");
					(*head) = slist;
				}
				cur->pre = slist;
				slist->next = cur;
				return;
			} else {
				if (cur->next) {
					cur = cur->next;
				} else {
					cur->next = makeSlist(item, val);
					cur->next->pre = cur;
					return;
				}
			}
		}
	}
}

void *searchSlist(SortedList *cur, int val) {
	while (cur) {
		if (cur->val == val) {
			return cur->data;
		}
		cur = cur->next;
	}
	return 0;
}

void freeSlist(SortedList *head) {
	SortedList *cur = head;
	while(cur) {
		SortedList *tmp = cur;
		cur = cur->next;
		free(tmp);
	}
}

void printSlist(SortedList *head) {
	printf("printing sorted list\n");
	while (head) {
		printf("Node(%i) - %p\n", head->val, head->data);
		head = head->next;
	}
}
