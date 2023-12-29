#include "Cell.h"

Cell *makeCell(int x, int y) {
	Cell *c = (Cell*)calloc(sizeof(Cell), 1);
	c->pos[0] = x;
	c->pos[1] = y;
	c->within = 0;//makeList();
	c->count = 0;
}

void addToCell(Cell* c, Form *f) {
	if (f != NULL) {
		/*
		if (c->within == 0) {
			c->within = makeList();
		}
		*/
		addToList(&(c->within), f);
		c->count++;
		/*
		if (c->count > 1) {
			printf("why so many in here??\n");
			printCell(c);
		}
		*/
		//if (f->size[0] != 0 && f->size[1] != 0) {
		//if (checkFormIsSolid(f)) {
		if (f->solid) {
			c->solid++;
		}
	} else {
		printf("tried to add NULL form to Cell ");
		printCell(c);
	}
}

Form *removeFromCell(Cell *c, Form *f) {
	if (!c->within) {
		return 0;
	}
	void *fv = (void *)f;
	if (fv) {
		void *v = removeFromList(&(c->within), fv);
		if (v != NULL) {
			c->count--;
			//if (checkFormIsSolid(f)) {
			if (f->solid) {
				c->solid--;
			}
		}
	}
	return fv;
}

bool checkSolid(void *form) {
	Form *f = form;
	return f->solid;
}

linkedList *getSolidForm(Cell* c) {
	//linkedList *solids = 0;
	clearCheck();
	if (c->within != 0) {
		Form *f = 0;
		//printCell(c);
		do {
			f = removeFromListCheck(&(c->within), checkSolid);	
			if (f != NULL) {
				addToList(&check, f);
				//printCell(c);
				c->count--;
				c->solid--;
			}
			//printf("new counnt %i\n", c->solid);
		} while (c->solid > 0);
	}
	return check;
}

linkedList *checkSolidForm(Cell* c) {
	linkedList *cur = c->within;
	//printf("checking solid forms\n");
	clearCheck();
	//linkedList *solids = 0;
	while (cur) {
		Form *f = cur->data;
		if (f) {
			//if (checkFormIsSolid(cur->data)) {
			if (f->solid) {
				addToList(&check, f);
			}
		}
		cur = cur->next;
	}
	/*
	Form *f = 0;
	//if (c->solid) {
	if (c->within != 0) {
		f = (Form*)checkList(&(c->within), checkFormIsSolid);	
	}
	return f;
	*/
	//printf("returning check\n");
	return check;
}

bool isSolidForm(Cell *c) {
	linkedList *cur = c->within;
	while (cur) {
		Form *f = cur->data;
		if (f && f->solid) {
			return true;
		}
		cur = cur->next;
	}
	return false;
}

Form **getCellContents(Cell *c) {
	if (c->count == 0 && c->within == NULL) {
		return NULL;
	}
	Form **content = (Form**)calloc(sizeof(Form*), c->count);
	linkedList *cur = c->within;
	int count = 0;
	while (cur != NULL) {
		if (cur->data != NULL) {
			content[count] = (Form*)cur->data;
			count++;
		}
		cur = cur->next;
	}
	return content;
}

bool writeCell(char *file, Cell *c) {
	linkedList *cur = c->within;
	int count = 0;
	int *arr = (int*)calloc(c->count, sizeof(int));
	while (cur != NULL) {
		if (cur->data != NULL) {
			Form *f = (Form*)cur->data;
			if (isFormCenter(f, c->pos[0], c->pos[1])) {
				/*
				if (f->id == 69) {
					int cx = (int)floor(f->pos[0]);
					int cy = (int)floor(f->pos[1]);
					printf("%i, %i got poopguy at %i, %i\n", data[1], data[2], cx, cy);
				}
				*/
				FormRecipe *r = getRecipe(f->id);
				if (r) {
					if (f->id == 0) {
						printf("saving poop guy at %i, %i\n", c->pos[0], c->pos[1]);
					}
					arr[count] = r->saveFunc(f);
					count++;
				}
			}	
		}
		cur = cur->next;
	}
	if (count > 0) {
		int *data = (int*)calloc(3 + count, sizeof(int));
		data[0] = count;
		data[1] = c->pos[0];
		data[2] = c->pos[1];
		for (int i = 3; i < 3 + count; i++) {
			data[i] = arr[i-3];
		}
		//printf("wriitng Cell: %i, %i, count: %i\n", data[1], data[2], data[0]);
		writeBinaryInt(file, data, 3 + count);
		free(data);
	}
	free(arr);
	return count > 0;
	/*
	data = readBinaryInt(file, 3 + c->count);
	for (int i = 0; i < 3 + c->count; i++) {
		printf(" - %i - ", data[i]);
	}
	printf("\n");
	free(data);
	*/
}

void printCell(Cell *c) {
	printf("printing Cell %i, %i\n", c->pos[0], c->pos[1]);
	if (c->within != 0) {
		printList(&(c->within), "within", printForm);
	}
}

void removeDeleteForm(void *f) {
	Form *form = f;
	removeForm(form);
	deleteForm(form);
}

void freeCell(Cell *c) {
	if (c->within != 0) {
		if (c->count > 0) {
			Form **forms = getCellContents(c);
			int count = c->count;
			for (int i = 0; i < count; i++) {
				removeForm(forms[i]);
				deleteForm(forms[i]);
			}
			free(forms);
		}
		//linkedList *cur = c->within;
		freeListSaveObj(&c->within);
		//deleteList(&(c->within), removeDeleteForm);
	}
	free(c);
}
