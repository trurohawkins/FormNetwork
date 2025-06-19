#include "Tile.h"

float *translations;
int dim;
linkedList *TileSets;
int tileCount = 0;
GLuint tileVAO = 0;

void initTileSets() {
	//printf("initializing tile seets\n");
	TileSets = makeList();
	tileVAO = makeSpriteVao(1, 1);
}

void freeTileSets() {
	deleteList(&TileSets, freeTileSet);
}

TileSet *makeTileSet(Anim *a, int xd, int yd, int mx, int my, float tileSizeX, float tileSizeY) {
	//printf("\n new tileset\n");
	TileSet *ts = (TileSet*)calloc(sizeof(TileSet), 1);
	ts->set = a;
	ts->typeID = -1;
	ts->renderOrder = 0;
	ts->multi = 1;
	GLuint tileShader = getSP(2);
	glUseProgram(tileShader);
	//printf("diemnsions recieved %i, %i\n", xd, yd);
	ts->trans = makeDrawScreen(xd, yd, mx, my, tileSizeX, tileSizeY, 3, 3, false, 0);
	ts->color = makeDrawScreen(xd, yd, mx, my, tileSizeX, tileSizeY, 1, 4, true, 1);
	ts->rot = makeDrawScreen(xd, yd, mx, my, tileSizeX, tileSizeY, 4, 4, true, 0);
	ts->texture = makeDrawScreen(xd, yd, mx, my, tileSizeX, tileSizeY, 5, 2, true, 0);
	printf("\n\n");
	addTileSet(ts);
	return ts;
}

void freeTileSet(void *ts) {
	TileSet *t = ts;
	freeDrawScreen(t->color);
	freeDrawScreen(t->trans);
	freeDrawScreen(t->rot);
	freeDrawScreen(t->texture);
	freeAnim(t->set);
	free(t);
}

void setTileVBO(TileSet *ts) {
	setScreenVBO(ts->trans);
	setScreenVBO(ts->rot);
	setScreenVBO(ts->color);
	setScreenVBO(ts->texture);
}

int addTileSet(TileSet *t) {
	addToList(&TileSets, t);
	return tileCount++;
}

TileSet *getTile(int index) {
	TileSet *ts = (TileSet*)indexList(&TileSets, index);
	if (ts == NULL) {
		printf("no good NULL tilesset\n");
	}
	return ts;
}

int getTileCount() {
	return tileCount;
}

DrawScreen *makeDrawScreen(int dimensionX ,int dimensionY, int maxDimensionX ,int maxDimensionY, float tileSizeX, float tileSizeY, int location, int stride, bool base, float defaultVal) {
	DrawScreen *ds = (DrawScreen*)calloc(sizeof(DrawScreen), 1);
	ds->maxX = maxDimensionX;
	ds->maxY = maxDimensionY;
	ds->sizeX = tileSizeX;
	ds->sizeY = tileSizeY;
	//printf("maxDs: %i, %i * %i\n", maxDimensionX, maxDimensionY, stride);
	ds->data = (float*)calloc(sizeof(float), (maxDimensionX) * (maxDimensionY) * stride);
	//printf("drawScreen is %i big\n", (sizeof(float) * maxDimensionX * maxDimensionY * stride) + sizeof(DrawScreen));
	for (int i = 0; i < maxDimensionX * maxDimensionY * stride; i++) {
		ds->data[i] = defaultVal;
	}
	ds->stride = stride;
	ds->location = location;

	sizeDrawScreen(ds, dimensionX, dimensionY, base);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return ds;
}

void freeDrawScreen(DrawScreen *ds) {
	free(ds->data);
	free(ds);
}

void sizeDrawScreen(DrawScreen *ds, int newSizeX, int newSizeY, bool base) {
	if (newSizeX > 0 && newSizeY > 0) {// && newSizeX <= ds->maxX && newSizeY <= ds->maxY) {
		//printf("SIZING DRAWSCREN %i, %i\n", newSizeX, newSizeY);
		//printf("%f\n", ds->data[0]);
		ds->dimensionX = newSizeX;
		ds->dimensionY = newSizeY;
		initializeData(ds, base);
		glGenBuffers(1, &(ds->vbo));
		bindData(ds);
		setScreenVBO(ds);
	}

}

void bindData(DrawScreen *ds) {
	glBindBuffer(GL_ARRAY_BUFFER, ds->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((ds->dimensionX) * (ds->dimensionY) * ds->stride), &((ds->data)[0]), GL_STATIC_DRAW);
}

