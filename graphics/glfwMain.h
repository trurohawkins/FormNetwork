#ifndef GLFWgra
#define GLFWgra
#include "glad.h"
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
//#include<GL/gl.h>

typedef struct Screen {
	GLFWwindow *window;
	int width;
	int height;
	double xRatio;
	double yRatio;
	int aspectRatioX;
	int aspectRatioY;
	int frame;
	float scale;
} Screen;
#include <stdio.h>

#include "shaders/Shaders.h"
#include "input/Input.h"
#include "text/Text.h"
#include "Anim.h"
#include "Camera.h"
#include "../helper/helper.h"

int initializeGLFW(int windowX, int windowY);
void glfwWindowSizeCallback(GLFWwindow *window, int width, int height);
void sizeScreen(int newFrame);
void setOrtho(float scale);
void setScreenMax(int max);
void setAspectRatio(int x, int y);
GLuint squareVao2d();
GLuint lineVao2d();
GLuint makeVao2d(float *shape, int len);
GLuint makeSpriteVao(float sx, float sy);
Screen *getWindow();
GLuint getSP();
void setCamFunction(void (*newFunc)(void));
#endif
