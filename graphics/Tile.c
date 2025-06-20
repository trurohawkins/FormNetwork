#include "Tile.h"

static TileSet tileSets[TILES];
static TileSet tileSet;
TileSet *tile = &tileSet;

float *translations;
int dim;
int tileCount = 0;
GLuint tileVAO = 0;

void initTileSets() {
	//printf("initializing tile seets\n");
	//TileSets = makeList();
	tileVAO = makeSpriteVao(1, 1);
}

TileSet *getTiles() {
	return tileSets;
}
/*
void freeTileSets() {
	deleteList(&TileSets, freeTileSet);
}
*/
TileSet *makeTileSet(Anim *a, int xd, int yd, float tileSizeX, float tileSizeY) {
	//printf("\n new tileset\n");
	if (tileCount >= TILES) {
		printf("you have %i tilesets already, increase in specs.h if you wnt more\n", tileCount);
		return &tileSets[TILES-1];
	}
	TileSet *ts = &tileSets[tileCount];//(TileSet*)calloc(sizeof(TileSet), 1);
	tileCount++;
	ts->set = a;
	ts->typeID = -1;
	ts->renderOrder = 0;
	ts->multi = 1;
	ts->size[0] = tileSizeX;
	ts->size[1] = tileSizeY;
	ts->dimension[0] = xd;
	ts->dimension[1] = yd;
	GLuint tileShader = getSP(2);
	glUseProgram(tileShader);
	//printf("diemnsions recieved %i, %i\n", xd, yd);
	initializeData(ts);	
	return ts;
}

void freeTileSet(void *ts) {
	TileSet *t = ts;
	freeAnim(t->set);
	free(t);
}

void setTileVBO(TileSet *ts) {
	setScreenVBO(&ts->trans);
	setScreenVBO(&ts->rot);
	setScreenVBO(&ts->color);
	setScreenVBO(&ts->texture);
}
/*
int addTileSet(TileSet *t) {
	addToList(&TileSets, t);
	return tileCount++;
}
*/
TileSet *getTile(int index) {
	if (index < tileCount && index > -1) {
		return &tileSets[index];
	} else {
		return NULL;
	}
	/*
	TileSet *ts = (TileSet*)indexList(&TileSets, index);
	if (ts == NULL) {
		printf("no good NULL tilesset\n");
	}
	return ts;
	*/
}

int getTileCount() {
	return tileCount;
}

void fillDrawScreen(DrawScreen *ds, int location, int stride) {
	ds->location = location;
	ds->stride = stride;

	//sizeDrawScreen(ds, base);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//return ds;
}

void freeDrawScreen(DrawScreen *ds) {
	free(ds);
}

//porbably outdated, depends how this goes
void sizeDrawScreen(TileSet *ts, DrawScreen *ds, bool base) {
	//initializeData(ts, ds, base);
	glGenBuffers(1, &(ds->vbo));
	bindData(ts);
	setScreenVBO(ds);
}

void bindTrans(TileSet *t) {
	int stride = t->trans.stride;
	glBindBuffer(GL_ARRAY_BUFFER, t->trans.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((t->dimension[0]) * (t->dimension[1]) * stride), &(t->tData[0]), GL_STATIC_DRAW);
}

void bindRot(TileSet *t) {
	int stride = t->rot.stride;
	glBindBuffer(GL_ARRAY_BUFFER, t->rot.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((t->dimension[0]) * (t->dimension[1]) * stride), &(t->rData[0]), GL_STATIC_DRAW);
}

void bindColor(TileSet *t) {
	int stride = t->color.stride;
	glBindBuffer(GL_ARRAY_BUFFER, t->color.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((t->dimension[0]) * (t->dimension[1]) * stride), &(t->cData[0]), GL_STATIC_DRAW);
}

void bindTexture(TileSet *t) {
	int stride = t->texture.stride;
	glBindBuffer(GL_ARRAY_BUFFER, t->texture.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((t->dimension[0]) * (t->dimension[1]) * stride), &(t->xData[0]), GL_STATIC_DRAW);
}

void bindData(TileSet *t) {
	bindTrans(t);
	bindRot(t);
	bindColor(t);
	bindTexture(t);
}

/*
void bindData(TileSet *t, DrawScreen *ds) {
	glBindBuffer(GL_ARRAY_BUFFER, ds->vbo);
	float *data = *ds->data;
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((t->dimension[0]) * (t->dimension[1]) * ds->stride), &(data[0]), GL_STATIC_DRAW);
}
*/

