#include "GlutProcess.h"

int FPS = 60;
void (*Update)(int);

void glutFunctions(void (*drawFunc)(void), void (*updateFunc)(int),
void (*keyD)(unsigned char, int, int), void (*keyU)(unsigned char, int, int)) {
	glutIgnoreKeyRepeat(1);	
	glutDisplayFunc(drawFunc);	
	glutKeyboardFunc(keyD);
	glutKeyboardUpFunc(keyU);
	glutJoystickFunc(joystick, 25);
	Update = updateFunc;
	glutTimerFunc(1000/FPS, updateFunc, 0);
	glutMainLoop();
}

void joystick(unsigned int buttonmask, int x, int y, int z) {
	//printf("getting joystick %i, %i, %i axis values\n", x, y, z);
	//printf("buttonmask %u\n", buttonmask);
}
/*
void keyDown(unsigned char key, int mx, int my) {
	if (key == 27) {
		glutLeaveMainLoop();
	} else if (key == 96) {
		//paused = !paused;
	}
	//keyPressPlayer(poopGuy, key);
}

void keyUp(unsigned char key, int mx, int my) {
	//keyReleasePlayer(poopGuy, key);
}
*/
void updateGlut() {
	glutPostRedisplay();
	glutTimerFunc(1000/FPS, Update, 0);
}
