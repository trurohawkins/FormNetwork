linkedList *BG;
linkedList *FG;
linkedList *pauseScreen;
Menu *curMenu;
float scale;

void drawGround(linkedList *ground) {
	linkedList *cur = ground;
	while (cur != NULL) {
		if (cur->data != NULL) {
			UI *ui = (UI*)cur->data;
			drawUI(ui);
		}
		cur = cur->next;
	}
}

UI *makeUI(char *baseFile, int numColors, int rows, int cols) {
	//printf("uist being made %i\n", numColors);
	bool generated = false;
	char **sprites = (char**)calloc(sizeof(char*), max(1, numColors));
	int sLen = strlen(baseFile);
	if (numColors > 1) {
		generated = true;
		for (int i = 0; i < numColors; i++) {
			sprites[i] = (char*)calloc(sizeof(char), sLen + 2);
			sprites[i][sLen-4] = (unsigned char)i + 48;
			strncpy(sprites[i], baseFile, sLen - 4);
			strcat(sprites[i], ".png");
		}
	} else {
		*sprites = (char*)calloc(sizeof(char), sLen + 1);
		strcpy(*sprites, baseFile); 
	}
	Anim *poo = makeAnim(sprites, numColors, generated, rows, cols);
	/*
	if (numColors != 1) {
		setPalette(pNum, poo);
	}
	*/
	if (numColors > 0) {
		for (int i = 0; i < numColors; i++) {
			free(sprites[i]);
		}
	} else {
		free(*sprites);
	}
	free(sprites);
	GLuint spriteVao = makeSpriteVao(1, 1);
	animAddVao(poo, spriteVao);//makeSpriteVao(1, 1));
	//animGenVao(poo);
	addAnim(poo);
	UI *ui = (UI *)calloc(sizeof(UI), 1);
	ui->xp = 0;//-0.01;
	ui->yp = 0;
	ui->xSize = 1;
	ui->ySize = 1;
	ui->roto = 3;
	ui->xInvert = false;
	ui->yInvert = false;
	ui->a = poo;
	ui->text = 0;
	return ui;
}

void addText(UI *ui, Text *t) {
	ui->text = t;//makeText(str, 1, true);
}

void addButtonText(Button *butt, Text *t) {
	addText(butt->graphics, t);
	butt->textCol1 = (float *)calloc(4, sizeof(float));
	butt->textCol2 = (float *)calloc(4, sizeof(float));
	for (int i = 0; i < 4; i++) {
		(butt->textCol1)[i] = t->color[i];
		(butt->textCol2)[i] = t->color[i];
	}
}

void addButtonSubColor(Button *butt, float r, float g, float b, float a) {
	if (butt->textCol2) {
		butt->textCol2[0] = r;
		butt->textCol2[1] = g;
		butt->textCol2[2] = b;
		butt->textCol2[3] = a;
	}
}

Button *makeButton(char *baseFile, int numColors, int rows, int cols, void(*func)(void)) {
	Button *butt = (Button*)calloc(1,sizeof(Button));
	butt->graphics = makeUI(baseFile, numColors, rows, cols);
	butt->func = func;
	return butt;
}

Menu *makeMenu(float keySpeed) {
	Menu *m = (Menu*)calloc(1, sizeof(Menu));
	m->count = 0;
	m->buttons = makeList();
	m->cursorX = m->cursorY = m->mx = m->my = 0;
	m->keySpeed = keySpeed;
}

