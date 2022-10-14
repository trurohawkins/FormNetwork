#ifndef DRAWWORLD
#define DRAWWORLD
typedef struct AnimOrder {
	int order;
	linkedList *anims;
	linkedList *poses;
} AnimOrder;

void initWorldDrawing();
void setWorldFrame();
void drawWorld(World *w);
void tileCell(TileSet *t, float remainder, int x, int y);
AnimOrder *makeAnimOrder(int order);
void addFormToAnim(AnimOrder *ao, Form *f, Anim *anim, float x, float y);
void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize);
void freeAnimOrder(AnimOrder *ao);
void drawWorldDebug(World *w);
void drawGrid();
float getCenterX();
float getCenterY();
#endif
