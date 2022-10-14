typedef struct gravVar {
	moveVar *move;
	int sideCol;
	int friction;
	int fricCount;
} gravVar;

Action *makeGravity();
void setFriction(Action *a, int n_fric);
void gravity(Form *f, Action *a);
