#include "Tile.h"

float *translations;
int dim;
linkedList *TileSets;
int tileCount = 0;
GLuint tileVAO = 0;

void initTileSets() {
	printf("initializing tile seets\n");
	TileSets = makeList();
	tileVAO = makeSpriteVao(1, 1);
}

TileSet *makeTileSet(Anim *a, int xd, int yd, int mx, int my) {
	TileSet *ts = (TileSet*)calloc(sizeof(TileSet), 1);
	ts->set = a;
	ts->typeID = -1;
	GLuint tileShader = getSP(2);
	glUseProgram(tileShader);
	//printf("diemnsions recieved %i, %i\n", xd, yd);
	ts->color = makeDrawScreen(xd, yd, mx, my, 1, 4, true, 1);
	ts->trans = makeDrawScreen(xd, yd, mx, my, 3, 3, false, 0);
	ts->rot = makeDrawScreen(xd, yd, mx, my, 4, 4, true, 0);
	ts->texture = makeDrawScreen(xd, yd, mx, my, 5, 2, true, 0);
	addTileSet(ts);
	return ts;
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

DrawScreen *makeDrawScreen(int dimensionX ,int dimensionY, int maxDimensionX ,int maxDimensionY, int location, int stride, bool base, float defaultVal) {
	DrawScreen *ds = (DrawScreen*)calloc(sizeof(DrawScreen), 1);
	ds->maxX = maxDimensionX;
	ds->maxY = maxDimensionY;
	//printf("maxDs: %i, %i\n", maxDimensionX, maxDimensionY);
	ds->data = (float*)calloc(sizeof(float), (maxDimensionX) * (maxDimensionY) * stride);
	for (int i = 0; i < maxDimensionX * maxDimensionY * stride; i++) {
		ds->data[i] = defaultVal;
	}
	ds->stride = stride;
	ds->location = location;

	sizeDrawScreen(ds, dimensionX, dimensionY, base);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return ds;
}

void sizeDrawScreen(DrawScreen *ds, int newSizeX, int newSizeY, bool base) {
	if (newSizeX > 0 && newSizeY > 0) {// && newSizeX <= ds->maxX && newSizeY <= ds->maxY) {
		//printf("SIZING DRAWSCREN %i - %i\n", newSizeX, newSizeY);
		ds->dimensionX = newSizeX;
		ds->dimensionY = newSizeY;
		initializeData(ds, base);
		glGenBuffers(1, &(ds->vbo));
		glBindBuffer(GL_ARRAY_BUFFER, ds->vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((ds->dimensionX) * (ds->dimensionY) * ds->stride), &((ds->data)[0]), GL_STATIC_DRAW);
		setScreenVBO(ds);
	}

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
	glVertexAttribDivisor(ds->location,1);
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

void bindData(DrawScreen *ds) {
	glBindBuffer(GL_ARRAY_BUFFER, ds->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * ((ds->dimensionX) * (ds->dimensionY) * ds->stride), &((ds->data)[0]), GL_STATIC_DRAW);
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
