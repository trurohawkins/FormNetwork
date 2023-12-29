Action *makeControl() {
	controlVar *cv = (controlVar*)calloc(1, sizeof(controlVar));
	cv->moveLeft = 0;
	cv->moveRight = 0;
	cv->mrCount = 0;
	cv->mlCount = 0;
	cv->move = 0;
	Action *a = makeAction(&control, cv);
	return a;
}

void setControlsMove(void *c, moveVar *mv) {
	controlVar *cv = (controlVar*)c;
	cv->move = mv;
}

int control(Form *f, Action *a) {
	controlVar *cv = (controlVar*)(a->vars);
	int moveX = cv->moveRight - cv->moveLeft; 
	if (cv->moveRight == 1) {
		cv->mrCount++;
	} else {
		cv->mrCount = 0;
	}
	if (cv->moveLeft == 1) {
		cv->mlCount++;
	} else {
		cv->mlCount = 0;
	}
	if (moveX != 0 && (cv->mrCount > 3 || cv->mlCount > 3)) {
		//moveVar *mv = (moveVar*)(cv->pooper->move->vars);
		if (abs(cv->move->force[0] + (cv->move->speed * moveX)) <= cv->move->maxForce) {
			//printf("%i, %i\n", cv->move->speed * moveX, cv->move->maxForce);
			addForce(cv->move, cv->move->speed * moveX, 0);
		}
		//taking steps up smll walls
		if (checkColSide(f, f->pos[0], f->pos[1], moveX, 0) != 0) {
			int xCol = getEdge(f, 0, moveX);
			int yCol = getEdge(f, 1, -1) + 2;
			//if (checkSide(f, 0, 1, false) == 0 && checkCol(xCol, yCol) == 0) {
			linkedList *ll = checkCol(f, xCol, yCol, true);
			if (checkColSide(f, f->pos[0], f->pos[1], 0, 1) == 0 && ll == 0) {
				//printf("going up to %f, %f\n", f->pos[0], f->pos[1]);
				removeForm(f);
				placeForm(f->pos[0], f->pos[1] + 1, f);
			}
			freeListSaveObj(&ll);
		}
	} else if (cv->moveRight == 0 && cv->moveLeft == 0) {
		// no input reset last input character. Turning based on last inp, want to change direction w/o moving{
		//cv->pooper->lastInp = 0;
	}
	return 0;
}
