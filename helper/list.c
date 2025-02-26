linkedList *makeList() {
	linkedList *nl = (linkedList*) calloc(1, sizeof(linkedList));
	nl->data = 0;
	nl->next = 0;
//	nl->prev = 0;
	return nl;
}

void addToList(linkedList **head, void *item) {
	if ((*head) == 0) {
		(*head) = makeList();
		(*head)->data = item;
	} else {
		linkedList *tmp = *head;
		while(tmp->data != 0) {
			if (tmp->next == 0) {
				tmp->next = makeList();
			}
			tmp = tmp->next;
		}
		tmp->data = item;
	}
}

//checks for doubles
bool addToListSingle(linkedList **head, void *item) {
	if ((*head) == 0) {
		(*head) = makeList();
		(*head)->data = item;
	} else {
		linkedList *tmp = *head;
		while(tmp->data != 0) {
			if (tmp->data == item) {
				return false;
			}
			if (tmp->next == 0) {
				tmp->next = makeList();
			}
			tmp = tmp->next;
		}
		tmp->data = item;
	}
	return true;
}



void *indexList(linkedList **head, int index) {
	linkedList *cur = *head;
	int originalIndex = index;
	while(cur != NULL) {
		if (cur->data != NULL) {
			if (index == 0) {
				return cur->data;
			} else {
				index--;
				cur = cur->next;
			}
		}
	}
	if (index > 0) {
		printf("index %i out of range for list\n", originalIndex);
	}
	return NULL;
}

void* findList(linkedList **head, void *item, bool (*cmp)(void*, void*)) {
	linkedList *cur = *head;
	while (cur != NULL) {
		if (cur->data != NULL) {
			if (cmp(cur->data, item) == true) {
				return cur->data;
			}
		}
		cur = cur->next;
	}
	return false;
}

bool cmpList(linkedList **head, void *item, bool (*cmp)(void*, void*)) {
	linkedList *cur = *head;
	while (cur != NULL) {
		if (cur->data != NULL) {
			if (cmp(cur->data,item) == true) {
				return true;
			}
		}
		cur = cur->next;
	}
	return false;
}

void *checkList(linkedList **head, bool (*chk)(void*)) {
	linkedList *cur = *head;
	while (cur != NULL) {
		if (cur->data != NULL) {
			if (chk(cur->data) == true) {
				return cur->data;
			}
		}
		cur = cur->next;
	}
	return false;
}

bool isInList(linkedList **head, void *item) {
	linkedList * cur = *head;
	while(cur != NULL) {
		if (cur->data != NULL) {
			if (cur->data == item) {
				return true;
			}
		}
		cur = cur->next;
	}
	return false;
}

bool isInListInt(linkedList **head, int item) {
	linkedList * cur = *head;
	while(cur != NULL) {
		if (cur->data != NULL) {
			int data = *((int*)cur->data);
			if (data == item) {
				return true;
			}
		}
		cur = cur->next;
	}
	return false;
}


void printList(linkedList **head, char *listName, void (*print)(void*)) {
	linkedList *cur = *head;
	printf("printing list %s\n", listName);
	int num = 0;
	while (cur != NULL) {
		printf("[%i] ", num);
		if (cur->data != NULL) {
			print(cur->data);
		}
		cur = cur->next;
		num++;
	}
	printf("printing done\n");
}

void listPrint(linkedList *head) {
	linkedList *cur = head;
	printf("printing list %p\n", head);
	int num = 0;
	while (cur != NULL) {
		printf("[%i] %p", num, cur->data);
		cur = cur->next;
		num++;
	}
	printf("\n");

}

void *removeFromList(linkedList **head, void *item) {
	if (*head == 0) {
		return 0;
	}
	void *data = 0;
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
				//break;
			} else {
				tmp = tmp->next;
				pre = pre->next;
			}
			//pre = pre->next;
		}
	}
	return data;
}

