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
	//newForm->roto = 0;
	//newForm->invert[0] = false;
	//newForm->invert[1] = false;
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
	//float wid = w;
	//float len = l;
	if (wid != 0 && len != 0) {
		//printf("form with body\n");
		newForm->body = (int***) calloc(wid, sizeof(int**));
		for (int i = 0; i < wid; i++) {
			newForm->body[i] = (int**) calloc(len, sizeof(int*));
		}
		/*
		wid -= 1;
		len -= 1;	
		*/
		printf("len/2: %f, floor(len/2): %f", len/2, -floor(len/2));
		int l = -floor(len/2);
		int w = -floor(wid/2);
		for (int x = 0; x < wid; x++) {
			for(int y = 0; y < len; y++) {
				newForm->body[x][y] = (int*)calloc(2, sizeof(int));
				newForm->body[x][y][0] = w + eModX + x;
				newForm->body[x][y][1] = l + eModY + y;
				int xb = (-wid/2) + x;
				int yb = (-len/2) + y;
				//printf("-len/2 = %i, len =%i\n", l, len);
				//printf("[%i][%i] = %i, %i\n", x, y, newForm->body[x][y][0], newForm->body[x][y][1]);
				//printf("val = %i, %i\n", xb, yb);
			}
		}
	} else {
		//printf("no body on this form\n");
		newForm->body = 0;
	}
	return newForm;
}

int getEdge(Form *f, int side, int d) {
	if (d > 0) {
		return f->pos[side]+(int)(f->size[side]/*+f->eMod[side]*/)/2 + 1;
	} else {
		return f->pos[side]-(int)(((f->size[side]/*-f->eMod[side]*/)/2 + 1)); 
	}
}

Form *checkSide(Form *f, int xd, int yd, bool collide) {
	Form *hit =0;
	if (xd != 0) {
		int col = 0;
		if (xd > 0) {
			col = (f->size[0]/*+f->eMod[0]*/)/2 + 1;
			//printf("\nCOL: %i\n\n", col);
		} else if (xd < 0) {
			col = -((f->size[0]/*-f->eMod[0]*/)/2 + 1);
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
			row = (f->size[1]/*+f->eMod[1]*/)/2 + 1;
		} else if (yd < 0) {
			row = -((f->size[1]/*-f->eMod[1]*/)/2 + 1);
		}
		int wid = f->size[0];///2;
		//printf("checking side, Im at %f, %f\n", f->pos[0], f->pos[1]);
		for (int i = 0; i < wid; i++) {
			//printf("checking: %f, %f\n", (f->pos[0] - f->size[0]/2) + i, f->pos[1] + row);
			hit = checkCol((f->pos[0] - f->size[0]/2) + i, f->pos[1] + row);
			if (hit != 0) {
				break;
				//printf("hit something Y\n");
			}
		}
	}
	return hit;
}

void deleteForm(void *form) {
	Form *f = (Form*)form;
	if (f->actor) {
		Actor *a = f->actor;
		a->body = 0;
		deleteActor(a);
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
	free(f->body);
	free(f);
}


bool compareForms(Form *f1, Form *f2) {
	if (f1 == 0 || f2 == 0) {
		return f1 == f2;
	}
	if (f1->color[0] == f2->color[0] && f1->color[1] == f2->color[1] && f1->color[2] == f2->color[2]) {
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