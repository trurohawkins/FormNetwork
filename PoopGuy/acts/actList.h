typedef struct moveVar {
	int speedCounter;
	int speed;
	int maxForce;
	int dir[2];
	int force[2];
	int mass;
	int forceCounter[2];
	int decel[2];
	int decelCounter[2];
	int decelSpeed[2];
} moveVar;

Action *makeMove();
moveVar *makeMoveVar();
int move(Form *f, Action *a);
void decelerate(moveVar *mv);
void addForce(void *m, int powX, int powY);
void setForce(void *m, int x, int y);
void setMoveSpeed(void *m, int n_speed);
void setMoveDirX(void *m, int n_dir);
void setMoveDirY(void *m, int n_dir);
void setMoveMass(void *m, int n_mass);
void setDecel(void *m, int xory, int d_interval, int d_speed);

#include "control.h"
#include "gravity.h"
#include "jump.h"
#include "eatPoop.h"
