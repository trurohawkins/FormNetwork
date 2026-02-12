#include "../helper.h"
#include "Form.h"
#include "../actor/Action.h"
#include "../actor/Actor.h"
#include "../actor/ActorList.h"
#include "Cell.h"
#include "World.h"
#include "Value.h"
#include "FormSpawner.h"
Form *inert;
Form *makeForm(float r, float g, float b, float wid, float len) {
	Form *newForm = (Form *)calloc(1, sizeof(Form));
	newForm->id = -1;
	newForm->pos[0] = -1;
	newForm->pos[1] = -1;
	newForm->color[0] = r;
	newForm->color[1] = g;
	newForm->color[2] = b;
	newForm->size[0] = wid;
	newForm->size[1] = len;
	newForm->anim = 0;
	newForm->colMatrix = 0;
	newForm->solid = true;
	//newForm->roto = 0;
	//newForm->invert[0] = false;
	//newForm->invert[1] = false;
	//float wid = w;
	//float len = l;
	if (wid != 0 && len != 0) {
		newForm->body = squareBody(wid, len);
		newForm->bLen = wid * len;
		newForm->sides = squareSides(wid, len);
		//newForm->sides = squareSides(wid, len);
	} else {
		//printf("no body on this form\n");
		newForm->body = 0;
	}
	return newForm;
}

Form *makeIrregularForm(float r, float g, float b, int *body, int bLen) {
	Form *newForm = (Form *)calloc(1, sizeof(Form));
	newForm->id = -1;
	newForm->pos[0] = -1;
	newForm->pos[1] = -1;
	newForm->color[0] = r;
	newForm->color[1] = g;
	newForm->color[2] = b;
	newForm->size[0] = 1;//wid;
	newForm->size[1] = 1;//len;
	newForm->anim = 0;
	newForm->colMatrix = 0;
	newForm->solid = true;
	//newForm->roto = 0;
	//newForm->invert[0] = false;
	//newForm->invert[1] = false;
	//float wid = w;
	//float len = l;
	if (body && bLen != 0) {
		newForm->body = body;//squareBody(wid, len);
		newForm->bLen = bLen;//wid * len;
		newForm->sides = calcSides(newForm->body, newForm->bLen);//wid, len);
		//newForm->sides = squareSides(wid, len);
	} else {
		printf("no body on this form\n");
		newForm->body = 0;
	}
	return newForm;

}

int *squareBody(int wid, int len) {
	int eModX = 0;
	if ((int)wid % 2 == 0) {
		eModX = 1;//-0.5;
	} else {
		//newForm->eMod[0] = 0;
	}
	int eModY = 0;
	if ((int)len % 2 == 0) {
		eModY = 1;//-0.5;
	} else {
		//newForm->eMod[1] = 0;
	}
	/*
	int*** body = (int***) calloc(wid, sizeof(int**));
	for (int i = 0; i < wid; i++) {
		body[i] = (int**) calloc(len, sizeof(int*));
	}
	*/
	int *body = (int *)calloc(wid * len * 2, sizeof(int));
	int l = -floor(len/2);
	int w = -floor(wid/2);
	int count = 0;
	for (int x = 0; x < wid; x++) {
		for(int y = 0; y < len; y++) {
			body[count++] = w + eModX + x;
			body[count++] = l + eModY + y;
			//body[x][y] = (int*)calloc(2, sizeof(int));
			//body[x][y][0] = w + eModX + x;
			//body[x][y][1] = l + eModY + y;
		}
	}
	return body;
}

