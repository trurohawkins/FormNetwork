intList *makeIntList(int data) {
	intList *il = calloc(1, sizeof(intList));
	il->next = 0;
	il->data = data;
	return il;
}
void addToIntList(intList **head, int item) {
	if ((*head) == 0) {
		(*head) = makeIntList(item);
		(*head)->data = item;
	} else {
		intList *tmp = *head;
		while(tmp != 0) {
			if (tmp->next == 0) {
				tmp->next = makeIntList(item);
			}
			tmp = tmp->next;
		}
	}
}

int removeFromIntList(intList **head, int item) {
	if (*head == 0) {
		return 0;
	}
	int data = 0;
	if ((*head)->data == item) {
		intList *oh = *head;
		(*head) = (*head)->next;
		data = oh->data;
		free(oh);
	} else {
		intList *tmp = (*head)->next;
		intList *pre = *head;
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

void freeIntList(intList **head) {
	intList *cur = *head;
	intList *next = 0;
	while (cur != 0) {
		next = cur->next;
		free(cur);
		cur = next;
	}
}

