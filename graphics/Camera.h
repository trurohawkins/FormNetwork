#ifndef CAMERA
#define CAMERA
#include "glfwMain.h"

typedef struct Camera {
	float x;
	float y;
	float z;
} Camera;

//void initCamera();
Camera* makeCamera();
void setCameraPos(Camera *cam, float x, float y);
void setCameraSize(Camera *cam, float z);
void setCamera(Camera *cam);
#endif