int **calcSides(int *body, int len) {
	linkedList *s[4];// = {0,0,0,0};
	for (int i = 0; i < 4; i++) {
		s[i] = makeList();
	}
	int lengths[4] = {0,0,0,0};
	int dirs[8] = {0,1, -1,0, 0,-1, 1,0};
	for (int i = 0; i < len; i++) {
		int x = body[i*2];
		int y = body[i*2+1];
		bool in[4] = {true, true, true, true};
		for (int j = 1; j < len; j++) {	
			int i2 = (i + j) % len;
			int x2 = body[i2 * 2];
			int y2 = body[i2 * 2 +1];
			if (x == x2) {
				int yDiff = abs(y - y2);
				if (yDiff == 1) {
					if (y < y2) {
						in[0] = false;	
					} else if (y > y2) {
						in[2] = false;
					}
				}
			}
			if (y == y2) {
				int xDiff = abs(x - x2);
				if (xDiff == 1) {
					if (x < x2) {
						in[3] = false;
					} else if (x > x2) {
						in[1] = false;
					}
				}
			}
		}
		for (int j = 0; j < 4; j++) {
			if (in[j]) {
				int *xs = malloc(sizeof(int));
				*xs = x + dirs[j*2];
				addToList(&s[j], xs);
				int *ys = malloc(sizeof(int));
				*ys = y + dirs[j*2+1];
				addToList(&s[j], ys);
				lengths[j]++;
				/*
				if (j == 1) {
					printf("x val = %i + %i\n", x, dirs[j*2]);
					printf("y val = %i + %i\n", y, dirs[j*2+1]);
				}
				*/
			}
		}
	}
	int **sides = calloc(4, sizeof(int**));
	for (int i = 0; i < 4; i++) {
		sides[i] = calloc(lengths[i] * 2 + 1, sizeof(int));
		sides[i][0] = lengths[i];
		linkedList *cur = s[i];
		printf("side %i:", i);
		for (int j = 1; j <= lengths[i] * 2; j++) {
			int num = *((int*)(cur->data));
			
			if (j%2 == 1) {
				printf(" (%i", num);
			} else {
				printf(",%i) ", num);
			}	
			
			sides[i][j] = num;//*((int*)(cur->data));
			cur = cur->next;
		}
		printf("\n");
		freeList(&s[i]);
	}
	return sides;
}

