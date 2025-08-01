#include "World.h"

World* theWorld = 0;
linkedList *check = 0;
linkedList *outOfBounds = 0;

void makeWorld(int x, int y) {	
	World *newWorld = (World*)calloc(1, sizeof(World));
	Cell ***mudBall = (Cell***) calloc( x, sizeof(Cell**));

	for (int i = 0; i < x ; i += 1) {
		mudBall[i] = (Cell**) calloc(y , sizeof(Cell*));
		for (int j = 0; j < y; j++) {
			mudBall[i][j] = makeCell(i,j);
		}
	}
	newWorld->map = mudBall;
	newWorld->x = x;
	newWorld->y = y;
	newWorld->terrain = makeList();
	theWorld = newWorld;
	makeInert();
	addToList(&outOfBounds, inert);
	//outOfBounds = calloc(1, sizeof(Form*));
	//outOfBounds[0] = inert;
}

World *getWorld() {
	return theWorld;
}

void addToTerrain(Form *f) {
	f->terrain = true;
	addToList(&(theWorld->terrain), f);
}

void deleteWorld() {
	if (!theWorld) {
		return;
	}
	for (int i = 0; i < theWorld->x ; i += 1) {
		for (int j = 0; j < theWorld->y; j++) {
			/*
			Form *f = theWorld->map[i][j];
			if (f != NULL) {
				if (f->id == 10) {
					deleteForm(f);
				}
			}
			*/
		//	printCell(theWorld->map[i][j]);
			freeCell(theWorld->map[i][j]);
		}
		free(theWorld->map[i]); 
	}
	deleteTerrain();

	free(theWorld->map);
	free(theWorld);
}

void deleteTerrain() {
	linkedList *t = theWorld->terrain;
	linkedList *next;
	while (t != NULL) {
		next = t->next;
		if (t->data != NULL) {
			deleteForm((Form*)t->data);
		}
		free(t);
		t = next;
	}
}

bool placeForm(float x, float y, TYPE *form) {
	/*
	int mod[2] = {0,0};
	if (form->size[0] != 0 && form->size[1] != 0) {
		for (int i = 0; i < 2; i++) {
			if (form->size[i] % 2 == 0) {
				mod[i] = -0.5;
			}
		}
	}
	*/
	//bool solidForm = checkFormIsSolid(form);
	//printf("placing form at %f, %f\n", x, y);
	form->pos[0] = x;// + mod[0];
	form->pos[1] = y;// + mod[1];
	if (form->size[0] == 0 && form->size[1] == 0) {
		int xp = floor(x);
		int yp = floor(y);
		if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
			/*
			if (solidForm) {
				Form *f = getSolidForm(theWorld->map[xp][yp]);
				if (f != NULL && f != form) {
					//deleteForm(f);
				}
			}
			*/
			//theWorld->map[xp][yp] = form;
			addToCell(theWorld->map[xp][yp], form);
			return true;
		}
	} else {
		//printf("placing for mof size %i, %i\n", form->size[0], form->size[1]);
		/*
		for (int i = 0; i < form->size[0]; i++) {
			for (int j = 0; j < form->size[1]; j++) {
				int xp = floor(x) + form->body[i][j][0];
				int yp = floor(y) + form->body[i][j][1];
				if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
					// commeneted
					if (solidForm) {
						Form *f = getSolidForm(theWorld->map[xp][yp]);
						if (f != NULL && f != form) {
							//deleteForm(f);
						}
					}
					//
					addToCell(theWorld->map[xp][yp],form);
				}
			}
		}
		*/
		bool inWorld = true;
		for (int i = 0; i < form->bLen * 2; i+=2) {
			int xp = floor(x) + form->body[i];
			int yp = floor(y) + form->body[i+1];
			if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
				addToCell(theWorld->map[xp][yp],form);
			} else {
				inWorld = false;
			}
		}
		return inWorld;
	}
	return false;
}

linkedList *scanCell(int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		return theWorld->map[x][y]->within;
	} else {
		if (!inert) {
			makeInert();
		}
		inert->pos[0] = x;
		inert->pos[1] = y;
		return outOfBounds;
	}
}

bool checkCollision(Form *f, int x, int y, bool solid) {
	linkedList *check = scanCell(x, y);
	while (check) {
		if (check->data) {	
			Form *chk = check->data;
			if ((solid && chk->solid || !solid) 
			&& !compareForms(chk, f) && canCollide(f, chk)) {
				//printf("form hit: %i\n", chk->id);
				return true;
			}
		}
		check = check->next;
	}
	return false;
}

