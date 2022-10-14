typedef struct jumpVar {
	moveVar *move;
	Action *grav;
	int curJP;
	int jpGoal;
	int maxJP;
	int jumpPow;
	int jumpCount;
	int jumpMax;
} jumpVar;

Action *makeJump(moveVar *mv, Action *n_grav);
void startJump(Form *f, Action*);
void jump(Form *f, Action* a);
