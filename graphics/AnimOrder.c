AnimOrder *back;
AnimOrder *mid;
AnimOrder *front;
SortedList *backLayers;
SortedList *layers;

AnimOrder *makeAnimOrder(int order) {
	AnimOrder *ao = (AnimOrder*)calloc(sizeof(AnimOrder), 1);
	ao->stamps = makeList();
	ao->order = order;
	return ao;
}

AnimStamp *makeAnimStamp(Anim *anim) {
	AnimStamp *stamp = calloc(1, sizeof(AnimStamp));
	stamp->anim = anim;
	for (int i = 0; i < 4; i++) {
		stamp->color[i] = 1;
		if (i < 2) {
			stamp->size[i] = 1;
		}
	}
	return stamp;
}

void addAnimToOrder(int drawOrder, Anim *anim, float x, float y, float *size, int rotation,int sprite,  float *tint, bool check) {
	/*
	if (anim == NULL) {
		anim = (Anim*)f->anim;
	}
	*/
	AnimOrder *ao ;
	if (drawOrder < 0) {
		ao = searchSlist(backLayers, drawOrder);
		if (!ao) {
			ao = makeAnimOrder(drawOrder);
			sortedAdd(&backLayers, ao, drawOrder);
		}
	} else {
		ao = searchSlist(layers, drawOrder);
		if (!ao) {
			ao = makeAnimOrder(drawOrder);
			sortedAdd(&layers, ao, drawOrder);
		}
	}

	if (check && cmpList(&(ao->stamps), anim, compareStamp)) {
		//printf("anim already on list not adding\n");
		return;
	}
	AnimStamp *stamp = makeAnimStamp(anim);
	stamp->poses[0] = x;
	stamp->poses[1] = y;
	stamp->roto = rotation;
	if (size != 0) {
		memcpy(stamp->size, size, sizeof(float) * 2);
		//printf("%f, %f\n", stamp->size[0], stamp->size[1]);
	}
	stamp->sprite = sprite;
	if (tint != 0) {
		memcpy(stamp->color, tint, sizeof(float) * 4);
	}
	addToList(&(ao->stamps), stamp);
}

void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize) {
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
					a->palette[step] *= tint[0];
					a->palette[step+1] *= tint[1];
					a->palette[step+2] *= tint[2];
					a->palette[step+3] *= tint[3];
					//printf("%f\n", tint[3]);
				}
			}
			sMatrix[0] = stamp->size[0];
			sMatrix[5] = stamp->size[1];
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
	free(as);
}

void freeAnimOrder(AnimOrder *ao) {
	deleteList(&(ao->stamps), freeStamp);
	free(ao);
}