bool checkColAtPos(Form *form, int x, int y, bool solid) {
	if (form->size[0] == 0 && form->size[1] == 0) {
		return checkCollision(form, x, y, true);
	} else {
		for (int i = 0; i < form->bLen * 2; i += 2) {
			int xp = x + form->body[i];//[j][0];
			int yp = y + form->body[i+1];//[j][1];
			if (checkCollision(form, xp, yp, solid)) {
				return true;
			}
		}
	}
	return false;
}

bool checkColSideAtPos(Form *f, float xp, float yp, int xd, int yd, bool solid) {
	if (xd != 0) {
		int side = 3;
		if (xd < 0) {
			side = 1;
		}
		if (checkColSideI(f, xp, yp, side, solid)) {
			return true;
		}
	}
	if (yd != 0) {
		int side = 0;
		if (yd < 0) {
			side = 2;
		}
		if (checkColSideI(f, xp, yp, side, solid)) {
			return true;
		}
	}
	return false;
}

bool checkColSideI(Form *f, float xp, float yp, int dir, bool solid) {
	int size = 0;
	if (dir % 2 == 1) {
		size = 1;
	}
	if (dir == 1) {
		//printf("pos: %f, %f\n", xp ,yp);
	}
	for (int i = 0; i < f->sides[dir][0]; i++) {
		int xc = xp + f->sides[dir][i*2+1];
		int yc = yp + f->sides[dir][i*2+2];
		if (dir == 1) {
			//printf("checking %i, %i\n", xc, yc);
		}
		//checkColAddList(&content, f, xc, yc, solid);//floor(xp) + col, (floor(yp) - f->size[1]/2) + i);	
		if (checkCollision(f, xc, yc, solid)) {
			return true;
		}
	}
	return false;
}

linkedList *checkForm(int x, int y, bool solid) {
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		if (solid) {
			return checkSolidForm(theWorld->map[x][y]);
		} else {
			return theWorld->map[x][y]->within;
		}
	} else {
		clearCheck();
		if (inert == 0) {
			makeInert();
		}
		inert->pos[0] = x;
		inert->pos[1] = y;
		addToList(&check, inert);
		return check;
	}
}

linkedList *checkCol(Form *f, int x, int y, bool solid) {
	linkedList *content = 0;
	linkedList *check = checkForm(x, y, solid);
	linkedList *c = check;
	bool gotInert = false;
	while (check) {
		if (check->data) {	
			Form *chk = check->data;
			/*
			bool cc = canCollide(f, check->data);
			if (chk->id == 420) {
				gotInert = true;	
			}
			*/
			if ((solid && chk->solid || !solid) 
			&& !compareForms(chk, f) && canCollide(f, chk)) {
				addToList(&content, check->data);
			}
		}
		check = check->next;
	}
	if (gotInert) {
		//freeListSaveObj(&c);
	}
	return content;
}

void checkColAddList(linkedList **list, Form *f, int x, int y, bool solid) {
	linkedList *check = checkCol(f, x, y, solid);
	linkedList *c = check;
	bool gotInert = false;
	while (check) {
		if (check->data) {
			Form *poo = check->data;
			if  (poo->id == 420) {
				gotInert = true;
			}
			if (!isInList(list, check->data)) {
				addToList(list, check->data);
			} else {
				//break;
			}
		}
		check = check->next;
	}
	//checkCol always makes a list
	if (c) {// && (solid || gotInert)) {
		freeListSaveObj(&c);
	}
}


bool checkColliderPos(Collider *c, int x, int y) {
	for (int i = 0; i < c->bLen * 2; i += 2) {
		int xp = x + c->body[i];//[i][j][0];
		int yp = y + c->body[i+1];;//[j][1];
		if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
			if (checkForm(xp, yp, true)) {
				return true;
			}
		} else {
			return true;
		}
	}
	return false;
}

