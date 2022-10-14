#include "poopPlayer.h"
int maxPoopers = 2;
PoopGuy **poopers = 0;
int numPoopers = 2;
int curPoopers = 0;

PoopGuy *makePoopGuy (int sx, int sy) {
	PoopGuy *pooper = (PoopGuy *)calloc(1, sizeof(PoopGuy));
	pooper->speed = 10;
	pooper->maxForce = 10;
	pooper->me = makeActor(makeForm(0.2, 1, 0.2, sx, sy));
	pooper->me->body->id = 69;
	addStat(pooper->me->body, "anim", 0);
	pooper->move = makeMove();
	Action *grav = makeGravity(pooper->move->vars);
	pooper->jump = makeJump(pooper->move->vars, grav);
	pooper->eatPoop = makeStomach(pooper->me->body, sx, sy);
	pooper->control = makeControl();
	//setPoopGuy(pooper->control->vars, pooper);
	addAction(pooper->me, pooper->move);
	addAction(pooper->me, grav);
	addAction(pooper->me, pooper->jump);
	addAction(pooper->me, pooper->eatPoop);
	addAction(pooper->me, pooper->control);
	//changeDir(pooper->eatPoop pooper->me->body, 3);
	//cv->moveRight = 1;
	//pg = pooper;
	return pooper;
}
/*
PoopGuy *getPoopGuy() {
	return pg;
}
*/

void deletePoopGuy(void *poop) {
	PoopGuy *pooper = (PoopGuy*)poop;
	eatPooVar *ep = (eatPooVar*)(pooper->eatPoop->vars);
	freeList(&(ep->stomach));
	free(pooper);
}


void initPoopers() {
	poopers = (PoopGuy**)calloc(maxPoopers, sizeof(PoopGuy));
}

Form *makePoopPlayer(int pNum) {
	PoopGuy *pooper = (PoopGuy *)calloc(1, sizeof(PoopGuy));
	//pooper->speed = 10;
	//pooper->maxForce = 10;
	pooper->me = makeActor(makeForm(0.2, 1, 0.2, 3, 3));
	pooper->me->body->id = pNum;
	addStat(pooper->me->body, "anim", 0);
	pooper->move = makeMove();
	moveVar *mv = (moveVar*)pooper->move->vars;
	mv->speed = 10;
	mv->maxForce = 10;
	Action *grav = makeGravity(pooper->move->vars);
	pooper->jump = makeJump(pooper->move->vars, grav);
	pooper->eatPoop = makeStomach(pooper->me->body, 3, 3);
	pooper->control = makeControl();
	setControlsMove(pooper->control->vars, pooper->move->vars);
	addAction(pooper->me, pooper->move);
	addAction(pooper->me, grav);
	addAction(pooper->me, pooper->jump);
	addAction(pooper->me, pooper->eatPoop);
	addAction(pooper->me, pooper->control);
	//placeForm(xp, yp, pooper->me->body);
	//checkSide(pooper->me->body, 1, 0, true);
	addActor(pooper->me);
	char *baseFile = "resources/poopGuySpriteSheet.png"; 
	int numColors = 7;
	bool generated = false;
	char **sprites = (char**)calloc(sizeof(char*), numColors);
	int sLen = strlen(baseFile);
	if (numColors > 1) {
		generated = true;
		for (int i = 0; i < numColors; i++) {
			sprites[i] = (char*)calloc(sizeof(char), sLen + 2);
			sprites[i][sLen-4] = (unsigned char)i + 48;
			strncpy(sprites[i], baseFile, sLen - 4);
			strcat(sprites[i], ".png");
		}
	} else {
		*sprites = (char*)calloc(sizeof(char), sLen + 1);
		strcpy(*sprites, baseFile); 
	}
	Anim *poo = makeAnim(sprites, numColors, generated, 4, 6);
	if (numColors != 1) {
		setPalette(pNum, poo);
	}
	for (int i = 0; i < numColors; i++) {
		free(sprites[i]);
	}
	free(sprites);
	//Anim *poo = makeAnim("resources/Heart2.png", 1, 1, tc, ts);
	setScale(poo, 4, 4);
	addSprite(poo, 0, 1);
	for (int i = 1; i < 4; i++) {
		addSprite(poo, i, 6);
	}
	//setOffset(poo, 0.0, -0.3);//so he overlaps ground makes it look like hes waalking
	GLuint spriteVao = makeSpriteVao(1, 1);
	animAddVao(poo, spriteVao);//makeSpriteVao(1, 1));
	setAnim(pooper->me->body, poo);

	Player *p = makePlayer(pooper, pNum, deletePoopGuy);
	pooper->player = p;
	//key mouse ocntrols
	if (p->num == 0) {
		addControl(p, "K0W", up);
		addControl(p, "K0A", left);
		addControl(p, "K0S", down);
		addControl(p, "K0D", right);
		addControl(p, "M01", toggleEat);
		addControl(p, "M00", poop);
		addControl(p, "K0 ", jumpStart);
	} else {
		addControl(p, "K0I", up);
		addControl(p, "K0J", left);
		addControl(p, "K0K", down);
		addControl(p, "K0L", right);
		addControl(p, "K0O", toggleEat);
		addControl(p, "K0U", poop);
		addControl(p, "K0M", jumpStart);
	}
	makeJoyButtControl(p, ';', up);
	makeJoyButtControl(p, '>', left);
	makeJoyButtControl(p, '=', down);
	makeJoyButtControl(p, '<', right);
	makeJoyButtControl(p, '0', jumpStart);
	makeJoyAxeControl(p, '5', poop);
	makeJoyAxeControl(p, '2', toggleEat);
	makeJoyAxeControl(p, '3', poop);
	makeJoyAxeControl(p, '4', toggleEat);
	makeJoyAxeControl(p, '0', xMove);
	makeJoyAxeControl(p, '1', yMove);
	poopers[curPoopers++] = pooper;
	return pooper->me->body;
}

