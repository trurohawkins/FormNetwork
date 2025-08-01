intList *makeIntList(int data) {
	intList *il = calloc(1, sizeof(intList));
	il->next = 0;
	il->data = data;
	return il;
}
void addToIntList(intList **head, int item) {
	if ((*head) == 0) {
		(*head) = makeIntList(item);
		//(*head)->data = item;
	} else {
		intList *tmp = *head;
		while(tmp->next != 0) {
			tmp = tmp->next;
		}
		tmp->next = makeIntList(item);
	}
}

bool addToIntListSingle(intList **head, int item) {
	if ((*head) == 0) {
		(*head) = makeIntList(item);
		//(*head)->data = item;
	} else {
		intList *chk = *head;
		while (chk) {
			if (chk->data == item) {
				return false;
			} else {
				if (chk->next == 0) {
					break;
				} else {
					chk = chk->next;
				}
			}
		}
		chk->next = makeIntList(item);
	}
	return true;
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

void checkAndRemoveInt(intList **head, bool (*chk)(int)) {
	if ((*head) != NULL) {
			if (chk((*head)->data)) {
				intList *oh = *head;
				(*head) = (*head)->next;
				free(oh);
			} else {
				intList *tmp = (*head)->next;
				intList *pre = *head;
				while (tmp != 0) {
					if (chk(tmp->data)) {
						pre->next = tmp->next;
						free(tmp);
						tmp = pre->next;
					} else {
						tmp = tmp->next;
					}
					pre = pre->next;
				}
			}
	}
}

int countList(intList *head) {
	int count = 0;
	intList *tmp = head;
	while (tmp) {
		count++;
		tmp = tmp->next;
	}
	return count;
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

