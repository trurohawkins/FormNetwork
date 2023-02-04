int terminalVelocity = -10;
int gravForce = 1;
int side = 1;
Action *makeGravity(moveVar *mv) {
	gravVar *gv = (gravVar*)calloc(1, sizeof(gravVar));
	gv->move = mv;
	gv->friction = 20;
	gv->fricCount = 0;
	gv->sideCol = 1;
	//moveVar *mv = makeMoveVar(); //uses its own moveVar, to avoid freeing a pointer that has already been freed
	Action *a = makeAction(&gravity, gv);
	a->active = 1;
	return a;
}

void setFriction(Action *a, int n_fric) {
	((gravVar*)a->vars)->friction = n_fric;
}

int gravity(Form *f, Action *a) {
	gravVar *gv = (gravVar*)a->vars;
	moveVar *mv = gv->move;
	int ns = 1;
	//if (checkCol(f->pos[0]-1, f->pos[1]) == 0 && checkCol(f->pos[0]+1, f->pos[1]) == 0) {
	//if (checkSide(f, -1, 0, false) == 0 && checkSide(f, 1, 0, false) == 0) {
	if (checkColSide(f, f->pos[0], f->pos[1], -1, 0) != 0 || checkColSide(f, f->pos[0], f->pos[1], 1, 0) != 0) {
		ns = 0;
	}
	if (ns != gv->sideCol && ns == 1) {
		//setForce(mv, -1, 0);
	}
	gv->sideCol = ns;
	//if (checkCol(f->pos[0], f->pos[1] - 1) == 0) {
	//if (checkSide(f, 0, -1, true) == 0) {
	if (!checkColSide(f, f->pos[0], f->pos[1], 0, -1)) {
		if (gv->fricCount >= gv->friction * gv->sideCol) {
			if (mv->force[1] >= (terminalVelocity * mv->mass) + gravForce) {
				addForce(mv, 0, -gravForce);
			}
			gv->fricCount = 0;
		} else {
			gv->fricCount++;
		}
	} else {
		//setForce(mv, -1, 0);
	}
}
