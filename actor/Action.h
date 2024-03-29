#ifndef ACTION
#define ACTION
typedef struct Action {
	char active;
	void *vars;
	linkedList *dynVars;
	int (*fun)(Form*, struct Action*);
	void (*del)(struct Action*);
	char *name;
} Action;

Action *makeAction(int (*fun)(Form*, Action*), void *vars);
void addDynVar(Action *a, void *ptr);
void nameAction(Action *a, char *name);
void freeAction(Action *action);
void deleteAction(Action *action);
//void setActVar(Action *action, int index, int val);
int defaultAction(Form *f, Action *a);
#endif
