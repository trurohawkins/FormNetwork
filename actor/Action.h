#ifndef ACTION
#define ACTION
typedef struct Action {
	char active;
	void *vars;
	void (*fun)(Form*, struct Action*);
	char *name;
} Action;

Action *makeAction(void (*fun)(Form*, Action*), void *vars);
void nameAction(Action *a, char *name);
void deleteAction(Action *action);
//void setActVar(Action *action, int index, int val);
void defaultAction(Form *f, Action *a);
#endif
