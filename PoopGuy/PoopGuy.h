#ifndef POOPGUY
#define POOPGUY


PoopGuy *makePoopGuy(int sx, int sy);
PoopGuy *getPoopGuy();
void deletePoopGuy(void *);
void keyPressPoopGuy(PoopGuy *, char);
void keyReleasePoopGuy(PoopGuy *, char);
/*
void up(void *pg, float val);
void left(void *pg, float val);
void down(void *pg, float val);
void right(void *pg, float val);
void xMove(void *pg, float val);
void yMove(void *pg, float val);
void poop(void *pg, float val);
void toggleEat(void *pg, float val);
void jumpStart(void *pg, float val);

void setAnimSprite(PoopGuy *pg);
*/
#endif