bool checkPosCol(Form *form, int x, int y) {
	if (form->size[0] == 0 && form->size[1] == 0) {
		if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
			linkedList *c = checkCol(form, x, y, true);
			if (c) {
				freeListSaveObj(&c);
				return true;
			} else {
				return false;
			}
		}	 else {
			return true;
		}
	} else {
		/*
		for (int i = 0; i < form->size[0]; i++) {
			for (int j = 0; j < form->size[1]; j++) {
				int xp = x + form->body[i][j][0];
				int yp = y + form->body[i][j][1];
				if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
					linkedList *c = checkCol(form, xp, yp, true);
					if (c) {
						freeListSaveObj(&c);
						return true;
					}
					freeListSaveObj(&c);
				} else {
					return true;
				}
			}
		}
		*/
		for (int i = 0; i < form->bLen * 2; i+= 2) {
			int xp = x + form->body[i];//[j][0];
			int yp = y + form->body[i+1];//[j][1];
			if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
				linkedList *c = checkCol(form, xp, yp, true);
				if (c) {
					freeListSaveObj(&c);
					return true;
				}
				freeListSaveObj(&c);
			} else {
				return true;
			}
		}
		return false;
	}
}

linkedList *checkPos(Form *form, int x, int y, bool solid) {
	linkedList *solids = 0;//makeList();
	if (form->size[0] == 0 && form->size[1] == 0) {
		return checkCol(form, x, y, true);
	} else {
		for (int i = 0; i < form->bLen * 2; i += 2) {
			int xp = x + form->body[i];//[j][0];
			int yp = y + form->body[i+1];//[j][1];
			checkColAddList(&solids, form, xp, yp, solid);
		}
	}
	return solids;
}

linkedList *checkSideI(Form *f, float xp, float yp, int dir, bool solid) {
	linkedList *content = 0;
	int size = 0;
	if (dir % 2 == 1) {
		size = 1;
	}
	if (dir == 1) {
		//printf("pos: %f, %f\n", xp ,yp);
	}
	for (int i = 0; i < f->sides[dir][0]; i++) {
		int xc = xp + f->sides[dir][i*2+1];
		int yc = yp + f->sides[dir][i*2+2];
		if (dir == 1) {
			//printf("checking %i, %i\n", xc, yc);
		}
		checkColAddList(&content, f, xc, yc, solid);//floor(xp) + col, (floor(yp) - f->size[1]/2) + i);	
	}
	return content;
}

linkedList *checkSide(Form *f, float xp, float yp, int xd, int yd, bool solid) {
	linkedList *solids = 0;//makeList();
	if (xd != 0) {
		int side = 3;
		if (xd < 0) {
			side = 1;
		}
		solids = checkSideI(f, xp, yp, side, solid);
		//printf("checking side %i\n", side);
	}
	if (yd != 0) {
		int side = 0;
		if (yd < 0) {
			side = 2;
		}
		for (int i = 0; i < f->sides[side][0]; i++) {
			int xc = xp + f->sides[side][i*2+1];
			int yc = yp + f->sides[side][i*2+2];
			//printf("checksdie ycheck %i, %i\n", xc, yc);
			//linkedList *check = checkCol(f, xp + f->sides[side][i*2], yp + f->sides[side][i*2+1]);//floor(xp) + col, (floor(yp) - f->size[1]/2) + i);
			checkColAddList(&solids, f, xc, yc, solid);//floor(xp) + col, (floor(yp) - f->size[1]/2) + i);
			/*
			linkedList *c = check;
			while (check) {
				if (check->data) {
					if (!isInList(&solids, check)) {
						addToList(&solids, check->data);
					} else {
						break;
					}
				}
				check = check->next;
			}
			freeListSaveObj(&c);
*/
		}
	}

	return solids;
}

bool checkColSide(Form *f, float xp, float yp, int xd, int yd) {
	if (xd != 0) {
		int side = 3;
		if (xd < 0) {
			side = 1;
		}
		for (int i = 0; i < f->sides[side][0]; i++) {
			int xc = xp + f->sides[side][i*2+1];
			int yc = yp + f->sides[side][i*2+2];
			//printf("(%i, %i)\n", xc, yc);
			linkedList *check = checkCol(f, xc, yc, true);
			linkedList *c = check;
			while (check) {
				if (check->data) {
					//printf("checking item %f\n", ((Form*)check->data)->id);
					if (!compareForms(check->data, f)) {
						//Form *poo = check->data;
						//printf("x checked and got %i\n", poo->id);
						freeListSaveObj(&c);
						return true;
					}
				}
				check = check->next;
			}
			freeListSaveObj(&c);
		}
	}
	if (yd != 0) {
		int side = 0;
		if (yd < 0) {
			side = 2;
		}
		for (int i = 0; i < f->sides[side][0]; i++) {
			linkedList *check = checkCol(f, xp + f->sides[side][i*2+1], yp + f->sides[side][i*2+2], true);//floor(xp) + col, (floor(yp) - f->size[1]/2) + i);
			linkedList *c = check;
			while (check) {
				if (check->data) {
					if (!compareForms(check->data, f)) {
						if (f->id == 69) {
							Form *poo = check->data;
							//printf("y on side %i checked and got %i\n", side, poo->id);
						}
						freeListSaveObj(&c);
						return true;
					}
				}
				check = check->next;
			}
			freeListSaveObj(&c);
		}
	}
	return false;
}

