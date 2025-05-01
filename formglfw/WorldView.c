#include "WorldView.h"
WorldView *defaultView;
linkedList *follow;

WorldView *makeWorldView(int scale) {
	WorldView *wv = (WorldView *)calloc(sizeof(WorldView), 1);
	wv->centerX = 0;
	wv->centerY = 0;
	wv->frameX = 0;
	wv->frameY = 0;
	wv->scalePower = scale;
	wv->zoomSpeed = 0.2;
	wv->moveSpeed = 18;
	/*
	wv->defaultFrameX = 0;
	wv->defaultFrameY = 0;
	*/
	wv->cam = makeCamera();
	return wv;
}

void initWorldView(int x, int y) {
	defaultView = makeWorldView(100);
	/*
	defaultView->defaultFrameX = defaultView->frameX = x;	
	defaultView->defaultFrameY = defaultView->frameY = y;
	*/
	defaultView->frameDest = defaultView->frame = (int)(sqrt(x * y));
	setFrame(defaultView, x);
}

WorldView *getDefaultView() {
	return defaultView;
}
//applying new screen ratio to frame data
void resizeScreen() {
	if (defaultView) {
		setFrame(defaultView, defaultView->frame);
	}
}

void setFrameMin(WorldView *wv, float min) {
	wv->frameMin = min;
}

void setFrame(WorldView *wv, float frame) {
	Screen *s = getWindow();
	World *w = getWorld();
	wv->frame = frame;

	float objSizes[2] = {wv->objSX, wv->objSY};
	float frames[2] = {wv->frameX, wv->frameY};

	float frameX = ceil(frame  * s->xRatio) - 1; 
	int centerBuffX = 0;
	if ((int)frameX % 2 == 0) {
		centerBuffX = 0.5 * wv->scalePower;//(frameX - floor(frameX)) / 2; 
	}	
	float fx = (frameX)/2;
	int cx = clampF(wv->centerX, (floor(fx)*wv->scalePower) - centerBuffX , (w->x * wv->scalePower) - ((ceil(fx)*wv->scalePower) + centerBuffX));
	float xRem = intToFrac(cx + centerBuffX, wv->scalePower);// - ((cx / 10) * 10);
	if (frameX >= w->x) {
		wv->frameX = w->x;
		wv->objSX = (2.0 * ((float)w->x/frameX)) / wv->frameX;//(float)scr->width / 10000;
		wv->cam->x = 1 - ((float)w->x / (frameX)); 
		wv->buffX = 0;
	} else {
		wv->frameX = ceil(frameX) + 1;//(centerBuffX * 2);
		if (centerBuffX == 0) {
			wv->buffX = ((float)cx/wv->scalePower) - floor(fx);//max(-1, cx-fx);//clamp(cx-fx, 0, w->x - 1 - frameX);
		} else {
			wv->buffX = max(0, round(((float)cx/wv->scalePower) - fx));
		}
		wv->objSX = 2.0 / frameX;//(s->xRatio * (double)frame);//(s->xRatio * frame);//wv->frameX);//(float)scr->height /10000;
		wv->cam->x = -xRem * wv->objSX;
	}
	
	float frameY = ceil(frame  * s->yRatio) - 1;
	int centerBuffY = 0;
	if ((int)frameY % 2 == 0) {
		centerBuffY = 0.5 * wv->scalePower;
	}	
	float fy = (frameY)/2;
	int cy =  clampF(wv->centerY, (floor(fy)*wv->scalePower) - centerBuffY, (w->y * wv->scalePower) - ((ceil(fy)*wv->scalePower) + centerBuffY));// + centerBuffY);
	float yRem =  intToFrac(cy + centerBuffY, wv->scalePower);
	if (frameY >= w->y) {
		wv->frameY = w->y;
		wv->objSY = (2.0 * ((float)w->y/frameY)) / wv->frameY;//(float)scr->height /10000;
		wv->cam->y = 1 - ((float)w->y / frameY);
		wv->buffY = 0;
	} else {
		wv->frameY = ceil(frameY) + 1;//(centerBuffY * 2);
		//wv->buffY = cy-floor(fy);
		if (centerBuffY == 0) {
			wv->buffY = ((float)cy/wv->scalePower) - floor(fy);//may(-1, cy-fy);//clamp(cy-fy, 0, w->y - 1 - frameY);
		} else {
			wv->buffY = max(0, round(((float)cy/wv->scalePower) - fy));
		}
		wv->objSY = 2.0 / frameY;//((double)frame * s->yRatio);//(s->yRatio * frame);//(wv->frameY);//(float)scr->height /10000;
		wv->cam->y = -yRem * wv->objSY;
	}
	//printf("drawing world with dimensions %f, %f at %i, %i\n", frameX, frameY, cx, cy);
	//printf("world buffer %i, %i --- trans: %f, %f\n", wv->buffX, wv->buffY, xRem, yRem);
	float zRem = (frame - floor(frame)) * wv->objSX;

	//printf("%f with remainder %f\n", frame, zRem);
	wv->cam->z = 1;//0.5;//1 + zRem;//zRem * -0.01f;//wv->objSX;

	setCamera(wv->cam);
	//printf("worldbuffer %i, camera trans %f\n", wv->buffX, -xRem);
	//printf("about to resize tiles, the offset is %i and bool value is %i\n",wv->offsetX, wv->offsetX != 0);
	if (!equal(wv->objSX,objSizes[0]) || !equal(wv->objSY,objSizes[1])) {
		printf("obj size %f, %f -> %f, %f\n", objSizes[0], objSizes[1], wv->objSX, wv->objSY);
		printf("frame size %f, %f -> %f. %f\n", frames[0], frames[1], wv->frameX, wv->frameY);
		printf("resize\n");
		for (int i = 0; i < getTileCount(); i++) {
			TileSet *ts = getTile(i);
			setTileSize(ts, wv->objSX, wv->objSY);
			resizeTileSet(ts, wv->frameX, wv->frameY);//wv->frameX != w->x, wv->frameY != w->y);
		}
	}
}

