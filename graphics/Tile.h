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
	int renderOrder;
	void (*tileSprites)(struct TileSet*, void*, int, int);
	// replace tileSprites with setTile, or not, searching through tilesets can be intense
	// and currently thewrong Form might be set as data
	void (*setTiles)(struct TileSet*);
	void *data;
	int multi;//this can be used for making a tile set with higher resolution
} TileSet;

void initTileSets();
void freeTileSets();
TileSet *makeTileSet(Anim *a, int dx, int dy, int mx, int my, float sizeX, float sizeY);
void freeTileSet(void *ts);
void setTileVBO(TileSet *ts);
int addTileSet(TileSet *ts);
TileSet *getTile(int index);
int getTileCount();
DrawScreen *makeDrawScreen(int dimensionX, int dimensionY, int maxDimensionX, int maxDimensionY, float tileSizeX, float tileSizeY, int location, int stride, bool base, float defaultVal);
void freeDrawScreen(DrawScreen *ds);
void initializeData(DrawScreen *ds, bool base);
void setScreenVBO(DrawScreen *ds);
void setUpTiles(Anim *a, float *sMatrix, double xSize, double ySize);
void drawTileSet(TileSet *ts, float objSX, float objSY, float frameX, float frameY);
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
void tileProgram();
#endif
