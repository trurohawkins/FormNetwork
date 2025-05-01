typedef struct gravVar {
	moveVar *move;
	int sideCol;
	int friction;
	int fricCount;
} gravVar;

Action *makeGravity(moveVar *mv);
void setFriction(Action *a, int n_fric);
int gravity(Form *f, Action *a);
