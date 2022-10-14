#ifndef POOPGUY
#define POOPGUY
//#include "../actor/PoopGuy.h"

typedef struct PoopGuy {
	Actor *me;
	Action *jump;
	Action *move;
	Action *eatPoop;
	Action *control;
	int speed;
	int maxForce;
	char lastInp;
	Player *player;
} PoopGuy;

PoopGuy* makePoopGuy(int sx, int sy);
void deletePoopGuy(void *poop);
void initPoopers();
Form *makePoopPlayer (int pNum);
int savePoopPlayer(Form *f);
void up(void *pg, float val);
void left(void *pg, float val);
void down(void *pg, float val);
void right(void *pg, float val);
void xMove(void *pg, float val);
void yMove(void *pg, float val);
void poop(void *pg, float val);
void toggleEat(void *pg, float val);
void jumpStart(void *pg, float val);

void setPalette(int pNum, Anim *a);
void setAnimSprite(PoopGuy *pg);
void freePoopers();
PoopGuy **getPoopers();
int getNumPoopers();
#endif
