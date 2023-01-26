#ifndef ACTION
#define ACTION
typedef struct Action {
	char active;
	void *vars;
	int (*fun)(Form*, struct Action*);
	char *name;
} Action;

Action *makeAction(int (*fun)(Form*, Action*), void *vars);
void nameAction(Action *a, char *name);
void deleteAction(Action *action);
//void setActVar(Action *action, int index, int val);
int defaultAction(Form *f, Action *a);
#endif