void initializeData(TileSet *ts) {
	fillDrawScreen(&ts->trans, 3, 3);
	for (int i = 0; i < WX * WY * 3; i++) {
		ts->tData[i] = 0;
	}
	int index = 0;
	double startX = -1 + (ts->size[0] * 0.5);
	double startY = -1 + (ts->size[1] * 0.5);		
	for (int y = 0; y < ts->dimension[1]; y++) {
		for (int x = 0; x < ts->dimension[0]; x++) {
			ts->tData[index++]	= startX + (ts->size[0] * x);
			ts->tData[index++]	= startY + (ts->size[1] * y);
			//1 visible 0 not
			ts->tData[index++] = 1;	
		}
	}
	glGenBuffers(1, &(ts->trans.vbo));
	bindTrans(ts);
	setScreenVBO(&ts->trans);
	//sizeDrawScreen(ts, &ts->trans, false);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	for (int i = 0; i < WX * WY * 4; i++) {
		ts->rData[i] = 0;
		ts->cData[i] = 1;
		if (i < WX * WY * 2) {
			ts->xData[i] = 0;
		}
	}
	index = 0;
	for (int y = -ts->dimension[1]; y < ts->dimension[1]; y+=2) { 
		for (int x = -ts->dimension[0]; x < ts->dimension[0]; x+=2) {
				ts->rData[index] = 1;	
				ts->cData[index] = 1;	
				ts->xData[index] = 1;	
				index++;
		}
	}
	fillDrawScreen(&ts->rot, 4, 4);
	glGenBuffers(1, &(ts->rot.vbo));
	bindRot(ts);
	setScreenVBO(&ts->rot);
	//sizeDrawScreen(ts, &ts->rot, true);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	fillDrawScreen(&ts->color, 1, 4);
	glGenBuffers(1, &(ts->color.vbo));
	bindColor(ts);
	setScreenVBO(&ts->color);
	//sizeDrawScreen(ts, &ts->color, true);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	fillDrawScreen(&ts->texture, 5, 2);
	glGenBuffers(1, &(ts->texture.vbo));
	bindTexture(ts);
	setScreenVBO(&ts->texture);
	//sizeDrawScreen(ts, &ts->texture, true);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}
