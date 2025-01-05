#include "FormGraphics.h"
//#include "Form.h"
//#include "../graphicsSource/Graphics.h"

//#include<stdio.h>

void drawForm(Form *f, int x, int y, int size) {
	if (f != 0) {
		//	printf("%i, %i is %f \n", x, y, f->color[0]);
			drawSquare(f->color, x, y, size);
	}
}
bool grid = false;
float centerX;
float centerY;
int initialTime;
int frameCount = 0;
bool paused;

void drawWorld() {
	glClearColor(0.1, 0.2, 0.75, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	World *w = getWorld();
	//get dimensions of the frame
	int fx = getFX()/2;
	int fy = getFY()/2;
	int cx = clamp(centerX, fx, w->x - fx - 1);
	int cy = clamp(centerY, fy, w->y - fy - 1);
	for (int i=cx-fx; i <= cx+fx ; i ++) {
		for (int j=cy-fy; j <= cy+fy; j++){
			if (i >= 0 && i < w->x && j >= 0 && j < w->y) {
				drawForm(w->map[i][j], i-(cx-fx), j-(cy-fy), 1);
			}
		}
	}
	if (grid) {
		for (int i=cx-fx; i <= cx+fx ; i ++) {
			for (int j=cy-fy; j <= cy+fy; j++){
				drawLine(0,0,0,0, j-(cy-fy), fx*2, j-(cy-fy));
			}
			drawLine(0,0,0,i-(cx-fx), 0, i-(cx-fx), fy*2);

		}
	}
	frameCount++;
	int curTime = time(NULL);
	if (curTime - initialTime >= 1) {
		int fps = frameCount / (curTime - initialTime);
		//printf("FPS: %i\n", fps);
		initialTime = curTime;
		frameCount = 0;
	}
	glutSwapBuffers();
}

void setCenter(float cp[2]) {
	centerX = cp[0];
	centerY = cp[1];
}

void setInitTime(int newTime) {
	initialTime = newTime;
}

void setGrid(bool on) {
	grid = on;
}

void keyDown(unsigned char key, int mx, int my) {
	if (key == 27) {
		glutLeaveMainLoop();
	} else if (key == 96) {
		paused = !paused;
	}
	keyPressPlayer(getPlayer(), key);
}

void keyUp(unsigned char key, int mx, int my) {
	keyReleasePlayer(getPlayer(), key);
}

void update(int value) {
	if (!paused) {
		actorListDo();
	}
	setCenter(getPlayer()->me->body->pos);
	updateGlut();
}


/*
void drawFunc() {
	printf("caca\n");
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	Form *f = makeForm(1, 1, 1);
	drawForm(f, 0, 0, 1);	
	free(f);
	
	glutSwapBuffers();
}

*/
