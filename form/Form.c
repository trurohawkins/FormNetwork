#include "Form.h"
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
	//newForm->roto = 0;
	//newForm->invert[0] = false;
	//newForm->invert[1] = false;
	//float wid = w;
	//float len = l;
	if (wid != 0 && len != 0) {
		newForm->body = makeBody(wid, len);
		newForm->sides = calcSides(wid, len);
	} else {
		//printf("no body on this form\n");
		newForm->body = 0;
	}
	return newForm;
}

int ***makeBody(int wid, int len) {
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
	int*** body = (int***) calloc(wid, sizeof(int**));
	for (int i = 0; i < wid; i++) {
		body[i] = (int**) calloc(len, sizeof(int*));
	}
	int l = -floor(len/2);
	int w = -floor(wid/2);
	for (int x = 0; x < wid; x++) {
		for(int y = 0; y < len; y++) {
			body[x][y] = (int*)calloc(2, sizeof(int));
			body[x][y][0] = w + eModX + x;
			body[x][y][1] = l + eModY + y;
		}
	}
	return body;

}
int **calcSides(int wid, int len) {
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
	sides[0] = (int*)calloc(wid * 2, sizeof(int));
	sides[1] = (int*)calloc(len * 2, sizeof(int));
	sides[2] = (int*)calloc(wid * 2, sizeof(int));
	sides[3] = (int*)calloc(len * 2, sizeof(int));
	int rowP = (len + eModY)/2 + 1;
	int rowN = -((len - eModY)/2 + 1);
	/*
		 printf("wid: %f  %f\n", wid, -wid/2);
		 printf("0 check = ");
	 */
	for (int i = 0; i < wid; i++) {
		int xc = (-(int)wid/2) + i + eModX;// + eModX;
		int p = i * 2;
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
		int p = i * 2;
		sides[1][p] = colN;
		sides[1][p+1] = yc;

		sides[3][p] = colP;
		sides[3][p+1] = yc;
		//printf(" (%i, %i) ", colN, yc);
	}
	return sides;
}

Collider *makeCollider (int wid, int len) {
	Collider *c = calloc(1, sizeof(Collider));
	c->size[0] = wid;
	c->size[1] = len;
	c->sides = calcSides(wid, len);
	c->body = makeBody(wid, len);
	return c;
}

void changeCollider(Form *f, Collider *c) {
	f->body = c->body;
	f->sides = c->sides;
	f->size[0] = c->size[0];
	f->size[1] = c->size[1];
}

Collider *getCollider(Form *f) {
	Collider *c = calloc(sizeof(Collider), 1);
	c->body = f->body;
	c->sides = f->sides;
	c->size[0] = f->size[0];
	c->size[1] = f->size[1];
	return c;
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
	printf("deleting form\n");
	Form *f = (Form*)form;
	if (f->actor) {
		Actor *a = f->actor;
		a->body = 0;
		//deleteActor(a);
		a->deleteMe = true;
	}
	//removeForm(f);
	//	printf("form: %i\n", f->id);
	if (f->size[0] != 0 && f->size[1] != 0) {
		for (int x = 0; x < f->size[0]; x++) {
			for(int y = 0; y < f->size[1]; y++) {
				free(f->body[x][y]);
			}
		}
		for (int i = 0; i < f->size[0]; i++) {
			free(f->body[i]);
		}
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
	bool checkFormIsSolid(void *form) {
		Form *f = (Form*)form;
		if (f->size[0] != 0 || f->size[1] != 0) {
			return true;
		} else {
			return false;
		}
	}

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
