#include "../glfw.h"
#include "Camera.h"

Camera* makeCamera() {
	Camera *cam = (Camera*)calloc(sizeof(Camera), 1);
	cam->x = 0;
	cam->y = 0;
	cam->z = 1;
	//setCamera(cam);
	return cam;
}

void setCameraPos(Camera *cam, float x, float y) {
	cam->x = x;
	cam->y = y;
	setCamera(cam);
}

void setCameraSize(Camera *cam, float z) {
	cam->z = z;
	setCamera(cam);
}

void setCamera(Camera  *cam) {
	//printf("setting camera to %f, %f, %f\n", cam->x, cam->y, cam->z);
	float identity[16] = {
	cam->z, 0, 0, 0,
	0, cam->z, 0, 0,
	0, 0, 1, 0,
	cam->x, cam->y, 0, 1
	};
	for (int i = 0; i < 3; i++) {
		GLuint shader = getSP(i);
		glUseProgram(shader);
		GLuint camera = glGetUniformLocation(shader, "Cam");
		glUniformMatrix4fv(camera, 1, GL_FALSE, identity);
		GLuint camPos = glGetUniformLocation(shader, "camPos");
		glUniform3f(camPos, cam->x, cam->y, 0);
	}
}
/*
void initCamera() {
	GLuint tileShader = getSP(1);
	float identity[16] = {
	2, 0, 0, 0.1,
	0, 2, 0, 0,
	0, 0, 2, 0,
	0, 0, 0, 1
	};
	float pos[3] = {10000, 100, 0};
	GLuint camera = glGetUniformLocation(tileShader, "Cam");
	GLuint camPos = glGetUniformLocation(tileShader, "camPos");
	GLuint camSize = glGetUniformLocation(tileShader, "camSize");
	if (camPos < 0) {
		printf("error getting camera variable\n");
	} 
	//glUniform3fv(camPos, 1, pos);
	glUseProgram(tileShader);
	glUniform3f(camPos, 0.1, 0, 0);
	glUniform3f(camSize, 0, 0, 0);
	//glUniformMatrix4fv(camera, 1, GL_TRUE, identity);
}
*/