void initializeData(DrawScreen *ds, bool base) {
	int index = 0;
	if (base) {
		for (int y = -ds->dimensionY; y < ds->dimensionY; y+=2) { 
			for (int x = -ds->dimensionX; x < ds->dimensionX; x+=2) {
					(ds->data)[index++] = 1;	
			}
		}
	} else {
		double startX = -1 + (ds->sizeX * 0.5);
		double startY = -1 + (ds->sizeY * 0.5);		
		for (int y = 0; y < ds->dimensionY; y++) {
			for (int x = 0; x < ds->dimensionX; x++) {
				(ds->data)[index++]	= startX + (ds->sizeX * x);
				(ds->data)[index++]	= startY + (ds->sizeY * y);
				for (int i = 0; i < ds->stride-2; i++) {
					(ds->data)[index++] = 1;	
				}

			}
		}
	}
}

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
	bindData(ts->trans);
	bindData(ts->rot);
	bindData(ts->color);
	bindData(ts->texture);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (ceil(frameX*ts->multi)) * ceil(frameY*ts->multi));
}


void resizeTileSet(TileSet *t, int newSizeX, int newSizeY) {
	sizeDrawScreen(t->trans, newSizeX, newSizeY, false);
	sizeDrawScreen(t->rot, newSizeX, newSizeY, true);
	sizeDrawScreen(t->color, newSizeX, newSizeY, true);
	sizeDrawScreen(t->texture, newSizeX, newSizeY, true);
}

void setTileSize(TileSet *t, float sizeX, float sizeY) {
	t->trans->sizeX = sizeX;
	t->trans->sizeY = sizeY;
	t->rot->sizeX = sizeX;
	t->rot->sizeY = sizeY;
	t->color->sizeX = sizeX;
	t->color->sizeY = sizeY;
	t->texture->sizeX = sizeX;
	t->texture->sizeY = sizeY;
}

//edits final piece of data at moment
//because the x and y shouldn't change unless its a rotation screen, in which all 4 values change
//POS -  mod 1: edits visible
//TEX - mod 2- X val mod 1 - Y val
//ROT - mod 4 - 1 - x y z w
void editData(DrawScreen *ds, int x, int y, float val, int mod) {
	ds->data[((y) * (ds->dimensionX) * ds->stride) + (ds->stride-mod) + (x * ds->stride)] = val;
}

float getData(DrawScreen *ds, int x, int y, int mod) {
	int index = (y * (ds->dimensionX) * ds->stride) + (ds->stride-mod) + (x * ds->stride); 
	if (index >= 0 && index < (ds->dimensionX) * (ds->dimensionY) * ds->stride) {
		return ds->data[index];
	} else {
		return -190;
	}
}

int *getXY(DrawScreen *ds, int index) {
	int y = index / ((ds->dimensionX) * ds->stride);
	int x = (index - (y * (ds->dimensionX) * ds->stride)) / ds->stride;
	//printf("index %i Produced %i, %i\n", index, x, y);
	int *pair = (int*)calloc(sizeof(int), 2);
	pair[0] = x;
	pair[1] = y;
	return pair;
}

void clearData(DrawScreen *ds, bool base) {
	int index = 0;
	for (int y = -ds->dimensionY; y < ds->dimensionY; y+=2) { 
		for (int x = -ds->dimensionX; x < ds->dimensionX; x+=2) {
			if (!base) {
				index+=2;
				for (int i = 0; i < ds->stride-2; i++) {
					ds->data[index++] = 0;	
				}
			} else {
				for (int i = 0; i < ds->stride; i++) {
					ds->data[index++] = 0;	
				}
			}
		}
	}
	bindData(ds);
}

void setRots(DrawScreen *ds, float rad) {
	int index = 0;
	for (int y = -ds->dimensionY; y < ds->dimensionY; y+=2) { 
		for (int x = -ds->dimensionX; x < ds->dimensionX; x+=2) {
			ds->data[index++] = cos(rad);	
			ds->data[index++] = -sin(rad);	
			ds->data[index++] = sin(rad);	
			ds->data[index++] = cos(rad);	
		}
	}
	bindData(ds);
}

void setData(DrawScreen *ds, float value) {
	int index = 0;
	for (int y = -ds->dimensionY; y < ds->dimensionY; y+=2) { 
		for (int x = -ds->dimensionX; x < ds->dimensionX; x+=2) {
			ds->data[index++] = value;	
			ds->data[index++] = value;	
			ds->data[index++] = value;	
			ds->data[index++] = value;	
		}
	}
	bindData(ds);
}

void setRot(DrawScreen *ds, int x, int y, float rad) {
	editData(ds, x, y, cos(rad), 4);
	editData(ds, x, y, -sin(rad), 3);
	editData(ds, x, y, sin(rad), 2);
	editData(ds, x, y, cos(rad), 1);	
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

void printData(DrawScreen *ds) {
	for (int y = ds->dimensionY-1; y >= 0; y--) {
		for (int x = 0; x < ds->dimensionX; x++) {
			if (getData(ds, x, y, 2) == 1) {
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

void setTileSetID(TileSet *ts, int id) {
	ts->typeID = id;
}

void tileProgram() {
	glUseProgram(getSP(2));
	glBindVertexArray(getTileVAO());
}

