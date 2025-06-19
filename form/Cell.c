#include "Cell.h"
int maxCellCount = 8;

Cell *makeCell(int x, int y) {
	Cell *c = (Cell*)calloc(sizeof(Cell), 1);
	c->pos[0] = x;
	c->pos[1] = y;
	c->within = 0;//makeList();
	//c->content = calloc(sizeof(Form*), maxCellCount);
	c->count = 0;
}

void addToCell(Cell *c, Form *f) {
	if (f != NULL && c->count < maxCellCount) {
		/*
		if (c->within == 0) {
			c->within = makeList();
		}
		*/
		if (addToListSingle(&(c->within), f)) {
			c->count++;
		}
	} else {
		printf("%i < %i\n", c->count, (maxCellCount - 1));
		printf("tried to add NULL form to Cell ");
		printCell(c);
	}
}

Form *removeFromCell(Cell *c, Form *f) {
	/*
	for (int i = 0; i < maxCellCount; i++) {
		if (c->content[i] == f) {
			c->content[i] = 0;
			c->count--;
			//printf("form removed\n");
			return f;
		}
	}
	return 0;
	*/
	if (!c->within) {
		return 0;
	}
	void *fv = (void *)f;
	if (fv) {
		void *v = removeFromList(&(c->within), fv);
		if (v != NULL) {
			c->count--;
		}
	}
	return fv;
}

Form **getCellContents(Cell *c) {
	//return c->content;
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
				FormRecipe *r = getRecipe(f->id);
				if (r) {
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
				if (!forms[i]->terrain) {
					removeForm(forms[i]);
					deleteForm(forms[i]);
				}
			}
			free(forms);
		}
		//linkedList *cur = c->within;
		freeListSaveObj(&c->within);
		//deleteList(&(c->within), removeDeleteForm);
	}
	//free(c);
}
