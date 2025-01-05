AnimOrder *back;
AnimOrder *mid;
AnimOrder *front;

AnimOrder *makeAnimOrder(int order) {
	AnimOrder *ao = (AnimOrder*)calloc(sizeof(AnimOrder), 1);
	ao->anims = makeList();
	ao->poses = makeList();
	ao->sprites = makeList();
	ao->rotations = makeList();
	ao->order = order;
}

void addAnimToOrder(int drawOrder, Anim *anim, float x, float y, int sprite, int rotation, bool check) {
	/*
	if (anim == NULL) {
		anim = (Anim*)f->anim;
	}
	*/
	AnimOrder *ao;
	if (drawOrder > 0) {
		ao = front;
	} else if (drawOrder < 0) {
		ao = back;
	} else {
		ao = mid;
	}

	if (check && cmpList(&(ao->anims), anim, compareAnims)) {
		//printf("anim already on list not adding\n");
		return;
	}
	
	addToList(&(ao->anims), anim);
	float *xPos = (float*)calloc(1, sizeof(float));
	float *yPos = (float*)calloc(1, sizeof(float));
	*xPos = x;
	*yPos = y;
	addToList(&(ao->poses), xPos);
	addToList(&(ao->poses), yPos);
	int *spr = (int*)calloc(1, sizeof(int));
	*spr = sprite;
	addToList(&(ao->sprites), spr);
	int *roto = (int*)calloc(1, sizeof(int));
	*roto = rotation;
	addToList(&(ao->rotations), roto);
}

void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize) {
	linkedList *curAnim = ao->anims;//animList;
	linkedList *curPos = ao->poses;//posList;
	linkedList *curSprite = ao->sprites;
	linkedList *curRoto = ao->rotations;
	float *xPos;
	float *yPos;
	int *sprite;
	int *roto;
	while (curAnim != NULL) {
		if (curAnim->data != NULL) {
			xPos = (float*)(curPos->data);
			curPos = curPos->next;
			if (curPos != NULL) {
				yPos = (float*)(curPos->data);
			} else {
				printf("no yPos\n");
			}
			Anim *a = (Anim*)(curAnim->data);
			sprite = curSprite->data;
			if (*sprite != -1) {
				changeSprite(a, *sprite);
			}
			roto = curRoto->data;	
			if (*roto != -1) {
				setRoto(a, *roto);
			}
			drawSprite(a, sMatrix, xSize, ySize, *xPos, *yPos);
			curPos = curPos->next;
			curSprite = curSprite->next;
			curRoto = curRoto->next;
		}
		curAnim = curAnim->next;
	}
}

void freeAnimOrder(AnimOrder *ao) {
	freeListSaveObj(&(ao->anims));
	freeList(&(ao->poses));
	freeList(&(ao->sprites));
	freeList(&(ao->rotations));
	free(ao);
}