int savePoopPlayer(Form *f) {
	return f->id;
}

void up(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	if (val > 0) {
		Anim *a = (Anim*)p->me->body->anim;
		//setInvert(p->me->body, 0, false);
		//setRoto(p->me->body, 0);
		setInvert(a, 0, false);
		setRoto(a, 0);
		eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
		changeDir(ep, p->me->body,0);
		setAnimSprite(p);
	}
}

void left(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	controlVar *cv = (controlVar*)p->control->vars;
	eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
	if (val > 0) {
		Anim *a = (Anim*)p->me->body->anim;
		setInvert(a, 0, true);
		setRoto(a, 1);
		changeDir(ep, p->me->body, 1);
		cv->moveLeft = 1;
	} else {
		cv->moveLeft = 0;
	}
	setAnimSprite(p);
}

void down(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	if (val > 0) {
		Anim *a = (Anim*)p->me->body->anim;
		setInvert(a, 0, false);
		setRoto(a, 2);
		eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
		changeDir(ep, p->me->body,2);
		setAnimSprite(p);
	}
}

void right(void *pg, float val) {
	//printf("%p is going right\n", pg);
	PoopGuy *p = (PoopGuy*)pg;	
	controlVar *cv = (controlVar*)p->control->vars;
	eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
	if (val > 0) {
		Anim *a = (Anim*)p->me->body->anim;
		setInvert(a, 0, false);	
		setRoto(a, 3);
		changeDir(ep, p->me->body, 3);
		cv->moveRight = 1;
	} else {
		cv->moveRight = 0;
	}
	setAnimSprite(p);
}

void xMove(void *pg, float val) {
	if (val > 0) {
		right(pg, 1);
	}  else if (val < 0) {
		left(pg, 1);
	} else {
		right(pg, 0);
		left(pg, 0);
	}
}
void yMove(void *pg, float val) {
	//printf("y move val: %f\n", val);
	if (val > 0) {
		up(pg, 1);
	}  else if (val < 0) {
		down(pg, 1);
	} else {
		up(pg, 0);
		down(pg, 0);
	}
}

void poop(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
	if (val > 0) {
		ep->pooping = 1;
	} else {
		ep->pooping = 0;
	}
	setAnimSprite(p);
}

void toggleEat(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	if (val > 0) {//only if when pressed not released
		eatPooVar *ep = (eatPooVar*)(p->eatPoop->vars);
		ep->eating = (ep->eating+1)%2;
		setAnimSprite(p);
	}
}

void jumpStart(void *pg, float val) {
	PoopGuy *p = (PoopGuy*)pg;	
	if (val > 0) {
		startJump(p->me->body, p->jump);
	}
}

void setPalette(int pNum, Anim *a) {
	if (pNum == 0) {
		float palette[28] = {0.023529, 0.188235, 0.027451,1, 0.090196, 0.737255, 0.109804,1, 0.098039, 0.811765, 0.121569,1, 0.109804, 0.862745, 0.133333,1, 1.000000, 1.000000, 1.000000,1, 0.098039, 0.772549, 0.117647,1, 0.105882, 0.831373, 0.129412,1};
		loadPalette(a, palette);
	} else if (pNum == 1) {
		float palette[28] = {0.0,0.0,0.0,1,  0.4,0,0.2,1,  0.3,0,0.1,1, 0.8,0,0.2,1,  1,1,1,1,  0.9,0.1,0.2,1, 0.50,0,0,1};
		loadPalette(a, palette);
	}
}


void setAnimSprite(PoopGuy *pg) {
	eatPooVar *ep = (eatPooVar*)(pg->eatPoop->vars);
	controlVar *cv = (controlVar*)(pg->control->vars);
	bool walking = cv->moveLeft > 0 || cv->moveRight > 0;
	Anim *a = (Anim*)pg->me->body->anim;
	if (walking) {
		if (ep->eating) {
			//setStat(pg->me->body, 3);
			//setStat(pg->me->body, "anim", 3);
			changeSprite(a, 3);

		} else {
			//setStat(pg->me->body, "anim", 2);
			changeSprite(a, 2);
			//setStat(pg->me->body, 2);	
		}
	} else {
		if (ep->eating) {
			//setStat(pg->me->body, 1);
			changeSprite(a, 1);
			//setStat(pg->me->body, "anim", 1);
		} else {
			//setStat(pg->me->body, 0);	
			changeSprite(a, 0);
			//setStat(pg->me->body, "anim", 0);
		}
	}
}

void freePoopers() {
	free(poopers);
}

PoopGuy **getPoopers() {
	return poopers;
}

int getNumPoopers() {
	return numPoopers;
}
