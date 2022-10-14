#ifndef CLOUDACTOR
#define CLOUDACTOR

typedef struct cloudVar {
	int moveInterval;
	int moveCounter;
} cloudVar;

Actor *makeCloud(int x, int y, int speed);
void cloudHover(Form *c, Action *a);
#endif
