#ifndef WORLD
#define WORLD
#define TYPE Form
typedef struct World
{
	Cell ***map; 
	int x;
	int y;
	linkedList *terrain;
	// world state?;
} World;

extern World* theWorld;

void makeWorld(int x, int y);
World *getWorld();
void deleteWorld();
void deleteTerrain();
void placeForm(float x, float y, Form *form);
int getFormID(int x, int y);
bool checkPosCol(Form *f, int x, int y);
bool checkColSide(Form *f, float xp, float yp, int xd, int yd);
linkedList *checkPos(Form *f, int x, int y);
linkedList *checkSolidPos(Form *f, int x, int y);
linkedList *checkSolidSide(Form *f, float x, float y, int xd, int yd);
Form *checkForm(int x, int y);
Form* checkCol(int x, int y);
Form *takeForm(int x, int y);
Form *removeForm(Form *f);
void freeWorld();
Form *makeDirt(int);
int saveDirt(Form *d);
Form *makeStone(int);
int saveForm(Form *f);
void dirtFloor(int height);
void makeSquare(int x, int y, int z);
void makeStoneSquare(int x, int y, int z);
void makeCircle(int x, int y, int r);
void writeWorld(char *file);
bool loadWorld(char *file);
#endif
