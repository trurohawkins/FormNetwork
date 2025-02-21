intList *makeIntList(int data) {
	intList *il = calloc(1, sizeof(intList));
	il->next = 0;
	il->data = data;
	return il;
}
void addToIntList(intList **head, int item) {
	if ((*head) == 0) {
		(*head) = makeIntList();
		(*head)->data = item;
	} else {
		linkedList *tmp = *head;
		while(tmp != 0) {
			if (tmp->next == 0) {
				tmp->next = makeIntList(item);
			}
			tmp = tmp->next;
		}
	}
}

int removeFromList(intList **head, int item) {
	if (*head == 0) {
		return 0;
	}
	int data = 0;
	if ((*head)->data == item) {
		linkedList *oh = *head;
		(*head) = (*head)->next;
		data = oh->data;
		free(oh);
	} else {
		linkedList *tmp = (*head)->next;
		linkedList *pre = *head;
		while (tmp != 0) {
			if (tmp->data == item) {
				pre->next = tmp->next;
				data = tmp->data;
				free(tmp);
				tmp = pre->next;
			} else {
				tmp = tmp->next;
				pre = pre->next;
			}
		}
	}
	return data;
}

void freeList(intList **head) {
	intList *cur = *head;
	intList *next = 0;
	while (cur != 0) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

