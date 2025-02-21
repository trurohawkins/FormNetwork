AnimOrder *back;
AnimOrder *mid;
AnimOrder *front;

AnimOrder *makeAnimOrder(int order) {
	AnimOrder *ao = (AnimOrder*)calloc(sizeof(AnimOrder), 1);
	ao->stamps = makeList();
	ao->anims = makeList();
	ao->poses = makeList();
	ao->sprites = makeList();
	ao->rotations = makeList();
	ao->order = order;
	return ao;
}

void addAnimToOrder(int drawOrder, Anim *anim, float x, float y, int sprite, int rotation, float *tint, bool check) {
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

	if (check && cmpList(&(ao->stamps), anim, compareStamp)) {
		//printf("anim already on list not adding\n");
		return;
	}
	AnimStamp *stamp = calloc(1, sizeof(AnimStamp));
	stamp->anim = anim;
	stamp->poses = calloc(2, sizeof(float));
	stamp->poses[0] = x;
	stamp->poses[1] = y;
	stamp->sprite = sprite;
	stamp->roto = rotation;
	stamp->color = tint;
	addToList(&(ao->stamps), stamp);
	/*
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
	addToList(&(ao->tints), tint);
	*/
	//printf("adding to tints: %i\n", countContents(ao->tints));
}

void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize) {
	//printf("drawing anim orcer\n");
	linkedList *curAnim = ao->anims;//animList;
	linkedList *curPos = ao->poses;//posList;
	linkedList *curSprite = ao->sprites;
	linkedList *curRoto = ao->rotations;
	linkedList *curTint = ao->tints;
	//printf("rotos: %i, tints: %i\n", countContents(curRoto), countContents(curTint));
	/*
	float *xPos;
	float *yPos;
	int *sprite;
	int *roto;
	float *tint;
	*/
	linkedList *curStamp = ao->stamps;
	while (curStamp != NULL) {
		if (curStamp->data != NULL) {
			AnimStamp *stamp = curStamp->data;
			Anim *a = stamp->anim;
			float xPos = stamp->poses[0];
			float yPos = stamp->poses[1];
			int sprite = stamp->sprite;
			if (sprite != -1) {
				changeSprite(a, sprite);
			}
			int roto = stamp->roto;
			if (roto != -1) {
				setRoto(a, roto);
			}
			float *tint = stamp->color;
			float *save = getPalette(a);
			//printf("save:%p\n", save);
			if (tint != 0) {
				for (int i = 0; i < a->texture->numTex; i++) {
					int step = i * 4;
					a->palette[step] = tint[0];
					a->palette[step+1] = tint[1];
					a->palette[step+2] = tint[2];
					a->palette[step+3] = tint[3];
					//printf("%f\n", tint[3]);
				}
			}
			drawSprite(a, sMatrix, xSize, ySize, xPos, yPos);
			if (tint != 0) {
				loadPalette(a, save);
			}
			free(save);
		}
		curStamp = curStamp->next;
	}
}

bool compareStamp(void *a1, void *a2) {
	return ((AnimStamp*)a1)->anim == (Anim*)a2;
}

void freeStamp(void *stamp) {
	AnimStamp *as = stamp;
	free(as->poses);
	if (as->color != 0) {
		free(as->color);
	}
	free(as);
}

void freeAnimOrder(AnimOrder *ao) {
	deleteList(&(ao->stamps), freeStamp);
	freeListSaveObj(&(ao->anims));
	freeList(&(ao->poses));
	freeList(&(ao->sprites));
	freeList(&(ao->rotations));
	freeList(&(ao->tints));
	free(ao);
}


