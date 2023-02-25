#ifndef TILE 
#define TILE

typedef struct DrawScreen {
	float *data;
	int dimensionX;
	int dimensionY;
	float ratioX;
	float ratioY;
	int maxX;
	int maxY;
	int stride;
	GLuint vbo;
	int location;
	double sizeX;
	double sizeY;
} DrawScreen;

typedef struct TileSet {
	Anim *set;
	int typeID;
	DrawScreen *trans;
	DrawScreen *rot;
	DrawScreen *color;
	DrawScreen *texture;
} TileSet;

void initTileSets();
void freeTileSets();
TileSet *makeTileSet(Anim *a, int dx, int dy, int mx, int my);
void freeTileSet(void *ts);
void setTileVBO(TileSet *ts);
int addTileSet(TileSet *ts);
TileSet *getTile(int index);
int getTileCount();
DrawScreen *makeDrawScreen(int dimensionX, int dimensionY, int maxDimensionX, int maxDimensionY, int location, int stride, bool base, float defaultVal);
void freeDrawScreen(DrawScreen *ds);
void initializeData(DrawScreen *ds, bool base);
void setScreenVBO(DrawScreen *ds);
void sizeDrawScreen(DrawScreen *ds, int newSizeX, int newSizeY, bool base);
void resizeTileSet(TileSet *t, int newSizeX, int newSizeY);
void setTileSize(TileSet *t, float sizeX, float sizeY);
void editData(DrawScreen *ds, int x, int y, float val, int mod);
int *getXY(DrawScreen *ds, int index);
float getData(DrawScreen *ds, int x, int y, int mod);
void bindData(DrawScreen *ds);
void clearData(DrawScreen  *ds, bool base);
void setData(DrawScreen *ds, float value);
void setRots(DrawScreen *ds, float rot);
void setRot(DrawScreen *ds, int x, int y, float rad);
void tileDataPoo(DrawScreen *ds, DrawScreen *rot);
float dirToRad(int d);
GLuint getTileVAO();
void printData(DrawScreen *ds);
void setTileSetID(TileSet *t, int id);
#endif
