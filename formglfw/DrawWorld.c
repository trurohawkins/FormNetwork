#include "DrawWorld.h"
WorldView *curView = 0;
int tMat, rMat, sMat, drawColor;
GLuint square, line;

void initWorldDrawing() {
	printf("initiating world drawing\n");
	GLuint baseShader = getSP(0);
	if (baseShader != 0) {
		tMat = glGetUniformLocation(baseShader, "tMat");
		if (tMat == -1) {
			printf("vert doesnt have a var tMat\n");
		}
		sMat = glGetUniformLocation(baseShader, "sMat");
		if (sMat == -1) {
			printf("vert doesnt have a var sMat\n");
		}
		rMat = glGetUniformLocation(baseShader, "rMat");
		if (rMat == -1) {
			printf("vert doesnt have a var rMat\n");
		}
		drawColor = glGetUniformLocation(baseShader, "inputColor");
		if (drawColor == -1) {
			printf("frag doesnt have a var drawColor\n");
		}
	}
	curView = getDefaultView();
	square = squareVao2d();
	line = lineVao2d(0);
}


void drawWorld(World *w) {
	Screen *s = getWindow();
	AnimOrder *back = makeAnimOrder(-1);
	AnimOrder *mid = makeAnimOrder(0);
	AnimOrder *front = makeAnimOrder(1);
	float sMatrix[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	GLuint texShader = getSP(1);
	GLuint tileShader = getSP(2);
	linkedList *tileList = makeList();
	glUseProgram(tileShader);
	int tileSeen = 0;
	//printf("\n\n");
	for (int x = 0; x < ceil(curView->frameX) + 0 && x < w->x; x++) {
		for (int y = 0; y < ceil(curView->frameY) && y < w->y; y++) {
			int xp = x + curView->buffX;
			int yp = y + curView->buffY;
			if (xp >= 0 && xp < w->x && yp >= 0 && yp < w->y) {
				Cell *cur = w->map[xp][yp];
				Form** residents = getCellContents(cur);
				if (residents != NULL) {
					for (int i = 0; i < cur->count; i++) {
						//if (residents != NULL) {
						Form *f = residents[i];
						if (f != NULL) {
							float xfp = f->pos[0] - (int)curView->buffX;// - curView->offsetX;//x + frame[0];
							float yfp = f->pos[1] - (int)curView->buffY;// - curView->offsetY;
							float *tile = getStat(f, "tile");
							//printf("- %i -\n", f->id);
							if (tile != NULL) {
								int *ts = (int*)calloc(sizeof(int), 1);
								*ts = (int)(*tile);			
								TileSet *t = getTile(*ts);
								float remainder = *tile - *ts;
								if (t != NULL) {
									if (t->typeID == -1) {
										setTileSetID(t, f->id);
									}
									if (!isInListInt(&tileList, *ts)) {
										tileSeen++;
										addToList(&tileList, ts);
										clearData(t->trans, false);
										//clearData(t->texture, true);
										setRots(t->rot, 0);
										//setData(t->color, 1);
									} else {
										free(ts);
									}
									editData(t->trans, x, y, 1, 1);
								} else {
									free(ts);
								}
								tileCell(t, remainder, xp, yp);
								float *m = getStat(f, "moisture");
								if (m != NULL) {
									float moistMulti = 1 - min(*m, 0.9);// min(1 - ( (f->stat) - 0.1), 1);
									editData(t->color, x, y, moistMulti, 4);
									editData(t->color, x, y, moistMulti, 3);
									editData(t->color, x, y, moistMulti, 2);
									editData(t->color, x, y, 1, 1);
								}
								//printf(" %i ", 1);
								//editTranslations(x, y, 0);
							} else {
								//printf("poo\n");

								// check if weve seen form, not for center, incse form's center isn't on screen
 								if (f->anim != NULL) {// && (xp == (int)(floor(f->pos[0])) && yp == (int)(floor(f->pos[1])))) {
									Anim *a = (Anim*)f->anim;
									if (a->drawOrder > 0) {
										addFormToAnim(front, f, 0, xfp, yfp);
									} else if (a->drawOrder < 0) {
										addFormToAnim(back, f, 0, xfp, yfp);
									} else {
										addFormToAnim(mid, f, 0, xfp, yfp);
									}
								}
							}
						}
					}
				}
				free(residents);
			}
		}
	}
	float mat[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glUseProgram(texShader);
	wvDrawBackground(getDefaultView(), mat);	
	drawBG(sMatrix);
	drawAnimOrder(back, mat, curView->objSX, curView->objSY);
	GLuint squa = squareVao2d();
	if (tileSeen > 0) {
		glUseProgram(tileShader);
		glBindVertexArray(getTileVAO());
		void **tileSets = getContents(&tileList, tileSeen);
		for (int i = 0; i < tileSeen; i++) {
			int cur = *((int*)tileSets[i]);
			TileSet *tmp = getTile(cur);
			setTileVBO(tmp);
			setUpTiles(tmp->set, sMatrix, curView->objSX, curView->objSY);
			bindData(tmp->trans);
			bindData(tmp->rot);
			bindData(tmp->color);
			bindData(tmp->texture);
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (ceil(curView->frameX)+0) * ceil(curView->frameY));
		}
		free(tileSets);
	}
	freeList(&tileList);
	glUseProgram(texShader);
	drawAnimOrder(mid, mat, curView->objSX, curView->objSY); 
	drawAnimOrder(front, mat, curView->objSX, curView->objSY);
	freeAnimOrder(back);
	freeAnimOrder(front);
	freeAnimOrder(mid);
	drawFG(sMatrix);
}

void tileCell(TileSet *t, float remainder, int x, int y) {
	int **d = getDirs();
	DrawScreen *ds = t->texture;
	DrawScreen *rot = t->rot;
	World *w = getWorld();
	Screen *s = getWindow();
	int id = getFormID(x, y);
	if (id == t->typeID) {
		int mostOpen = -1;
		int start = 0;
		int startSide = start;
		for (int i = 0; i < 4; i++) {
			int openSides = 0;
			start = (start + i) % 4;
			for (int j = 0; j < 4; j++) {
				int cur = (start + j) % 4;
				if (getFormID(x + d[cur][0], y + d[cur][1]) != t->typeID) {
					openSides++;
				} else {
					break;
				}
			}
			if (openSides > mostOpen) {
				mostOpen = openSides;
				startSide = start;
			}
		}

		if (mostOpen == 1) {
			int oppoSide = (startSide + 2) % 4;
			if (getFormID(x + d[oppoSide][0], y + d[oppoSide][1]) != t->typeID) {
				mostOpen = 5;
			} else {
				int **d8 = getDir8();
				int nextCorn = ((startSide*2) + 3) % 8;
				int preCorn = ((startSide*2) + 5) % 8;
				float nc = getFormID(x + d8[nextCorn][0], y + d8[nextCorn][1]);
				float pc = getFormID(x + d8[preCorn][0], y + d8[preCorn][1]);
				if (nc != t->typeID && pc != t->typeID) {
					mostOpen = 13;
				} else if (nc != t->typeID){ 
					mostOpen = 11;
				} else if (pc != t->typeID) {
					mostOpen = 12; 
				}
			}
		} else if (mostOpen == 2) {
				int **d8 = getDir8();
				int oppoCorn = ((startSide*2) + 5) % 8;
				if (getFormID(x + d8[oppoCorn][0], y + d8[oppoCorn][1]) != t->typeID) {
					mostOpen = 14;
				}
		} else if (mostOpen == 0) {
			int **d8 = getDir8();
			int check = 0;
			int mostCorn = -1;
			start = 7;
			int s = 7;
			for (int i = 0; i < 4; i++) {
				int corners = 0;
				start = (s + (i * 2)) % 8;
				for (int j = 0; j < 4; j++) {
					int cur = (start + (j * 2)) % 8;
					if (getFormID(x + d8[cur][0], y + d8[cur][1]) != t->typeID) {
						corners++;
					} else {
						break;
					}
				}
				if (corners > mostCorn) {
					check = i;
					mostCorn = corners;
				}
			}
			if (mostCorn != 0) {
				mostOpen = 6 + (mostCorn -1);
				startSide = check;
			}
		}

		float texVal = (t->set->spriteNum-1 - mostOpen) * (t->set->frameY)+1; 
		float texValX = (int)(round(remainder * (t->set->length[0] - 1))) * t->set->frameX;
		editData(ds, x - (int)curView->buffX, y - (int)curView->buffY, texVal, 1);
		editData(ds, x -(int)curView->buffX, y - (int)curView->buffY, texValX, 2);
		setRot(rot, x - (int)curView->buffX, y - (int)curView->buffY, dirToRad(startSide));
	}
}

AnimOrder *makeAnimOrder(int order) {
	AnimOrder *ao = (AnimOrder*)calloc(sizeof(AnimOrder), 1);
	ao->anims = makeList();
	ao->poses = makeList();
	ao->order = order;
}

void addFormToAnim(AnimOrder *ao, Form *f, Anim *anim, float x, float y) {
	if (anim == NULL) {
		anim = (Anim*)f->anim;
	}
	if (cmpList(&(ao->anims), anim, compareAnims)) {
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
}

void drawAnimOrder(AnimOrder *ao, float *sMatrix, float xSize, float ySize) {
	linkedList *curAnim = ao->anims;//animList;
	linkedList *curPos = ao->poses;//posList;
	float *xPos;
	float *yPos;
	while (curAnim != NULL) {
		if (curAnim->data != NULL) {
			xPos = (float*)(curPos->data);
			curPos = curPos->next;
			if (curPos != NULL) {
				yPos = (float*)(curPos->data);
			} else {
				printf("no yPos\n");
			}
			curPos = curPos->next;
			Anim *a = (Anim*)(curAnim->data);
			drawSprite(a, sMatrix, xSize, ySize, *xPos, *yPos);
		}
		curAnim = curAnim->next;
	}
}

void freeAnimOrder(AnimOrder *ao) {
	freeListSaveObj(&(ao->anims));
	freeList(&(ao->poses));
	free(ao);
}

void drawWorldDebug(World *w) {
		float mat[] = {
			1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
		float rMatrix[] = {
			cos(0), -sin(0), 0.0, 0.0,
			sin(0), cos(0), 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	GLuint baseShader = getSP(0);//makeShaderProgram("graphicsSource/matVS.glsl", "graphicsSource/simpFS.glsl");
	glUseProgram(baseShader);
	glUniformMatrix4fv(rMat, 1, GL_TRUE, rMatrix);
	mat[0] = curView->objSX;
	mat[5] = curView->objSY;
	glUniformMatrix4fv(sMat, 1, GL_TRUE, mat);
	glBindVertexArray(square);
	mat[0] = 1;
	mat[5] = 1;
	float startX = -1 - (curView->objSX/2);
	float startY = -1 - (curView->objSY/2);
	for (int x = 0; x < curView->frameX; x++) {
		mat[3] = (startX + curView->objSX) + (x * curView->objSX);
		for (int y = 0; y < curView->frameY; y++) {
			int xp = x + curView->buffX;// frame[0];//(cx-fx);
			int yp = y + curView->buffY;//frame[1];//(cy-fy);
			if (xp >= 0 && xp < w->x && yp >= 0 && yp < w->y) {
				Form *f = checkSolidForm(w->map[xp][yp]);
				if (f != NULL) {
					mat[7] = (startY + curView->objSY) + (y * curView->objSY);	
					glUniformMatrix4fv(tMat, 1, GL_TRUE, mat);
					float *m = getStat(f, "moisture");
					if (m != NULL) {
						float *fCol = (float*)calloc(3, sizeof(float));
						float moistMulti = 1 - min(*m, 0.9);// min(1 - ( (f->stat) - 0.1), 1);
						for (int i = 0; i < 3; i++) {
							fCol[i] = f->color[i] * moistMulti;
						}
						glUniform4f(drawColor, fCol[0], fCol[1], fCol[2], 1.0);
						free(fCol);
					} else {
						glUniform4f(drawColor, f->color[0], f->color[1], f->color[2], 1.0);
					}
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
		}
	}
}

void drawGrid() {
	Screen *s = getWindow();
	glUseProgram(getSP(0));
	float xSize = curView->objSX;//2.0f / curView->frameX;//(float)scr->width / 10000;
	float ySize = curView->objSY;//2.0f / curView->frameY;//(float)scr->height /10000;
		float matrix[] = {
			2.0, 0.0, 0.0, 0.0,
			0.0, 2.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	
	glUniformMatrix4fv(sMat, 1, GL_TRUE, matrix);
	glUniformMatrix4fv(rMat, 1, GL_TRUE, matrix);
	glUniform4f(drawColor, 0, 0, 0, 0.4);
	glBindVertexArray(line);
	for (int x = 0; x < curView->frameX; x++) {
		matrix[3] = -1 + (x * xSize);
		glUniformMatrix4fv(tMat, 1, GL_TRUE, matrix);
		glDrawArrays(GL_LINES, 0, 2);	
	}

	matrix[3] = 0;
	float rMatrix[] = {
		0, -1, 0, 0,
		1, 0, 0, 0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glUniformMatrix4fv(rMat, 1, GL_TRUE, rMatrix);
	for (int y = 0; y < curView->frameY; y++) {
		matrix[7] = -1 + (y * ySize);
		glUniformMatrix4fv(tMat, 1 ,GL_TRUE, matrix);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

float getCenterX() {
	return curView->centerX;
}

float getCenterY() {
	return curView->centerY;
}
