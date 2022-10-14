typedef struct formStack {
	//Form *type;
	int type;
	int count;
} formStack;

typedef struct eatPooVar {
	linkedList *stomach;
	int dir;
	bool pooping;
	bool eating;
	float bite[2];
	int xBite;
	int yBite;
	int biteCounter;
	int biteInterval;
} eatPooVar;

Action *makeStomach(Form *f, float biteX, float biteY);

void changeDir(eatPooVar *ep, Form* f, int dir);
void stomachStuff(Form *f, Action *a);
void addToStack(Form *f, Action *a);
Form *removeFromStack();
