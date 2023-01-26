#include "god.h"

GOD *makeGodPlayer(float px, float py, int fx, int fy) {
	GOD *g = (GOD*)calloc(1, sizeof(GOD));
	g->p = makePlayer(g, 0, freeGod);
	g->p->active = false;
	g->cam = getDefaultView();
	g->pos = (int*)calloc(2, sizeof(int));
	(g->pos)[0] = px;// * g->cam->scalePower;
	(g->pos)[1] = py;// * g->cam->scalePower;
	//printf("godf poss starting at %i, %i\n", g->pos[0], g->pos[1]);
	g->move = (short*)calloc(2, sizeof(short));
	g->zoom = (bool*)calloc(2, sizeof(bool));
	g->frame = (int*)calloc(2, sizeof(int));
	g->frame[0] = fx;
	g->frame[1] = fy;
	g->speed = 1;// * g->cam->scalePower;
	g->world = getWorld();
	g->maxZoom = max(g->world->x, g->world->y) * 3;
	g->minZoom = 2;
	/*
	if (g->world->y > g->world->x) {
		g->zoomSpeedX = 2;
		g->zoomSpeedY = 2 * g->world->y / g->world->x;
	} else if (g->world->x > g->world->y) {
		g->zoomSpeedX = 2 * g->world->x / g->world->y;
		g->zoomSpeedY = 2;
	} else {
		g->zoomSpeedX = 2;
		g->zoomSpeedY = 2;
	}
	*/
	g->zoomSpeedX = 1;
	//printf("zoom speeds: %i, %i\n", g->zoomSpeedX, g->zoomSpeedY);

	Actor *god = makeActor(NULL);
	Action *move = makeAction(&moveGodView, g);
	addAction(god, move);
	addActor(god);

	addControl(g->p, "K0^", camUp);
	addControl(g->p, "K0<", camLeft);
	addControl(g->p, "K0_", camDown);
	addControl(g->p, "K0>", camRight);
	addControl(g->p, "K0-", zoomOut);
	addControl(g->p, "K0=", zoomIn);
	return g;
}

void freeGod(void *g) {
	GOD* god = (GOD*)g;
	free(god->pos);
	free(god->move);
	free(god->frame);
	//free(god); //as is now no need to free cuz its being used as the actions vars
}

void zoomOut(void *v, float f) {
	GOD* god = (GOD*)v;
	if (f  > 0) {
		god->zoom[0] = true;
	} else {
		god->zoom[0] = false;
	}
}

int minDist = 4;

void zoomIn(void *v, float f) {
	GOD* god = (GOD*)v;
	printf("zoom out %f\n", f);
	if (f > 0) {
		god->zoom[1] = true;
	} else {
		god->zoom[1] = false;
	}
}

void camUp(void *v, float f) {
	GOD *god = (GOD*)v;
	if (f > 0) {
		god->move[1] = 1;
	} else if (god->move[1] != -1) {
		god->move[1] = 0;
	}
}
void camLeft(void *v, float f) {
	GOD *god = (GOD*)v;
	if (f > 0) {
		god->move[0] = -1;
	} else if (god->move[0] != 1) {
		god->move[0] = 0;
	}
}

void camDown(void *v, float f) {
	GOD *god = (GOD*)v;
	if (f > 0) {
		god->move[1] = -1;
	} else if (god->move[1] != 1) {
		god->move[1] = 0;
	}
}

void camRight(void *v, float f) {
	GOD *god = (GOD*)v;
	if (f > 0) {
		god->move[0] = 1;
	} else if (god->move[0] != -1) {
		god->move[0] = 0;
	}
}

