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
int startJump(Form *f, Action*);
int jump(Form *f, Action* a);