void addToBody(Form *f, int *newGrowth, int amount) {
	//if (f->pos[0] != -1 && f->pos[1] != -1)  {
		int lengths[4] = {f->sides[0][0], f->sides[1][0], f->sides[2][0], f->sides[3][0]};
		int dirs[8] = {0,1, -1,0, 0,-1, 1,0};
		//printf("adding to body of size %i\n", f->bLen);
		int addLen = amount;
		for (int i = 0; i < amount; i++) {
			int x = newGrowth[i*2];
			int y = newGrowth[i*2+1];
			//printf("checking for pos (%i, %i)\n", x, y);
			bool good = true;
			//int *newBody = calloc((f->bLen + 1) * 2, sizeof(int));
			bool in[4] = {true, true, true, true};
			for (int j = 0; j < f->bLen; j++) {
				int bx = f->body[j*2];
				int by = f->body[j*2+1];
				if (bx == x && by == y) {
					good = false;
					newGrowth[i*2] = INT_MIN;
					newGrowth[i*2+1] = INT_MIN;
					addLen--;
					printf("already ehrei [%i] %i, %i\n", j, bx, by);
					break;
				} else {
					if (x == bx) {
						int yDiff = abs(y - by);
						if (yDiff == 1) {
							if (y < by) {
								in[0] = false;	
							} else if (y > by) {
								in[2] = false;
							}
						}
					}
					if (y == by) {
						int xDiff = abs(x - bx);
						if (xDiff == 1) {
							if (x < bx) {
								in[3] = false;
							} else if (x > bx) {
								in[1] = false;
							}
						}
					}
				}
			}
			if (newGrowth[i*2] > INT_MIN && newGrowth[i*2+1] > INT_MIN) {
				for (int j = 0; j < 4; j++) {
					if (in[j]) {
						bool gotIn = false;
						int xc = x + dirs[j*2];
						int yc = y + dirs[j*2+1];
						//printf("adding (%i, %i) to side %i\n", xc, yc, j);
						for (int k = 0; k < f->sides[j][0]; k++) {
							int xDiff = abs(xc - f->sides[j][k*2+1]);
							int yDiff = abs(yc - f->sides[j][k*2+2]);
							/*
							printf("point (%i, %i)\n", f->sides[j][k*2+1], f->sides[j][k*2+2]);
							printf("Diff == %i, %i\n", xDiff, yDiff);
							*/
							if (j % 2 == 1) {
								//check y value
								if (yDiff == 0 && xDiff == 1) {
									f->sides[j][k*2+1] = xc;
									f->sides[j][k*2+2] = yc;
									gotIn = true;
									break;
								}
							} else {
								//check x value
								if (xDiff == 0 && yDiff == 1) {
									f->sides[j][k*2+1] = xc;
									f->sides[j][k*2+2] = yc;
									gotIn = true;
									break;
								}
							}
						}
						if (!gotIn) {
							/*
							printf("adding to new\n");
							for (int k = 0; k < f->sides[j][0]; k++) {
								printf(" (%i, %i) ", f->sides[j][k*2+1], f->sides[j][k*2+2]);

							}
							printf("\n");
							*/
							//add to side
							int newLen = f->sides[j][0] + 1;
							int *newSide = calloc(newLen * 2 + 1, sizeof(int));
							memcpy(newSide, f->sides[j], (f->sides[j][0] * 2 + 1) * sizeof(int));
							newSide[0] = newLen;
							newSide[newLen*2-1] = xc;
							newSide[newLen*2] = yc;
							free(f->sides[j]);
							f->sides[j] = newSide;
							/*
							for (int k = 0; k < f->sides[j][0]; k++) {
								printf(" (%i, %i) ", f->sides[j][k*2+1], f->sides[j][k*2+2]);

							}
							printf("\n");
							*/
						}
					}
				}
			}
		}
		if (addLen > 0) {
			int *newBody = calloc((f->bLen + addLen) * 2, sizeof(int));
			memcpy(newBody, f->body, f->bLen * 2 * sizeof(int));
			int added = 0;
			for (int i = 0; i < amount; i++) {
				if (newGrowth[i*2] > INT_MIN) {
					//printf("adding %i, %i\n", newGrowth[i*2], newGrowth[i*2+1]);
					newBody[((f->bLen+added)*2)] = newGrowth[i*2];
					newBody[((f->bLen+added)*2) + 1] = newGrowth[i*2+1];
					added++;
				}
			}
			free(f->body);
			f->body = newBody;
			f->bLen += addLen;
			/*
			for (int i = 0; i < f->bLen; i++) {
				printf(" (%i, %i) ", f->body[i*2], f->body[i*2+1]);
			}
			printf("\n");
			*/
		}
			/*
			if (!good) {
				free(newBody);
				printf("didn't add %i, %i to body\n", x, y);
				continue;
			} else {
				newBody[f->bLen*2] = x;//newGrowth[i*2];
				newBody[f->bLen*2+1] = y;//newGrowth[i*2+1];
				f->bLen++;
				free(f->body);
				f->body = newBody;
			}
			*/
			/*
		}
		for (int i = 0; i < 4; i++) {
			free(f->sides[i]);
		}
		free(f->sides);
		f->sides = calcSides(f->body, f->bLen);
		*/
		/*
			for (int j = 0; j < 4; j++) {
				int xc = xp + dirs[j*2];
				int yc = yp + dirs[j*2+1];
				printf("checking (%i, %i)\n", xc, yc);
				bool self = false;
				if (xc >= 0 && yc >= 0 && xc < w->x && yc < w->y) {
					linkedList *cur = w->map[xc][yc]->within;
					while (cur) {
						Form *f2 = cur->data;
						if (f2) {
							if (f->id == f2->id) {
								self = true;
								break;
							}
						}
						cur = cur->next;
					}
				}
				if (self) {
					continue;
				}
				//add to side
				int newLen = f->sides[j][0] + 1;
				int *newSide = calloc(newLen * 2 + 1, sizeof(int));
				newSide[0] = newLen;
				for (int k = 0; k < f->sides[j][0] * 2; k++) {
					newSide[k+1] = f->sides[j][k+1];
				}
				newSide[newLen*2] = xp;
				newSide[newLen*2+1] = yp;
				free(f->sides[j]);
				f->sides[j] = newSide;
				printf("adding to side %i\n", j);
			}
		}
	}
	*/
}

