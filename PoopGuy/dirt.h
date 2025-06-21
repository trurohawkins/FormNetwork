#ifndef DIRT
#define DIRT

Form *makeDirt(int moist);
int saveDirt(Form *d);
Form *makeStone(int null);
void makeSquare(int x, int y, int z);
void makeStoneSquare(int x, int y, int z);
void makeCircle(int x, int y, int r);
void dirtFloor(int height);

#endif
