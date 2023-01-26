typedef struct controlVar {
	int moveLeft;
	int moveRight;
	int mrCount;
	int mlCount;
	moveVar *move;
} controlVar;

Action *makeControl();
void setControlsMove(void *c, moveVar *move);
int control(Form *f, Action *a);
