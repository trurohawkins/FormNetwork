Action *makeAction(int (*n_fun)(Form*,Action*), void *n_vars) {
	Action *newAction = (Action*)calloc(1, sizeof(Action));
	newAction->active = 1;
	if (n_fun == 0) {
		newAction->fun = &defaultAction;
	} else {
		newAction->fun = n_fun;
	}
	newAction->vars = n_vars;
	return newAction;
}

void nameAction(Action *a, char *name) {
	a->name = calloc(1, strlen(name));
	memcpy(a->name, name, strlen(name));
}


void deleteAction(Action *action) {
	if (action->name) {
		free(action->name);
	}
	free(action->vars);
	//free(action);//changed because part of list and deleting that list will fre this memory
}

int defaultAction(Form *f, Action *a) {
	//printf("I(%i) am acting %i \n", f->id, f->id);
}
/*
void setActVar(Action *action, int index, int val) {
	if (index < 3 && index > 0) {
		action->var[index] = val;
	} else {
		printf("unable to set var for action\n");
	}
}
*/