void setCenter(WorldView *wv, float xp, float yp) {
	wv->centerX = xp * wv->scalePower;
	wv->centerY = yp * wv->scalePower;
}

void centerCamera(WorldView *wv, float x, float y) {
	Screen *s = getWindow();
	World *w = getWorld();
	float frameSizeX = 2.0f / wv->frameX;
	float frameSizeY = 2.0f / wv->frameY;
	float buffer = 0;
	float cx = clampF(x, buffer, w->x - buffer);
	float cy = clampF(y, buffer, w->y - buffer);
	float fx = -((-1 + frameSizeX/2) + (cx * frameSizeX));
	//float fx = -((-1 + frameSizeX/2) + (f->pos[0] * frameSizeX));
	float fy = -((-1 + frameSizeY/2) + (cy * frameSizeY));
	//printf("camera set %f, %f from %f, %f\n", fx, fy, x ,y);
	//mulltiply x and y coordinates so that the scaling is applied to the movement
	setCameraPos(wv->cam, fx * wv->cam->z, fy * wv->cam->z);
}

void wvMakeBackground(WorldView *wv, char *image) {
	/*
	char **sprites = (char**)calloc(sizeof(char*), 1);
	*sprites = (char*)calloc(sizeof(char), strlen(image) + 1);
	strcpy(*sprites, image);
	*/
	World *w = getWorld();
	char **sprites = makeSheet(image, 1);
	Anim *bg = makeAnim(sprites, 1, false, 1, 1);//makeAnimSheet(image, 1, 1, 1);
	free(*sprites);
	free(sprites);
	setScale(bg, w->x, w->y);//scaleX, scaleY);
	animAddVao(bg, makeSpriteVao(1, 1));
	wv->background = bg;
}

Anim *wvChangeBackground(WorldView *wv, Anim *bg) {
	World *w = getWorld();
	Anim *tmp = wv->background;
	setScale(bg, w->x, w->y);//scaleX, scaleY);
	wv->background = bg;
	return tmp;
}

void wvDrawBackground(WorldView *wv, float *matrix) {
	if (wv->background) {
		World *w = getWorld();
		// maybe dont need to set these
		matrix[0] = 1;
		matrix[5] = 1;
		drawSprite(wv->background, matrix, wv->objSX, wv->objSY, (float)w->x/2 -(int)wv->buffX - 0.5f, (float)w->y/2 - (int)wv->buffY - 0.5f);
	}
}

void followForm(Form *f) {
	if (!follow) {
		follow = makeList();
	}
	addToList(&follow, f);
}

