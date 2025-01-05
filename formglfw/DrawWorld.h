#ifndef DRAWWORLD
#define DRAWWORLD

void initWorldDrawing();
void setWorldFrame();
void drawWorld(World *w);
void tileCell(TileSet *t, float remainder, int x, int y);
void drawWorldDebug(World *w);
void drawForm(Form *f, int buffX, int buffY);
void drawGrid();
float getCenterX();
float getCenterY();
#endif