bool checkSideForVal(Form *f, float xp, float yp, int xd, int yd, char *stat) {
	if (xd != 0) {
		int side = 3;
		if (xd < 0) {
			side = 1;
		}
		for (int i = 0; i < f->sides[side][0]; i++) {
			int xc = xp + f->sides[side][i*2+1];
			int yc = yp + f->sides[side][i*2+2];
			if (xc >= 0 && yc >= 0 && yc < theWorld->y && xc < theWorld->x) {
				linkedList *forms = theWorld->map[xc][yc]->within;
				while (forms) {
					if (forms->data) {
						Form *chk = forms->data;
						float *v = getStat(chk, stat);
						if (v != 0) {
							return true;
						}
					}
					forms = forms->next;
				}
			} else {
				return false;
			}
		}

	}
	if (yd != 0) {
		int side = 0;
		if (yd < 0) {
			side = 2;
		}
		for (int i = 0; i < f->sides[side][0]; i++) {
			int xc = xp + f->sides[side][i*2+1];
			int yc = yp + f->sides[side][i*2+2];
			if (xc >= 0 && yc >= 0 && yc < theWorld->y && xc < theWorld->x) {
				linkedList *forms = theWorld->map[xc][yc]->within;
				while (forms) {
					if (forms->data) {
						Form *chk = forms->data;
						float *v = getStat(chk, stat);
						if (v != 0) {
							return true;
						}
					}
					forms = forms->next;
				}
			} else {
				return false;
			}
		}
	}
	return false;
}

linkedList *takeForm(int x, int y) {
	//Form *form = 0;S
	linkedList *forms = 0;
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		/*
		form = theWorld->map[x][y];
		theWorld->map[x][y] = 0;
		*/
		forms = getSolidForm(theWorld->map[x][y]);
	}
	return forms;
}

Form *removeFormPos(Form *f, int x, int y) {
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
	//takeForm(form->pos[0], form->pos[1]);
		removeFromCell(theWorld->map[x][y], f);
	}

}

Form *removeForm(Form* form) {
	if (form->size[0] == 0 && form->size[1] == 0) {
		int x = form->pos[0];
		int y = form->pos[1];
		if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		//takeForm(form->pos[0], form->pos[1]);
			removeFromCell(theWorld->map[x][y], form);
		}
	} else if (form->pos[0] >= 0 && form->pos[1] >= 0) {
		/*
		for (int i = 0; i < form->size[0]; i++) {
			for (int j = 0; j < form->size[1]; j++) {
				int xp = floor(form->pos[0]) + (form->body[i][j])[0];
				int yp = floor(form->pos[1]) + (form->body[i][j])[1];
				//printf("removing form from %i, %i\n", xp, yp);
				//commented
				if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
					theWorld->map[xp][yp] = 0;
				}
				//
				if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
					if (!removeFromCell(theWorld->map[xp][yp], form)) {
						printf("its not here\n");
					}
				}
				// commented
				if (takeForm(xp, yp) == 0) {
					printf("its not here\n");
				}
				//
				//takeForm(form->pos[0], form->pos[1]);
			}
		}
		*/
		for (int i = 0; i < form->bLen * 2; i+=2) {
			int xp = floor(form->pos[0]) + form->body[i];
			int yp = floor(form->pos[1]) + form->body[i+1];
			//printf("removing form %i, %i from %i, %i\n", xp, yp, form->body[0], form->body[i+1]);
			if (xp >= 0 && yp >= 0 && xp < theWorld->x && yp < theWorld->y) {
				if (!removeFromCell(theWorld->map[xp][yp], form)) {
					printf("its not here\n");
				}
			}
		}

	} else {
		printf("cant remove out of bounds form %f, %f\n", form->pos[0], form->pos[1]);
	}