void unFollowForm(Form *f) {
	if (!follow) {
		printf("we are not following anything\n");
		return;
	}
	if (f != 0) {
		removeFromList(&follow, f);
	}
}

void followForms(WorldView *wv) {
	if (follow) {
		linkedList *head = follow;
		int count = 0;
		float xp = 0;
		float yp = 0;
		float maxDistance = -1; 
		while (head) {
			Form *f = (Form*)head->data;
			if (f) {
				xp += f->pos[0] + f->pMod[0];
				yp += f->pos[1] + f->pMod[1];
				count++;
				linkedList *sub = follow;
				while (sub) {
					Form *f2 = (Form*)sub->data;
					if (f2) {
						float dist = distance(f->pos[0], f->pos[1], f2->pos[0], f2->pos[1]);
						if (dist > maxDistance) {
							maxDistance = dist;
						}
					}
					sub = sub->next;
				}
			}
			head = head->next;
		}
		xp /= count;
		yp /= count;
		World *w = getWorld();
		// we clamp the distance between our frameMin and the world Max, we dont know which is bigger, so we use min and max to figure that out inline
		int worldMax = max(w->x, w->y) + 1;
		maxDistance = clamp(maxDistance + 10, min(worldMax,wv->frameMin), max(worldMax, wv->frameMin));
		if (xp * wv->scalePower != wv->centerX || yp * wv->scalePower != wv->centerY) {
			setCenter(wv, xp, yp);
		}
		//if (maxDistance != wv->frame ) {
			setFrame(wv, maxDistance);
		//}
	}
}

void lerpView(WorldView *wv) {
	if (wv->frame == wv->frameDest && wv->centerX == wv->cenDestX && wv->centerY == wv->cenDestY) {// && wv->frameY == wv->frameDestY) {
		return;
	}
	printf("lerping from %f,%f to %f, %f\n", wv->centerX, wv->centerY, wv->cenDestX, wv->cenDestY);
	if (wv->frame != wv->frameDest) {
		int dir = sign(wv->frameDest - wv->frame);
		if (dir > 0) {
			if (wv->frame + wv->zoomSpeed  < wv->frameDest) {
				wv->frame += wv->zoomSpeed;
			} else {
				wv->frame = wv->frameDest;
			}
		} else {
			if (wv->frame - wv->zoomSpeed > wv->frameDest) {
				wv->frame -= wv->zoomSpeed;
			} else { 
				wv->frame = wv->frameDest;
			}
		}
	}

	if (wv->centerX != wv->cenDestX) {
		int dir = sign(wv->cenDestX - wv->centerX);
		//printf("X dir is %i and speed is %f\n", dir);
		if (dir > 0) {
			if (wv->centerX + wv->moveSpeed  < wv->cenDestX) {
				wv->centerX += wv->moveSpeed;
			} else {
				wv->centerX = wv->cenDestX;
			}
		} else {
			if (wv->centerX - wv->moveSpeed > wv->cenDestX) {
				wv->centerX -= wv->moveSpeed;
			} else { 
				wv->centerX = wv->cenDestX;
			}
		}
	}
	if (wv->centerY != wv->cenDestY) {
		int dir = sign(wv->cenDestY - wv->centerY);
		if (dir > 0) {
			if (wv->centerY + wv->moveSpeed  < wv->cenDestY) {
				wv->centerY += wv->moveSpeed;
			} else {
				wv->centerY = wv->cenDestY;
			}
		} else {
			if (wv->centerY - wv->moveSpeed > wv->cenDestY) {
				wv->centerY -= wv->moveSpeed;
			} else { 
				wv->centerY = wv->cenDestY;
			}
		}
	}

	setFrame(wv, wv->frame);
	/*
	if (wv->frameY != wv->frameDestY) {
		int dir = sign(wv->frameDestY - wv->frameY);
		if (dir > 0) {
			if (wv->frameY + wv->zoomSpeed  < wv->frameDestY) {
				wv->frameY += wv->zoomSpeed;
			} else {
				wv->frameY = wv->frameDestY;
			}
		} else {
			if (wv->frameY - wv->zoomSpeed > wv->frameDestY) {
				wv->frameY -= wv->ZoomSpeed;
			} else { 
				wv->frameY = wv->frameDestY;
			}
		}
	}
	*/
}