void *removeFromListCheck(linkedList **head, bool (*chk)(void*)) {
	void *data = 0;
	if ((*head) != NULL) {
		if ((*head)->data != NULL) {
			if (chk((*head)->data)) {
				//printf("removed head\n");
				linkedList *oh = *head;
				(*head) = (*head)->next;
				data = oh->data;
				free(oh);
			} else {
				linkedList *tmp = (*head)->next;
				linkedList *pre = *head;
				while (tmp != 0) {
					if (chk(tmp->data)) {
						pre->next = tmp->next;
						data = tmp->data;
						free(tmp);
						return data;
						tmp = pre->next;
					} else {
						tmp = tmp->next;
					}
					pre = pre->next;
				}
			}
		}
	}
	return data;
}

bool checkAndDelete(linkedList **head, bool (*chk)(void*), void (*del)(void*)) {
	if ((*head) != NULL) {
		//printf("checking and deleting for %p\n", *head);
		if ((*head)->data != NULL) {
			if (chk((*head)->data)) {
				linkedList *oh = *head;
				//printf("removed head %p\n", oh->data);
				(*head) = (*head)->next;
				del(oh->data);
				free(oh);
			} else {
				linkedList *tmp = (*head)->next;
				linkedList *pre = *head;
				while (tmp != 0) {
					//printf("checking %p\n", tmp->data);
					if (chk(tmp->data)) {
						pre->next = tmp->next;
						del(tmp->data);
						free(tmp);
						tmp = pre->next;
					} else {
						tmp = tmp->next;
						pre = pre->next;
					}
				}
			}
		}
	}
}

void checkAndRemove(linkedList **head, bool (*chk)(void*)) {
	if ((*head) != NULL) {
			if (chk((*head)->data)) {
				linkedList *oh = *head;
				(*head) = (*head)->next;
				free(oh->data);
				free(oh);
			} else {
				linkedList *tmp = (*head)->next;
				linkedList *pre = *head;
				while (tmp != 0) {
					if (chk(tmp->data)) {
						pre->next = tmp->next;
						free(tmp->data);
						free(tmp);
						tmp = pre->next;
					} else {
						tmp = tmp->next;
						pre = pre->next;
					}
				}
			}
	}
}


void **getContents(linkedList **head, int num) {
	if (num > 0) {
		void **contents = (void**)calloc(sizeof(void*), num);
		linkedList *cur = *head;
		int i = 0;
		while (cur != NULL) {
			if (i < num) {
				if (cur->data != NULL) {
					contents[i] = cur->data;	
					i++;
					cur = cur->next;
				}
			}
		}
		if (i > 0) {
			return contents;
		} else {
			free(contents);
			return NULL;
		}
	} else {
		return NULL;
	}
}

int countContents(linkedList *cur) {
	int count = 0;
	while (cur != 0) {
		if (cur->data != 0) {
			count++;
		}
		cur = cur->next;
	}
	return count;
}


void freeList(linkedList **ll) {
	linkedList *cur = *ll;
	linkedList *next = 0;
	while (cur != NULL) {
		next = cur->next;
		if (cur->data != NULL) {
			free(cur->data);
		}
		free(cur);
		cur = next;
	}	
}

void freeListSaveObj(linkedList **ll) {
	linkedList *cur = *ll;
	linkedList *next = 0;
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}
	//*ll = 0;
}

void deleteList(linkedList **ll, void (*deleteFunc)(void*)) {
	linkedList *cur = *ll;
	linkedList *next = 0;
	while (cur != NULL) {
		next = cur->next;
		if (cur->data) {
			deleteFunc(cur->data);
		}
		free(cur);
		cur = next;
	}	
}

void *removeFromListInt(linkedList **head, int item) {
	void *data = 0;
	if (*head == 0 || (*head)->data == 0) {
		return data;
	}
	if (*(int*)((*head)->data) == item) {
		linkedList *oh = *head;
		(*head) = (*head)->next;
		data = oh->data;
		free(oh);
	} else {
		linkedList *tmp = (*head)->next;
		linkedList *pre = *head;
		while (tmp != 0) {
			if (*((int*)(tmp->data)) == item) {
				pre->next = tmp->next;
				data = tmp->data;
				free(tmp);
				tmp = pre->next;
			} else {
				tmp = tmp->next;
			}
			pre = pre->next;
		}
	}
	return data;
}

void *popList(linkedList **head) {
	void *data = (*head)->data;
	linkedList *oh = *head;
	(*head) = (*head)->next;
	free(oh);
	return data;
}