int **squareSides(int wid, int len) {
	int eModX = 0;
	if ((int)wid % 2 == 0) {
		eModX = 1;//-0.5;
	} else {
		//newForm->eMod[0] = 0;
	}
	int eModY = 0;
	if ((int)len % 2 == 0) {
		eModY = 1;//-0.5;
	} else {
		//newForm->eMod[1] = 0;
	}
	int **sides = (int**) calloc(4, sizeof(int**));
	sides[0] = (int*)calloc(wid * 2 + 1, sizeof(int));
	sides[0][0] = wid;// * 2;
	sides[1] = (int*)calloc(len * 2 + 1, sizeof(int));
	sides[1][0] = len;// * 2;
	sides[2] = (int*)calloc(wid * 2 + 1, sizeof(int));
	sides[2][0] = wid;// * 2;
	sides[3] = (int*)calloc(len * 2 + 1, sizeof(int));
	sides[3][0] = len;// * 2;
	int rowP = (len + eModY)/2 + 1;
	int rowN = -((len - eModY)/2 + 1);
	/*
		 printf("wid: %f  %f\n", wid, -wid/2);
		 printf("0 check = ");
	 */
	for (int i = 0; i < wid; i++) {
		int xc = (-(int)wid/2) + i + eModX;// + eModX;
		int p = i * 2 + 1;
		sides[0][p] = xc;
		sides[0][p+1] = rowP;

		sides[2][p] = xc;
		sides[2][p+1] = rowN;
		//printf(" (%i, %i) ", xc, rowP);
	}
	/*
		 printf("\n 2 check = ");
		 for (int i = 0; i < wid; i++) {
		 printf(" (%i, %i) ", sides[2][i*2], newForm->sides[2][i*2+1]);
		 }
		 printf("\n");
	 */
	int colP = (wid + eModX)/2 + 1;
	int colN = -((wid - eModX)/2 + 1);
	//printf("1 check = ");
	for (int i = 0; i < len; i++) {
		int yc = (-(int)len/2) + i + eModY;
		int p = i * 2 + 1;
		sides[1][p] = colN;
		sides[1][p+1] = yc;

		sides[3][p] = colP;
		sides[3][p+1] = yc;
		//printf(" (%i, %i) ", colN, yc);
	}
	return sides;
}

Collider *squareCollider (int wid, int len) {
	Collider *c = calloc(1, sizeof(Collider));
	c->size[0] = wid;
	c->size[1] = len;
	c->sides = squareSides(wid, len);
	c->body = squareBody(wid, len);
	c->bLen = wid * len;
	return c;
}

void changeCollider(Form *f, Collider *c) {
	f->body = c->body;
	f->sides = c->sides;
	f->size[0] = c->size[0];
	f->size[1] = c->size[1];
	f->bLen = c->bLen;
}

Collider *getCollider(Form *f) {
	Collider *c = calloc(sizeof(Collider), 1);
	c->body = f->body;
	c->sides = f->sides;
	c->size[0] = f->size[0];
	c->size[1] = f->size[1];
	c->bLen = f->bLen;
	return c;
}

void freeCollider(Collider *c) {
	if (c->size[0] != 0 && c->size[1] != 0) {
		/*
		for (int x = 0; x < c->size[0]; x++) {
			for(int y = 0; y < c->size[1]; y++) {
				free(c->body[x][y]);
			}
		}
		for (int i = 0; i < c->size[0]; i++) {
			free(c->body[i]);
		}
		for (int i = 0; i < c->size[0] * c->size[1] * 2; i++) {
			free(c->body[i]);
		}
		*/
	}
	free(c->body);
	if (c->sides) {
		for (int i = 0; i < 4; i++) {
			free(c->sides[i]);
		}
		free(c->sides);
	}
	free(c);
}

int getEdge(Form *f, int side, int d) {
	if (d > 0) {
		return f->pos[side]+(int)(f->size[side]/*+f->eMod[side]*/)/2 + 1;
	} else {
		return f->pos[side]-(int)(((f->size[side]/*-f->eMod[side]*/)/2 + 1)); 
	}
}

/*
	 Form *checkSide(Form *f, int xd, int yd, bool collide) {
	 Form *hit =0;
	 if (xd != 0) {
	 int col = 0;
	 if (xd > 0) {
	 col = (f->size[0]+f->eMod[0])/2 + 1;
//printf("\nCOL: %i\n\n", col);
} else if (xd < 0) {
col = -((f->size[0]-f->eMod[0])/2 + 1);
}
int hei = f->size[1];//(f->size[1]-f->eMod[1])/2;
										 //printf("hei: %i - from %f\n", hei, f->size[1] + f->eMod[1]);
										 if (collide) {
//printf("checking side, Im at %f, %f\n", f->pos[0], f->pos[1]);
}
for (int i = 0; i < hei; i++) {
//if (collide) {
//	printf("p: %f, %f\n", f->pos[0] + col, (f->pos[1] - f->size[1]/2) + i);
//}
hit = checkCol(f->pos[0] + col, (f->pos[1] - f->size[1]/2) + i);
if (hit != 0) {
break;
}
}
if (hit != 0) {
//printf("hit something X\n");
return hit;
}
}
if (yd != 0) {
int row = 0;
if (yd > 0) {
row = (f->size[1]+f->eMod[1])/2 + 1;
} else if (yd < 0) {
row = -((f->size[1]-f->eMod[1])/2 + 1);
}
int wid = f->size[0];///2;
printf("checking side, Im at %f, %f\n", f->pos[0], f->pos[1]);
for (int i = 0; i < wid; i++) {
printf("checking: %f, %f\n", (f->pos[0] - f->size[0]/2) + i, f->pos[1] + row);
hit = checkCol((f->pos[0] - f->size[0]/2) + i, f->pos[1] + row);
if (hit != 0) {
break;
//printf("hit something Y\n");
}
}
}
return hit;
}
 */