//probably outdated, depends how this goes
/*
void initializeData(TileSet *t, DrawScreen *ds, bool base) {
	int index = 0;
	if (base) {
		for (int y = -t->dimension[1]; y < t->dimension[1]; y+=2) { 
			for (int x = -t->dimension[0]; x < t->dimension[0]; x+=2) {
					data[index++] = 1;	
			}
		}
	} else {
		double startX = -1 + (t->size[0] * 0.5);
		double startY = -1 + (t->size[1] * 0.5);		
		for (int y = 0; y < t->dimension[1]; y++) {
			for (int x = 0; x < t->dimension[0]; x++) {
				data[index++]	= startX + (t->size[0] * x);
				data[index++]	= startY + (t->size[1] * y);
				for (int i = 0; i < ds->stride-2; i++) {
					data[index++] = 1;	
				}

			}
		}
	}
}
*/
void setScreenVBO(DrawScreen *ds) {
	glEnableVertexAttribArray(ds->location);
	glBindBuffer(GL_ARRAY_BUFFER, ds->vbo);
	glVertexAttribPointer(ds->location, ds->stride, GL_FLOAT, GL_FALSE, ds->stride * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(ds->location, 1);
}

void setUpTiles(Anim *a, float *sMatrix, double xSize, double ySize) {
	float mat[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glUniformMatrix4fv(spriteRotTile, 1, GL_TRUE, mat);
	glUniformMatrix4fv(spriteTransTile, 1, GL_TRUE, mat);
	mat[3] = 0;
	mat[7] = 0;
	// for some reason gets rid of tile distortion lines when resizing screen
	xSize += 0.00001;
	ySize += 0.00001;
	mat[0] = xSize * a->ratio[0] * a->scale[0] * convertInvert(a->invert[0]);//a->flip[0];
	mat[5] = ySize * a->ratio[1] * a->scale[1] * convertInvert(a->invert[1]);//a->flip[1];
	glUniformMatrix4fv(spriteScaleTile, 1, GL_TRUE, mat);
	float tMat [] = {
		1.0, 0.0, getCoordX(a),
		0.0, 1.0, getCoordY(a),
		0.0, 0.0, 1.0,
	};
	float sMat [] = {
		a->frameX, 0.0, 0.0,
		0.0, a->frameY, 0.0,
		0.0, 0.0, 1.0,
	};
	glUniformMatrix3fv(tcTransTile, 1, GL_TRUE, tMat);
	glUniformMatrix3fv(tcScaleTile, 1, GL_TRUE, sMat);
	textureSource *ts = a->texture;
	int step = 0 * 4;
	glUniform4f(tcColorTile,(a->palette)[step],(a->palette)[step+1], (a->palette)[step+2], (a->palette)[step+3]);
	glBindTexture(GL_TEXTURE_2D, (ts->tex)[0]);
}

void drawTileSet(TileSet *ts, float objSX, float objSY, float frameX, float frameY) {
	setTileVBO(ts);
	float mat[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	setUpTiles(ts->set, mat, objSX/ts->multi, objSY/ts->multi);
	bindData(ts);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (ceil(frameX*ts->multi)) * ceil(frameY*ts->multi));
}


void resizeTileSet(TileSet *t, int newSizeX, int newSizeY) {
	t->dimension[0] = newSizeX;
	t->dimension[1] = newSizeY;
	initializeData(t);
	/*
	sizeDrawScreen(t, &t->trans, false);
	sizeDrawScreen(t, &t->rot, true);
	sizeDrawScreen(t, &t->color, true);
	sizeDrawScreen(t, &t->texture, true);
	*/
}

void setTileSize(TileSet *t, float sizeX, float sizeY) {
	t->size[0] = sizeX;
	t->size[1] = sizeY;
}

//edits final piece of data at moment
//because the x and y shouldn't change unless its a rotation screen, in which all 4 values change
//POS -  mod 1: edits visible
void editTrans(TileSet *t, int x, int y, float val) {
	int stride = t->trans.stride;
	t->tData[((y) * (t->dimension[0]) * stride) + (stride-1) + (x * stride)] = val;

}
//ROT - mod 4 - 1 - x y z w
void editRot(TileSet *t, int x, int y, float val, int mod) {
	int stride = t->rot.stride;
	t->rData[((y) * (t->dimension[0]) * stride) + (stride-mod) + (x * stride)] = val;
}

//COLOR mod 4-1 rgba
void editColor(TileSet *t, int x, int y, float val, int mod) {
	int stride = t->color.stride;
	t->cData[((y) * (t->dimension[0]) * stride) + (stride-mod) + (x * stride)] = val;
}

//TEX - mod 2- X val mod 1 - Y val
void editTexture(TileSet *t, int x, int y, float val, int mod) {
	int stride = t->texture.stride;
	t->xData[((y) * (t->dimension[0]) * stride) + (stride-mod) + (x * stride)] = val;
}

/*
void editData(TileSet *t, DrawScreen *ds, int x, int y, float val, int mod) {
	float *data = *ds->data;
	data[((y) * (t->dimension[0]) * ds->stride) + (ds->stride-mod) + (x * ds->stride)] = val;
}

float getData(TileSet *t, DrawScreen *ds, int x, int y, int mod) {
	int index = (y * (t->dimension[0]) * ds->stride) + (ds->stride-mod) + (x * ds->stride); 
	if (index >= 0 && index < (t->dimension[0]) * (t->dimension[1]) * ds->stride) {
		return *(ds->data)[index];
	} else {
		return -190;
	}
}
*/

/*
int *getXY(TileSet *t, DrawScreen *ds, int index) {
	int y = index / ((t->dimension[0]) * ds->stride);
	int x = (index - (y * (t->dimension[0]) * ds->stride)) / ds->stride;
	//printf("index %i Produced %i, %i\n", index, x, y);
	int *pair = (int*)calloc(sizeof(int), 2);
	pair[0] = x;
	pair[1] = y;
	return pair;
}
*/

void clearTrans(TileSet *t) {
	int index = 0;
	for (int y = -t->dimension[1]; y < t->dimension[1]; y+=2) { 
		for (int x = -t->dimension[0]; x < t->dimension[0]; x+=2) {
			index+=2;
			for (int i = 0; i < t->trans.stride-2; i++) {
				t->tData[index++] = 0;	
			}
		}
	}
	bindTrans(t);
}


/*
void setData(TileSet *t, DrawScreen *ds, float value) {
	int index = 0;
	float *data = *ds->data;
	for (int y = -t->dimension[1]; y < t->dimension[1]; y+=2) { 
		for (int x = -t->dimension[0]; x < t->dimension[0]; x+=2) {
			data[index++] = value;	
			data[index++] = value;	
			data[index++] = value;	
			data[index++] = value;	
		}
	}
	//do more specfic rather than binding them all
	bindData(t);
}
*/

void setRots(TileSet *t, float rad) {
	int index = 0;
	for (int y = -t->dimension[1]; y < t->dimension[1]; y+=2) { 
		for (int x = -t->dimension[0]; x < t->dimension[0]; x+=2) {
			t->rData[index++] = cos(rad);	
			t->rData[index++] = -sin(rad);	
			t->rData[index++] = sin(rad);	
			t->rData[index++] = cos(rad);	
		}
	}
	bindRot(t);
}

void setRot(TileSet *t, int x, int y, float rad) {
	editRot(t, x, y, cos(rad), 4);
	editRot(t, x, y, -sin(rad), 3);
	editRot(t, x, y, sin(rad), 2);
	editRot(t, x, y, cos(rad), 1);	
}

float dirToRad(int d) {
	if (d == 0) {
		return 0;
	} else if (d == 1) {
		return 1.5708;
	} else if (d == 2) {
		return 3.14159;
	} else if (d == 3) {
		return 4.71239;
	}
}

GLuint getTileVAO() {
	return tileVAO;
}

/*
void printData(TileSet *t, DrawScreen *ds) {
	for (int y = t->dimension[1]-1; y >= 0; y--) {
		for (int x = 0; x < t->dimension[0]; x++) {
			if (getData(t, ds, x, y, 2) == 1) {
				printf(" 1 ");
			} else {
				printf(" 0 ");
			}
		}
		printf("\n");
	}
	printf("\n");
	return;
}
*/

void printDrawScreen(DrawScreen *ds) {
	printf("drawScreen: stride: %i, vbo: %i, location: %i\n", ds->stride, ds->vbo, ds->location);
}

void setTileSetID(TileSet *ts, int id) {
	ts->typeID = id;
}

void tileProgram() {
	glUseProgram(getSP(2));
	glBindVertexArray(getTileVAO());
}