void drawUI(UI *ui) {
	float matrix[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	Screen *s = getWindow();
	glUseProgram(getSP(1));
	float xSize = ui->xSize * s->yRatio;// * scale;
	float ySize = ui->ySize * s->xRatio;// * scale;
	drawUIAnim(ui->a, matrix, xSize, ySize, ui->xp, ui->yp);
	if (ui->text) {
		float xp = ((1 + ui->xp + ui->text->xOffset)/2) * s->width ;//(-0.5 * s->width) + (ui->xp  * s->width);
		float yp = ((1 + ui->yp + ui->text->yOffset)/2) * s->height;
		drawText(ui->text, xp, yp);
	}
}

void moveUI(UI *ui, int xd, int yd, float xPow, float yPow) {
	if (ui != NULL) {
		ui->xp += xd * xPow;
		ui->yp += yd * yPow;
	}
}

void placeUI(UI *ui, float xp, float yp) {
	if (ui != NULL) {
		ui->xp = xp;
		ui->yp = yp;
	}
}

void setMenuActive(Menu *m, bool active) {
	void **butts = getContents(&(m->buttons), m->count);
	for (int i = 0; i < m->count; i++) {
		Button *b = (Button*)butts[i];
		if (active) {
			addPauseUI(b->graphics);
		} else {
			removePauseUI(b->graphics);
		}
	}
	Screen *screen = getWindow();
	if (active) {
		curMenu = m;
		glfwPollEvents();
		double xpos, ypos = 0;
		glfwGetCursorPos(screen->window, &xpos, &ypos);
		//call function for initial position
		processMenuMouseMove(screen->window, xpos, ypos);
		glfwSetCursorPosCallback(screen->window, processMenuMouseMove);
	} else {
		curMenu = 0;
		glfwSetCursorPosCallback(screen->window, 0);
	}
}

void processMenuClicks(int button, int action) {
	if (curMenu != NULL) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
			//printf("poopbutthole boyzz\n");
			if (curMenu->curButt) {
				curMenu->curButt->func();
			}
		}
	}
}

void processMenuKeys(int key, int action) {
	if (curMenu) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_W || key == 265) {
				curMenu->my = 1;
			} else if  (key == GLFW_KEY_A || key == 263) {
				curMenu->mx = -1;
			} else if (key == GLFW_KEY_S || key == 264) {
				curMenu->my = -1;
			} else if (key == GLFW_KEY_D || key == 262) {
				curMenu->mx = 1;
			} else if (key == GLFW_KEY_SPACE || key == GLFW_KEY_ENTER) {
				if (curMenu->curButt) {
					curMenu->curButt->func();
				}
			}
		} else if (action == GLFW_RELEASE) {
			if (key == GLFW_KEY_W || key == 265) {
				if (curMenu->my != -1) {
					curMenu->my = 0;
				}
			} else if  (key == GLFW_KEY_A || key == 263) {
				if (curMenu->mx != 1) {
					curMenu->mx = 0;
				}
			} else if (key == GLFW_KEY_S || key == 264) {
				if (curMenu->my != 1) {
					curMenu->my = 0;
				}
			} else if (key == GLFW_KEY_D || key == 262) {
				if (curMenu->mx != -1) {
					curMenu->mx = 0;
				}
			}
		}
	}
}

void processMenuAxes(int axe, float val) {
	if (curMenu) {
		if (axe == 0) {
			//x axis
			curMenu->mx = (int)val;
			//printf("mx: %i\n", curMenu->mx);
		} else if (axe == 1) {
			curMenu->my = (int)val;
		}
	}
}

void processMenuControllerButtons(int butt, int onoff) {
	if (curMenu) {
		if (onoff == 1) {
			if (butt == 11) {
				curMenu->my = 1;
			} else if (butt == 14) {
				curMenu->mx = -1;
			} else if (butt == 13) {
				curMenu->my = -1;
			} else if (butt == 12) {
				curMenu->mx = 1;
			} else if (butt == 0) {
				if (curMenu->curButt) {
					curMenu->curButt->func();
				}
			}
		} else {
			if (butt == 11) {
				if (curMenu->my != -1) {
					curMenu->my = 0;
				}
			} else if (butt == 14) {
				if (curMenu->mx != 1) {
					curMenu->mx = 0;
				}
			} else if (butt == 13) {
				if (curMenu->my != 1) {
					curMenu->my  = 0;
				}
			} else if (butt == 12) {
				if (curMenu->mx != -1) {
					curMenu->mx = 0;
				}
			}
		
		}
	}
}

void processMenuMouseMove(GLFWwindow *window, double xpos, double ypos) {
	Screen *screen = getWindow();
	float xp = -1 + (float)xpos / (screen->width/2);
	float yp = 1 - (float)ypos / (screen->height/2);
	menuMovement(xp, yp);
}