//	f->pos[0] = -1;
//	f->pos[1] = -1; maybe add back? took out because in the middle of move we need to remember the old position
	return form;
}

int* getFormID(int x, int y) {
	if (x > -1 && y > -1 && x < theWorld->x && y < theWorld->y) {
		/*
		linkedList *f = checkSolidForm(theWorld->map[x][y]);
		if (f != NULL) {
			return f->id;
		}
		*/
		Form **forms = getCellContents(theWorld->map[x][y]);
		int count = theWorld->map[x][y]->count;
		int *ids = calloc(count + 1, sizeof(int));
		//printf("%i and max: %i\n", count, maxCellCount);
		for (int i = 0; i < maxCellCount && i < count; i++) {
			ids[i] = forms[i]->id; 
		}
		ids[count] = -1;
		free(forms);
		return ids;
	}
	return 0;
}

bool checkFormID(int x, int y, int id) {
	bool check = false;
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		Cell *c = theWorld->map[x][y];
		if (c) {
			Form **forms = getCellContents(c);//theWorld->map[x][y]);
			if (forms) {
				int count = theWorld->map[x][y]->count;
				//int *ids = calloc(count + 1, sizeof(int));
				for (int i = 0; i < maxCellCount && i < count; i++) {
					if (forms[i]->id == id) {
						check = true;
						break;
					}
				}
				free(forms);
			}
		}
	}
	return check;
}

Form *checkStat(int x, int y, char *stat) {
	Form *f = NULL;
	if (x >= 0 && y >= 0 && x < theWorld->x && y < theWorld->y) {
		Cell *c = theWorld->map[x][y];
		if (c) {
			linkedList *cur = c->within;
			while (cur) {
				float *s = getStat(cur->data, stat);
				if (s) {
					f = cur->data;
					break;
				}
				cur = cur->next;
			}
		}
	}
	return f;
}

void freeWorld() {
	clearCheck();
	deleteActorList();
	deleteWorld();
	freeDirections();
}


int saveForm(Form *f) {
	//printf("saving form\n");
	return f->id;
}

void makeInert() {
	inert = makeForm(0,0,0, 0, 0);
	inert->id = 420;
	inert->solid = true;
	inert->pos[0] = -1;
	inert->pos[1] = -1;
}


void writeWorld(char *file) {
	//resets file to overwrite previous data
	int xs = theWorld->x;
	int ys = theWorld->y;
	FILE *fptr = fopen(file, "wb");
	int sizes[2] = {xs, ys};
	fwrite(sizes, sizeof(int), 2, fptr);
	fclose(fptr);	
	
	int count = 0;
	for (int x = 0; x < xs; x++) {
		for (int y = 0; y < ys; y++) {
			Cell *cur = theWorld->map[x][y];
			if (cur->count > 0) {
				if (writeCell(file, cur)) {
					count++;
				}
			}
		}
	}
	//loadWorld(file);
}

bool loadWorld(char *file) {
	FILE *fptr = fopen(file, "rb");
	if (fptr != NULL) {
		int *size = readBinaryInt(fptr, 2);
		makeWorld(size[0], size[1]);
		int *cellCount = (int*)calloc(sizeof(int), 1);
		//for (int i = 0; i < count; i++) {
		while (fread(cellCount, sizeof(int), 1, fptr) > 0) {
			//int *cellCount = readBinaryInt(fptr, 1);
			int *pos = readBinaryInt(fptr, 2);
			int *residents = readBinaryInt(fptr, *cellCount);
			Cell *cur = theWorld->map[pos[0]][pos[1]];

			for (int j = 0; j < *cellCount; j++) {
				if (residents[j] < 10) {
					printf("cell at %i, %i, count = %i: ", pos[0], pos[1], *cellCount);
					printf("%i, \n", residents[j]);
				}
				FormRecipe *r = getRecipe(residents[j]);
				if (r) {
					placeForm(pos[0], pos[1], r->makeFunc(residents[j]));
				}
				/*
				if (residents[j] == 10) {
					//addToCell(cur, makeDirt(0));
					placeForm(pos[0], pos[1], makeDirt(0));
				}
				*/
			}
			free(pos);
			free(residents);
		}
		free(cellCount);
		fclose(fptr);
		return true;
	}
	return false;
}

bool clearCheck() {
	if (check == 0) {
		return false;
	} else {
		freeListSaveObj(&check);
		check = 0;
		return true;
	}
}
