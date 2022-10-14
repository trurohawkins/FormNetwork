typedef struct Action {
	char active;
	void *vars;
	void (*fun)(Form*, struct Action*);
} Action;



Action *makeAction(void (*fun)(Form*, Action*), void *vars);
void deleteAction(Action *action);
//void setActVar(Action *action, int index, int val);
void defaultAction();