void menuMovement(float xp, float yp) {
	if (curMenu != NULL) {
		Screen *screen = getWindow();
		float closestD = FLT_MAX;//2147483647;
		Button *chosen = 0;
		void **butts = getContents(&(curMenu->buttons), curMenu->count);
		for (int i = 0; i < curMenu->count; i++) {
			Button *b = (Button*)butts[i];
			UI *ui = b->graphics;
			float bx = ui->xp;//(-1 * ui->xSize/2) + ((ui->xp + ui->a->offset[0]) * ui->xSize);
			float by = ui->yp;//(-1 * ui->ySize/2) + ((ui->yp + ui->a->offset[1]) * ui->ySize);
			float dist = distance(bx, by, xp, yp);
			if (dist < closestD) {
				closestD = dist;
				chosen = b;
			}
		}
		if (curMenu->curButt) {
			if (curMenu->curButt->graphics) {
				changeSprite(curMenu->curButt->graphics->a, 0);
				Text *t = curMenu->curButt->graphics->text;
				if (t) {
					setTextColor(t, curMenu->curButt->textCol1);
				}
			}
		}
		curMenu->curButt = chosen;
		if (chosen) {
			if (curMenu->curButt->graphics) {
				changeSprite(curMenu->curButt->graphics->a, 1);
					Text *t = curMenu->curButt->graphics->text;
					if (t) {
						setTextColor(t, curMenu->curButt->textCol2);
					}
			}
		}
	}

}

void updateMenu() {
	if (curMenu) {
		if (curMenu->mx != 0 || curMenu->my != 0) {
			Screen *screen = getWindow();
			float x = (curMenu->mx * curMenu->keySpeed) / screen->width/2;
			float y = (curMenu->my * curMenu->keySpeed) / screen->height/2;
			curMenu->cursorX = clampF(curMenu->cursorX + x, -1, 1);
			curMenu->cursorY = clampF(curMenu->cursorY + y, -1, 1);
			//printf("cursor pos: %f, %f\n", curMenu->cursorX, curMenu->cursorY);
			menuMovement(curMenu->cursorX, curMenu->cursorY);
		}
	}
}

void addButton(Menu *m, Button *butt) {
	addToList(&(m->buttons), butt);
	m->count++;
}
void freeButton(Button *butt) {
	freeUI(butt->graphics);
	free(butt);
}

void freeUI(UI *ui) {
	if (ui->text != 0) {
		freeText(ui->text);
	}
	free(ui);
}

void initUILists() {
	initBackgroundUI();
	initForegroundUI();
	initPauseUI();
}

void freeUILists() {
	freeBG();
	freeFG();
	freePause();
}

void initBackgroundUI(){
	BG = makeList();
}

void addBackground(UI *ui) {
	addToList(&BG, ui);
}

void removeBackground(UI *ui) {
	removeFromList(&BG, (void*)ui);
}

void drawBG() {
	drawGround(BG);
}

void freeBG() {
	freeList(&BG);
}

void initForegroundUI() {
	FG = makeList();
}

void addForeground(UI *ui) {
	addToList(&FG, ui);
}

void removeForeground(UI *ui) {
	removeFromList(&FG, (void*)ui);
}

void drawFG() {
	drawGround(FG);
}

void freeFG() {
	freeList(&FG);
}

void initPauseUI(){
	pauseScreen = makeList();
}

void addPauseUI(UI *ui) {
	addToList(&pauseScreen, ui);
}

void removePauseUI(UI *ui) {
	removeFromList(&pauseScreen, (void*)ui);
}

void drawActiveMenu() {
	drawGround(pauseScreen);
}

void freePause() {
	freeList(&pauseScreen);
}

void tmpButtFunc() {
	Screen *screen = getWindow();
	glfwSetWindowShouldClose(screen->window, 1);
	
	//printf("butt has been pressed\n");
}

void setUIOrtho(float n_scale) {
	scale = n_scale;
}
