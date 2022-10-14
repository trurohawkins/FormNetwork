Action *makeJump(moveVar *mv, Action *n_grav) {
	jumpVar *jv = (jumpVar*)calloc(1, sizeof(jumpVar));
	jv->move = mv;
	jv->grav = n_grav;
	jv->maxJP = 4;
	jv->jumpPow = 5;
	jv->jumpCount = 0;
	jv->jumpMax = 2;
	Action *a = makeAction(&jump, jv);
	a->active = 0;
	return a;
}

void startJump(Form *f, Action *a) {
	jumpVar *jv = (jumpVar*)a->vars;
		if (jv->jumpCount < jv->jumpMax) {
			jv->jpGoal = jv->maxJP;
			jv->curJP = 0;
			setForce(jv->move, -1, 0);
			jv->grav->active = 0;
			a->active = 1;
			jv->jumpCount++;
		}
}

void jump(Form *f, Action *a) {
	jumpVar *jv = (jumpVar*)a->vars;
	int dir = 0;
	if(jv->curJP != jv->jpGoal) {
		dir = sign(jv->jpGoal - jv->curJP);
		jv->curJP += dir;
	} else if (jv->jpGoal > 0){
		jv->jpGoal = 0;
	} else {
		dir = 0;
		jv->grav->active = 1;
		//if (checkCol(f->pos[0], f->pos[1]-1) != 0 ||checkCol(f->pos[0]+1, f->pos[1]) != 0 
		//		|| checkCol(f->pos[0]-1, f->pos[1]) != 0 ) {
		if (checkSide(f, 0, -1, false) != 0 || checkSide(f, 1, 0, false) != 0 || checkSide(f, -1, 0, false)) {
			a->active = 0;
			jv->jumpCount = 0;
		}
	}
	if (dir != 0) {
		addForce(jv->move, 0, dir * jv->jumpPow);
	}
}
