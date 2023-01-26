Action *makeStomach(Form *f, float biteX, float biteY) {
	eatPooVar *ep= (eatPooVar*)calloc(1, sizeof(eatPooVar));
	ep->stomach = makeList();
	ep->pooping = 0;
	ep->eating = 0;
	ep->bite[0] = biteX;
	//smaller length for rect, use 1
	ep->bite[1] =	biteY;
	ep->biteCounter = 0;
	ep->biteInterval = 1;
	changeDir(ep, f, 3);
	Action *stom = makeAction(&stomachStuff, ep);
	stom->active = 1;
	return stom;
}

void changeDir(eatPooVar *ep, Form* f, int dir) {
	ep->dir = dir;
	if (dir%2 == 0) {//assuming 4 directions
		ep->xBite = ep->bite[0] - f->pMod[0];
		ep->yBite = 1;//ep->bite[0];
	} else {
		ep->xBite = 1;//ep->bite[0];
		ep->yBite = ep->bite[1] - f->pMod[1];
	}
}

int stomachStuff(Form *f, Action *a) {
	eatPooVar *ep = (eatPooVar*)(a->vars);
	int **d = getDirs();
	int xc;
	int yc;
	if (ep->eating != 0) {
		if (ep->biteCounter > ep->biteInterval) {
			if (ep->dir % 2 == 0) {
				xc = (f->pos[0] - (1-f->pMod[0]) + ((f->size[0]/2)+1));
				yc = getEdge(f, 1, d[ep->dir][1]);
			} else {
				xc = getEdge(f, 0, d[ep->dir][0]);
				yc = (f->pos[1] - (1-f->pMod[1]) + ((f->size[1]/2)+1));
			}
			for (int x = 0; x < ep->xBite; x++) {
				for (int y = 0; y < ep->yBite; y++) {
					//printf(" %i, %i ", xc-x, yc - y);
					Form *food = takeForm(xc - x, yc - y);
					if (food != 0) {
						addToStack(food, a);
					}
				}
			}
			//printf("\n");
			ep->biteCounter = 0;
		} else {
			ep->biteCounter++;
		}
	} else if (ep->biteCounter != 0) {
		ep->biteCounter = 0;
	}
	if (ep->pooping != 0) {
		Form *poo = removeFromStack(a);
		if (poo != 0) {
			//printf("pooping\n");
			int buttDir = (ep->dir + 2) % 4;
			bool poopGood = true;
			if (checkSide(f, d[buttDir][0], d[buttDir][1], false) != 0) {
				if (checkSide(f, d[ep->dir][0], d[ep->dir][1], true) == 0) {
					removeForm(f);
					placeForm(f->pos[0] + d[ep->dir][0], f->pos[1] + d[ep->dir][1], f);
				} else {
					Form *block =checkSide(f, d[ep->dir][0], d[ep->dir][1], false);
					poopGood = false;
				}
			}
			if (poopGood) {
				addToStack(poo, a);
				if (ep->dir % 2 == 0) {
					xc = (f->pos[0] - (1-f->pMod[0]) + ((f->size[0]/2)+1));
					yc = getEdge(f, 1, d[buttDir][1]);
				} else {
					xc = getEdge(f, 0, d[buttDir][0]);
					yc = (f->pos[1] - (1-f->pMod[1]) + ((f->size[1]/2)+1));
				}
				for (int x = 0; x < ep->xBite; x++) {
					for (int y = 0; y < ep->yBite; y++) {
						poo = removeFromStack(a);
						if (poo != 0) {
							placeForm(xc -x, yc - y, poo);
						}else {
							x = 100;
							break;
						}
					}
				}
			} else {
				addToStack(poo, a);
			}
		}
	}
	return 0;
}

int addToStack(Form *f, Action *a) {
	eatPooVar *ep = (eatPooVar*)(a->vars);
	linkedList *cur = ep->stomach;
	while (cur->data != 0) {
		formStack *fs = (formStack*)(cur->data);
		//if (compareForms(f, fs->type) == 1) { //we have a match
		if (f->id == fs->type) {
			fs->count++;
			if (f->id == 10) {
				deleteForm(f);
			}
			f = 0;
			break;
		}
		if (cur->next == 0) {
			cur->next = makeList();
		}
		cur = cur->next;
	}
	if (f != 0) {
		formStack *fs  = (formStack*)calloc(1, sizeof(formStack));
		fs->type = f->id;
		fs->count = 1;
		cur->data = fs;
		if (f->id == 10) {
			deleteForm(f);
		}
	}
	return 0;
}

Form *removeFromStack(Action *a) {
	eatPooVar *ep = (eatPooVar*)(a->vars);
	//Form *poop = 0;
	int poopType = -1;
	if (ep->stomach != 0 && ep->stomach->data != 0) {
		formStack *fs = (formStack*)(ep->stomach->data);
		fs->count--;
		poopType = fs->type;
		if (fs->count == 0) {
			linkedList *oh = ep->stomach;
			ep->stomach = ep->stomach->next;
			free(oh);
			free(fs);
			if (ep->stomach == 0) {
				ep->stomach = makeList();
			}
		}
	}
	Form *poop = 0;
	if (poopType == 10) {
		poop = makeDirt(5);
	}
	return poop;
}

