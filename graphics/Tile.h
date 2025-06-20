#ifndef TILE 
#define TILE

#include "specs.h"

typedef struct DrawScreen {
	//float **data;
	int stride;
	GLuint vbo;
	int location;
} DrawScreen;

typedef struct TileSet {
	Anim *set;
	int typeID;
	int dimension[2];
	double size[2];
	float tData[WX*WY*3];
	DrawScreen trans;
	float rData[WX*WY*4];
	DrawScreen rot;
	float cData[WX*WY*4];
	DrawScreen color;
	float xData[WX*WY*2];
	DrawScreen texture;
	int renderOrder;
	void (*tileSprites)(struct TileSet*, void*, int, int);
	int multi;//this can be used for making a tile set with higher resolution
} TileSet;

extern TileSet *tile;

void initTileSets();
TileSet *getTiles();
//void freeTileSets();
TileSet *makeTileSet(Anim *a, int dx, int dy, float sizeX, float sizeY);
void freeTileSet(void *ts);
void setTileVBO(TileSet *ts);
//int addTileSet(TileSet *ts);
TileSet *getTile(int index);
int getTileCount();
void fillDrawScreen(DrawScreen *ds, int location, int stride);
void freeDrawScreen(DrawScreen *ds);
void initializeData(TileSet *ts);
//void initializeData(TileSet *t, DrawScreen *ds, bool base);
void setScreenVBO(DrawScreen *ds);
void setUpTiles(Anim *a, float *sMatrix, double xSize, double ySize);
void drawTileSet(TileSet *ts, float objSX, float objSY, float frameX, float frameY);
void sizeDrawScreen(TileSet *ts, DrawScreen *ds, bool base);
void resizeTileSet(TileSet *t, int newSizeX, int newSizeY);
void setTileSize(TileSet *t, float sizeX, float sizeY);
//void editData(TileSet *t, DrawScreen *ds, int x, int y, float val, int mod);
void editTrans(TileSet *t, int x, int y, float val);
void editRot(TileSet *t, int x, int y, float val, int mod);
void editColor(TileSet *t, int x, int y, float val, int mod);
void editTexture(TileSet *t, int x, int y, float val, int mod);
//int *getXY(TileSet *t, DrawScreen *ds, int index);
//float getData(TileSet *t, DrawScreen *ds, int x, int y, int mod);
//void bindData(TileSet *t, DrawScreen *ds);
void bindData(TileSet *t);
void bindTrans(TileSet *t);
void bindRot(TileSet *t);
void bindColor(TileSet *t);
void bindTexture(TileSet *t);
//void clearData(TileSet *t, DrawScreen  *ds, bool base);
void clearTrans(TileSet *t);
//void setData(TileSet *t, DrawScreen *ds, float value);
void setRots(TileSet *t, float rot);
void setRot(TileSet *t, int x, int y, float rad);
void tileDataPoo(DrawScreen *ds, DrawScreen *rot);
float dirToRad(int d);
GLuint getTileVAO();
//void printData(TileSet *t, DrawScreen *ds);
void printDrawScreen(DrawScreen *ds);
void setTileSetID(TileSet *t, int id);
void tileProgram();
#endif