void deleteForm(void *form) {
	//printf("deleting form\n");
	Form *f = (Form*)form;
	if (f->actor != 0) {
		Actor *a = f->actor;
		a->body = 0;
		//deleteActor(a);
		a->deleteMe = true;
	}
	//removeForm(f);
	
	//	printf("form: %i\n", f->id);
	if (f->size[0] != 0 && f->size[1] != 0) {
		/*
		for (int x = 0; x < f->size[0]; x++) {
			for(int y = 0; y < f->size[1]; y++) {
				free(f->body[x][y]);
			}
		}
		for (int i = 0; i < f->size[0]; i++) {
			free(f->body[i]);
		}
		*/
	}
	if (f->aCount > 0) {
		free(f->anim);
	}
	if (f->sides) {
		for (int i = 0; i < 4; i++) {
			free(f->sides[i]);
		}
		free(f->sides);
	}
	if (f->stats != NULL) {
		deleteList((linkedList**)&f->stats, freeValue);
	}
	if (f->colMatrix != 0) {
		free(f->colMatrix);
	}
	free(f->body);
	free(f);
}


bool compareForms(Form *f1, Form *f2) {
	if (f1 == 0 || f2 == 0) {
		return f1 == f2;
	}
	//printf("%i == %i\n", f1->id, f2->id);
	if (f1->id == f2->id && f1->pos[0] == f2->pos[0] && f1->pos[1] == f2->pos[1]) { //f1->color[0] == f2->color[0] && f1->color[1] == f2->color[1] && f1->color[2] == f2->color[2]) {
		return 1;
	} else {
		return 0;
	}
	}
	/*
		 void setRoto(Form *f, int degree) {
		 f->roto = degree;
		 }

		 void setInvert(Form *f, int axis, bool flipped) {
		 f->invert[axis] = flipped;
		 }
	 */
	 /*
	bool checkFormIsSolid(void *form) {
		Form *f = (Form*)form;

		//printf("formm: %p (%i, %i)\n", form, f->size[0], f->size[1]);
		if (f->size[0] != 0 || f->size[1] != 0) {
			return true;
		} else {
			return false;
		}
	}
*/
	bool isFormCenter(Form *f, int x, int y) {
		//if (f->size[0] <= 1 && f->size[1] <= 1) {
		if ((int)floor(f->pos[0]) == x && (int)floor(f->pos[1] == y)) {
			return true;
		} else {
			return false;
		}
	}

	void printForm(void *form) {
		Form *f = (Form*)form;
		printf("form id: %i size:%i, %i\n", f->id, f->size[0], f->size[1]);
	}

	int *addNoCol(Form *f, int *ids, int num) {
		int *old = f->colMatrix;
		if (num > 0) {
			f->colMatrix = calloc(num + 1, sizeof(int));
			for (int i = 0; i < num; i++) {
				f->colMatrix[i] = ids[i];
			}
			f->colMatrix[num] = -1;
		} else {
			f->colMatrix = ids;
		}
		return old;
	}

	bool canCollide(Form *a, Form *b) {
		if (a->colMatrix) {
			int i = 0;
			while (a->colMatrix[i] != -1) {
				if (a->colMatrix[i] == b->id) {
					return false;
				}
				i++;
			}
		}
		return true;
	}
	/*
		 void setStat(Form *f, float stat) {
	//printf("setting stat %f\n", stat);
	f->stat = stat;
	}
	 */


	//#include "../helper/helper.c"
#include "World.c"
#include "Value.c"
#include "Cell.c"
#include "FormSpawner.c"
#include "../actor/Action.c"
#include "../actor/Actor.c"
