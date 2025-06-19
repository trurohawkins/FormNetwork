#include "Cloud.h"
World *w = 0;

Actor *makeCloud(int x, int y, int speed) {
	Form *f = makeForm(1,1,1,0,0);
	//f->solid = false; replace with collision checking
	placeForm(x, y, f);
	//printf("made cloud, is it solid? %i\n", checkFormIsSolid(f));
	if (w == 0) {
		w = getWorld();
	}

	char **sheets = (char**)calloc(sizeof(char*), 1);
	*sheets = "resources/cloud.png";
	Anim *h = makeAnim(sheets, 0, false, 1, 1);
	h->roto = 3;
	setDrawOrder(h, -1);
	setScale(h, 10, 10);
	GLuint sv = makeSpriteVao(1,1);
	animAddVao(h, sv);
	setAnim(f, h);
	free(sheets);	


	cloudVar *cv = calloc(sizeof(cloudVar*), 1);
	cv->moveInterval = speed;
	cv->moveCounter = 0;
	Actor *cloud = makeActor(f);
	Action *ch = makeAction(&cloudHover, cv);
	addAction(cloud, ch);
	addActor(cloud);
}


int cloudHover(Form *c, Action *a) {
	cloudVar *cv = (cloudVar*)(a->vars);
	/*
	if (cv->moveCounter > cv->moveInterval) {
		removeForm(c);
		if (c->pos[0] < w->x) {
			placeForm(c->pos[0] + 1, c->pos[1], c);
		} else {
			placeForm(0, c->pos[1], c);
		}
		cv->moveCounter = 0;
	} else {
		printf("cloud at %f, %f\n", c->pos[0], c->pos[1]);
		float step = 1.0 / (cv->moveInterval - 3); 
		c->pos[0] += step;
		cv->moveCounter++;
	}
	*/
	float step = 1.0 / (cv->moveInterval); 
	float next = c->pos[0] + step;
	float check = (float)(floor(c->pos[0]) + 1);
	//printf("my next pos %f i am checking %f\n", next, check);
	//if (c->pos[0] + step == floor(c->pos[0]) + 1)  {
	if (check - next < 0.0001) {
		removeForm(c);
		placeForm(check, c->pos[1], c);
		//printf("cloud at %f\n", c->pos[0]);
	} else {
		c->pos[0] += step;
	}
	return 0;
}
