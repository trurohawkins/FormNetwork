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
extern linkedList *check;

void makeWorld(int x, int y);
World *getWorld();
void deleteWorld();
void addToTerrain(Form *f);
void deleteTerrain();
bool placeForm(float x, float y, Form *form);

linkedList *scanCell(int x, int y);
bool checkCollision(Form *f, int x, int y, bool solid);
bool checkColAtPos(Form *form, int x, int y, bool solid);
bool checkColSideAtPos(Form *f, float xp, float yp, int xd, int yd, bool solid);
bool checkColSideI(Form *f, float xp, float yp, int dir, bool solid);

linkedList *checkForm(int x, int y, bool solid);
linkedList *checkCol(Form *f, int x, int y, bool solid);

void checkColAddList(linkedList **list, Form *f, int x, int y, bool solid);
int* getFormID(int x, int y);
bool checkFormID(int x, int y, int id);

bool checkPosCol(Form *f, int x, int y);
bool checkColSide(Form *f, float xp, float yp, int xd, int yd);
bool checkColliderPos(Collider *c, int x, int y);

linkedList *checkPos(Form *f, int x, int y, bool solid);
linkedList *checkSide(Form *form, float xp, float yp, int xd, int yd, bool solid);
linkedList *checkSideI(Form *form, float xp, float yp, int dir, bool solid);
bool checkSideForVal(Form *f, float xp, float yp, int xd, int yd, char *val);

linkedList *takeForm(int x, int y);
Form *removeForm(Form *f);
Form *removeFormPos(Form *f, int x, int y);

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
bool clearCheck();
#endif
