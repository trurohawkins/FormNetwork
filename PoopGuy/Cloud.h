#ifndef CLOUDACTOR
#define CLOUDACTOR

typedef struct cloudVar {
	int moveInterval;
	int moveCounter;
} cloudVar;

Actor *makeCloud(int x, int y, int speed);
int cloudHover(Form *c, Action *a);
#endif
