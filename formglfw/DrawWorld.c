#include "DrawWorld.h"
WorldView *curView = 0;
int tMat, rMat, sMat, drawColor;
GLuint square, line;

void initWorldDrawing() {
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
	/*
	back = makeAnimOrder(-1);
	mid = makeAnimOrder(0);
	front = makeAnimOrder(1);
	*/
	float sMatrix[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	GLuint texShader = getSP(1);
	GLuint tileShader = getSP(2);
	//linkedList *tileList = makeList();
	SortedList *tileList = 0;
	glUseProgram(tileShader);
	int tileSeen = 0;
	//printf("draw %i, %i\n", curView->buffX, curView->buffY);
	for (int x = 0; x < ceil(curView->frameX) + 0 && x < w->x; x++) {
		for (int y = 0; y < ceil(curView->frameY) && y < w->y; y++) {
			int xp = x + curView->buffX;
			int yp = y + curView->buffY;
			if (xp >= 0 && xp < w->x && yp >= 0 && yp < w->y) {
				Cell *cur = w->map[xp][yp];
				// seems more efficient to just loop through linked list, rather than copy into array
				//Form** residents = getCellContents(cur);
				linkedList *residents = cur->within;
				while (residents) {
					//for (int i = 0; i < cur->count; i++) {
					if (residents->data) {
						//if (residents != NULL) {
						Form *f = residents->data;//[i];
						if (f != NULL) {
							float *tile = getStat(f, "tile");
							//printf("- %i -\n", f->id);
							if (tile != NULL) {
								//int *ts = (int*)calloc(sizeof(int), 1);
								//*ts = (int)(*tile);			
								TileSet *t = getTile(*tile);
								if (t != NULL) {
									if (t->typeID == -1) {
										setTileSetID(t, f->id);
									}
									if (!isInSlist(tileList, t)) {
										tileSeen++;
										sortedAdd(&tileList, t, t->renderOrder);
										clearData(t->trans, false);
										//clearData(t->texture, true);
										setRots(t->rot, 0);
										//setData(t->color, 1);
									} else {
										//free(ts);
									}
									//printf("tile %i, %i\n", x, y);
									if (t->tileSprites) {
										//tileCell(t, remainder, xp, yp);
										t->tileSprites(t, f, xp, yp);
									}
								} else {
									//free(ts);
								}
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
								if (f->render != 0) {
									f->render(f, xp, yp, (int)curView->buffX, (int)curView->buffY);
								} else {
									drawForm(f, (int)curView->buffX, (int)curView->buffY); 
								}
							}
						}
					}
					residents = residents->next;
				}
				//free(residents);
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
	//drawBG(sMatrix); //idk what this is from and how it changed
	drawBG();
	SortedList *cur = backLayers;
	while (cur) {
		drawAnimOrder(cur->data, mat, curView->objSX, curView->objSY);
		cur = cur->next;
	}
	GLuint squa = squareVao2d();
	if (tileSeen > 0) {
		glUseProgram(tileShader);
		glBindVertexArray(getTileVAO());
		SortedList *cur = tileList;
		while (cur) {
			drawTileSet(cur->data, curView->objSX, curView->objSY, curView->frameX, curView->frameY);
			cur = cur->next;
		}
		//free(tileSets);
	}
	freeSlist(tileList);
	glUseProgram(texShader);
	//drawAnimOrder(mid, mat, curView->objSX, curView->objSY); 
	//drawAnimOrder(front, mat, curView->objSX, curView->objSY);
	cur = layers;
	while (cur) {
		drawAnimOrder(cur->data, mat, curView->objSX, curView->objSY);
		cur = cur->next;
	}
	cur = backLayers;
	while (cur) {
		freeAnimOrder(cur->data);
		cur = cur->next;
	}
	cur = layers;
	while (cur) {
		freeAnimOrder(cur->data);
		cur = cur->next;
	}
	freeSlist(backLayers);
	backLayers = 0;
	freeSlist(layers);
	layers = 0;
	//drawFG(sMatrix); // idk why I passed this at any point
	drawFG();
}

void drawForm(Form *f, int buffX, int buffY) {
	float xfp = (f->pos[0] + f->pMod[0]) - buffX;// - curView->offsetX;//x + frame[0];
	float yfp = (f->pos[1] + f->pMod[1]) - buffY;// - curView->offsetY;

	// check if weve seen form, not for center, incse form's center isn't on screen
 	if (f->anim != NULL) {// && (xp == (int)(floor(f->pos[0])) && yp == (int)(floor(f->pos[1])))) {
		for (int j = 0; j < f->aCount; j++) {
			Anim *a = ((Anim**)f->anim)[j];
			addAnimToOrder(a->drawOrder, a, xfp, yfp, 0, -1, -1, 0, true);
		}
	}
}

void tileCell(TileSet *t, void *f, int x, int y) {
	float *tile = getStat(f, "tile");
	printf("tiling sprite %f, %i\n", *tile, t->typeID);
	float remainder = *tile - *tile;
	editData(t->trans, x, y, 1, 1);
	int **d = getDirs();
	DrawScreen *ds = t->texture;
	DrawScreen *rot = t->rot;
	World *w = getWorld();
	Screen *s = getWindow();
	int *ids = getFormID(x, y);
	int id = -1;
	int c = 0;
	while (ids[c] != -1) {
		if (ids[c] == t->typeID) {
			id = ids[c];
			break;
		} else {
			c++;
		}
	}
	free(ids);
	if (id == t->typeID) {
		int mostOpen = -1;
		int start = 0;
		int startSide = start;
		for (int i = 0; i < 4; i++) {
			int openSides = 0;
			start = (start + i) % 4;
			for (int j = 0; j < 4; j++) {
				int cur = (start + j) % 4;
				//if (getFormID(x + d[cur][0], y + d[cur][1]) != t->typeID) {
				if (!checkFormID(x + d[cur][0], y + d[cur][1], t->typeID)) {
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
			//if (getFormID(x + d[oppoSide][0], y + d[oppoSide][1]) != t->typeID) {
			if (!checkFormID(x + d[oppoSide][0], y + d[oppoSide][1], t->typeID)) {
				mostOpen = 5;
			} else {
				int **d8 = getDirs8();
				int nextCorn = ((startSide*2) + 3) % 8;
				int preCorn = ((startSide*2) + 5) % 8;
				//float nc = getFormID(x + d8[nextCorn][0], y + d8[nextCorn][1]);
				bool nc = checkFormID(x + d8[nextCorn][0], y + d8[nextCorn][1], t->typeID);
				//float pc = getFormID(x + d8[preCorn][0], y + d8[preCorn][1]);
				bool pc = checkFormID(x + d8[preCorn][0], y + d8[preCorn][1], t->typeID);
				//if (nc != t->typeID && pc != t->typeID) {
				if (!nc && !pc) {
					mostOpen = 13;
				//} else if (nc != t->typeID){ 
				} else if (!nc){ 
					mostOpen = 11;
				//} else if (pc != t->typeID) {
				} else if (!pc) {
					mostOpen = 12; 
				}
			}
		} else if (mostOpen == 2) {
				int **d8 = getDirs8();
				int oppoCorn = ((startSide*2) + 5) % 8;
				//if (getFormID(x + d8[oppoCorn][0], y + d8[oppoCorn][1]) != t->typeID) {
				if (!checkFormID(x + d8[oppoCorn][0], y + d8[oppoCorn][1], t->typeID)) {
					mostOpen = 14;
				}
		} else if (mostOpen == 0) {
			int **d8 = getDirs8();
			int check = 0;
			int mostCorn = -1;
			start = 7;
			int s = 7;
			for (int i = 0; i < 4; i++) {
				int corners = 0;
				start = (s + (i * 2)) % 8;
				for (int j = 0; j < 4; j++) {
					int cur = (start + (j * 2)) % 8;
					//if (getFormID(x + d8[cur][0], y + d8[cur][1]) != t->typeID) {
					if (!checkFormID(x + d8[cur][0], y + d8[cur][1], t->typeID)) {
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
		printf("tex val %f, %f\n", texVal, texValX);
		editData(ds, x - (int)curView->buffX, y - (int)curView->buffY, texVal, 1);
		editData(ds, x -(int)curView->buffX, y - (int)curView->buffY, texValX, 2);
		setRot(rot, x - (int)curView->buffX, y - (int)curView->buffY, dirToRad(startSide));
	}
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


	glUniformMatrix4fv(rMat, 1, GL_TRUE, mat);//rMatrix);
	mat[0] = w->x;//1;//curView->objSX;
	mat[5] = w->y;
	glUniformMatrix4fv(sMat, 1, GL_TRUE, mat);
	mat[0] = 1;
	mat[5] = 1;
	mat[3] = 0;//(float)w->x/2 - (int)curView->buffX - 0.5f;//-1;
	mat[7] = 0;//-1;
	glUniformMatrix4fv(tMat, 1, GL_TRUE, mat);
	glUniform4f(drawColor, 0, 0, 0, 1.0);
	glBindVertexArray(square);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	
	mat[3] = 0;
	mat[7] = 0;
	
	//set rotation
	mat[0] = curView->objSX;
	mat[5] = curView->objSY;
	glUniformMatrix4fv(sMat, 1, GL_TRUE, mat);
	mat[0] = 1;
	mat[5] = 1;
	float startX = -1 - (curView->objSX/2);
	float startY = -1 - (curView->objSY/2);
	//printf("%f + %f\n", startX, (startX + curView->objSX) + ((curView->frameX/2) * curView->objSX));
	for (int x = 0; x < curView->frameX; x++) {
		mat[3] = (startX + curView->objSX) + (x * curView->objSX);
		for (int y = 0; y < curView->frameY; y++) {
			int xp = x + curView->buffX;// frame[0];//(cx-fx);
			int yp = y + curView->buffY;//frame[1];//(cy-fy);
			if (xp >= 0 && xp < w->x && yp >= 0 && yp < w->y) {
				//Form *f = checkSolidForm(w->map[xp][yp]);
				//Form *f = 0;//checkSolidForm(w->map[xp][yp]);
				linkedList *forms = w->map[xp][yp]->within;//checkSolidForm(w->map[xp][yp]);
				/*
				Form **forms = w->map[xp][yp]->content;
				for (int i = 0; i < maxCellCount; i++) {
					if (forms[i] != 0) {
						f = forms[i];
						break;
					}
				}
				*/
				while (forms) {
					if (forms->data) {
						Form *f = forms->data;
						if (f != NULL) {
							mat[7] = (startY + curView->objSY) + (y * curView->objSY);	
							glUniformMatrix4fv(tMat, 1, GL_TRUE, mat);
							float *m = getStat(f, "moisture");
							if (m != NULL) {
								float fCol[] = {0,0,0};//(float*)calloc(3, sizeof(float));
								float moistMulti = 1 - min(*m, 0.9);// min(1 - ( (f->stat) - 0.1), 1);
								for (int i = 0; i < 3; i++) {
									fCol[i] = f->color[i] * moistMulti;
								}
								glUniform4f(drawColor, fCol[0], fCol[1], fCol[2], 1.0);
							} else {
								glUniform4f(drawColor, f->color[0], f->color[1], f->color[2], 1.0);
							}
							glDrawArrays(GL_TRIANGLES, 0, 6);
						}
					}
					forms = forms->next;
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