int counter = 0;
int speedInterval = 5;
//
int moveGodView(Form * v, Action *a) {
	if (counter < speedInterval) {
		counter++;
		return 0;
	} else {
		counter = 0;
	}
	GOD* god = (GOD*)(a->vars);
	if (god->move[0] > 0) {
		if (god->pos[0] + god->speed < god->world->x * god->cam->scalePower) {// - god->cam->frameX/2 ) {
			god->pos[0] += god->speed;
		} else {
			god->pos[0] = god->world->x * god->cam->scalePower;// - god->cam->frameX/2; 
		}
		//setCenter(god->cam, god->pos[0], god->pos[1]);

	} else if (god->move[0] < 0) {
		if (god->pos[0] - god->speed > 0) {//god->cam->frameX / 2) {
			god->pos[0] -= god->speed;
		} else {
			god->pos[0] = 0;//god->cam->frameX / 2;
		}
		//setCenter(god->cam, god->pos[0], god->pos[1]);
	}
	if (god->move[1] > 0) {
		if (god->pos[1] + god->speed < god->world->y * god->cam->scalePower ) {// - god->cam->frameY/2) {
			god->pos[1] += god->speed;
		} else {
			god->pos[1] = god->world->y * god->cam->scalePower;// - god->cam->frameY/2;
		}
		//setCenter(god->cam, god->pos[0], god->pos[1]);
	} else if (god->move[1] < 0) {
		if (god->pos[1] - god->speed > 0) {//god->cam->frameY / 2) {
			god->pos[1] -= god->speed;
		} else {
			god->pos[1] = 0;//god->cam->frameY /2;
		}
		//setCenter(god->cam, god->pos[0], god->pos[1]);
	}
	if (!god->p->active){// || (god->zoom[0] && god->zoom[1])) {
		return 0;
	}
	if (god->zoom[0]) {
		
		//if (god->frame[0] + god->zoomSpeedX < god->world->x && god->frame[1] + god->zoomSpeedY < god->world->y) {
		if (god->frame[0] + god->zoomSpeedX < god->maxZoom) {
			god->frame[0] += god->zoomSpeedX;
			//printf("frame: %i\n", god->frame[0]);
			//god->frame[1] += god->zoomSpeedY;
		} else {
			god->frame[0] = god->maxZoom;//world->x;
			//god->frame[1] = god->world->y;
			/*
			god->pos[0] = god->world->x/2;
			god->pos[1] = god->world->y/2;
			setCenter(god->cam, god->pos[0], god->pos[1]);
*/
		}
		godSetFrame(god);
	} else if (god->zoom[1]) {
		if (god->frame[0] - god->zoomSpeedX > god->minZoom) {//minDist * god->zoomSpeedX) {// && god->frame[1] - god->zoomSpeedY > minDist * god->zoomSpeedY) {
			god->frame[0] -= god->zoomSpeedX;
			//god->frame[1] -= god->zoomSpeedY;
		} else {
			god->frame[0] = god->minZoom;//minDist * god->zoomSpeedX;
			//god->frame[1] = minDist * god->zoomSpeedY;
		}
		godSetFrame(god);
	}
	godSetFrame(god);
	return 0;
}

void godOff(GOD *god) {
	//removePlayer(god->p);
	god->p->active = false;
}

void setGod(GOD *god, float px, float py, int fx, int fy) {
	god->frame[0] = fx;
	god->frame[1] = fy;
	god->pos[0] = px;
	god->pos[1] = py;
	//printf("god's pos: %f,%f\n", px, py);
}

void godOn(GOD *g) {
	//addPlayer(g->p);
	g->p->active = true;
	//g->pos[0] = g->world->x / 2;
	//g->pos[1] = g->world->y / 2;
	//setCenter(g->cam, g->pos[0], g->pos[1]);
	godSetFrame(g);
}

void godSetFrame(GOD* god) {
	//setFrame(god->cam, god->frame[0], god->frame[1]);
	//sizeScreen(god->frame[0]);
	//printf("god frame %i, %i\n", god->frame[0], god->frame[1]);
	if (god->cam->frame != god->frame[0] || god->pos[0] * god->cam->scalePower != god->cam->centerX || god->pos[1] * god->cam->scalePower != god->cam->centerY) {
		setCenter(god->cam, god->pos[0], god->pos[1]);//god->frame[0]/2, god->frame[1]/2);//(god->world->x)/2, (god->world->y)/2);
		setFrame(god->cam, god->frame[0]);
		//printf("cam %i, %i -> god pos %i, %i\n",god->cam->centerX, god->cam->centerY, god->pos[0], god->pos[1]);
		//resizeScreen();
		//god->pos[0] wv->centerX
	}
	/*
	for (int i = 0; i < getTileCount(); i++) {
		TileSet *ts = getTile(i);
		resizeTileSet(ts, god->frame[0], god->frame[1]);
	}
	*/
}
