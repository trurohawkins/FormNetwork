#include "glfwMain.h"
#include "shaders/glslLib.c"

const unsigned int screenWidth = 800*8/5, screenHeight = 450*8/5;
Screen *curScreen;
GLuint baseShaderProgram;
GLuint textureShaderProgram;
GLuint tileShaderProgram;
GLuint textShaderProgram;
void (*camFunc)(void) = 0;
GLFWmonitor *cur;

int initializeGLFW(int windowX, int windowY) {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
	//use OpenGL 3.3 set minimum so we dont use depreceated functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// look into the specifics of these functions
	//got them from depreceated tutroial(had to update function calls and arg names

	//window = glfwCreateWindow(screenWidth, screenHeight, "poop", glfwGetPrimaryMonitor(), NULL); full screen
	//window = glfwCreateWindow(screenWidth, screenHeight, "poop", NULL, NULL);
//	GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "poop", NULL, NULL);
	GLFWmonitor *mon = glfwGetPrimaryMonitor();
	if (windowX == 0 || windowY == 0) {
		const GLFWvidmode *vid = glfwGetVideoMode(mon);
		printf("monitor is of size %i x %i\n", vid->width, vid->height);
		windowX = vid->width;
		windowY = vid->height;
	} else {
		mon = NULL;
	}
	GLFWwindow *window = glfwCreateWindow(windowX, windowY, "poop", mon, NULL);

	if (window == NULL) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	} else {
		glfwMakeContextCurrent(window);
		curScreen = (Screen*)calloc(sizeof(Screen), 1);
		curScreen->window = window;
		curScreen->aspectRatioX = windowX;
		curScreen->aspectRatioY = windowY;
		curScreen->scale = 1;
		glfwGetWindowSize(curScreen->window, &(curScreen->width), &(curScreen->height));
		/*
		curScreen->xRatio = 1;
		curScreen->yRatio = 1;
		*/
		//glfwWindowSizeCallback(window, screenWidth, screenHeight);
		glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
	}
	gladLoadGL();
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	printf("OpenGL - %i.%i.%i\n", major, minor, rev);
	//tell GL to only draw onto a pixel if shape is cloer to the viewer
	// I turned it off because the sprites are all the same distance from camera
	//glEnable(GL_DEPTH_TEST);//enable depther testing
	//glDepthFunc(GL_LESS); //depth testing interpets a smaller value as "closer"
	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	baseShaderProgram = makeShaderProgram(matVS, matFS);
	//textureShaderProgram = makeShaderProgramFile("graphics/shaders/texVS.glsl", "graphics/shaders/texFS.glsl");
	textureShaderProgram = makeShaderProgram(singleTextureVS, singleTextureFS);
	tileShaderProgram = makeShaderProgram(textureVS, textureFS);
	//textShaderProgram = makeShaderProgramFile("graphics/shaders/textVS.glsl", "graphics/shaders/textFS.glsl");
	textShaderProgram = makeShaderProgram(textVS, textFS);
	//initCamera();
}


GLuint squareVao2d() {
	float square[12] = {
		-0.5, 0.5, 
		0.5, 0.5, 
		0.5, -0.5, 
		-0.5, 0.5, 
		0.5, -0.5, 
		-0.5, -0.5, 
	};
	return makeVao2d(square, 12);
}

GLuint lineVao2d(int lineType) {
	float line[4];
	int val = 1;
	for (int i = 0; i < 4; i++) {
		if (i % 2 == lineType) {
			line[i] = 0;
		} else {
			line[i] = val;
			val *= -1;
		}
	}
	return makeVao2d(line, 4);
}

GLuint makeVao2d(float *shape, int len) {
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * len, shape, GL_STATIC_DRAW);
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);//points = 0 on vao
	glEnableVertexAttribArray(0);
	return vao;	
}

GLuint makeSpriteVao(float sx, float sy) {
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f,  sx, 1,
		0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, sx, 1-sy,
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 0, 1-sy,
		-0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 0, 1
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	GLuint vbo, vao, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//points/pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	return vao;
}

void glfwWindowSizeCallback(GLFWwindow *window, int width, int height) {
	float sx = 0, sy = 0;
	if (curScreen->aspectRatioX != 0 && curScreen->aspectRatioY != 0) {
		float w = width;
		float h = height;
		//printf("%i x %i ", width, height);
		if (width > height) {
			while (w > 0) {
				h = (w * curScreen->aspectRatioY) / curScreen->aspectRatioX;
				if (h <= height) {
					break;
				} else {
					w--;
				}
			}
			//h = (w * curScreen->aspectRatioY) / curScreen->aspectRatioX;
		} else {
			while (h > 0) {
				w = (h * curScreen->aspectRatioX) / curScreen->aspectRatioY;
				if (w <= width) {
					break;
				} else {
					h--;
				}
			}

		}
		//printf("ratio %i:%i creates %f x %f\n",curScreen->aspectRatioX, curScreen->aspectRatioY, w, h);
		sx = (width - w) / 2;
		sy = (height - h) / 2;
		width = w;
		height = h;
	}
	//curScreen->scale used to maintain proportions when shifting back and fourth between sizes
	curScreen->scale = sqrt(width * height) / sqrt(curScreen->width * curScreen->height) * curScreen->scale;
	curScreen->width = width;
	curScreen->height = height;
	glViewport(sx, sy, curScreen->width, curScreen->height);
	sizeScreen(curScreen->frame);
	setTextOrtho(curScreen);
	//setUIOrtho(scaleUI);
	//setOrtho(scale);//for text & UI rendering
}

void sizeScreen(int frame) {
	curScreen->frame = frame;
	double xRatio = 1;
	double yRatio = 1;
	double xRatioInv = 1;
	double yRatioInv = 1;
	if (curScreen->width > curScreen->height) {
		xRatio = (double)curScreen->width / curScreen->height;
		yRatio = 1;
		xRatioInv = (double)curScreen->height / curScreen->width;
		yRatioInv = 1;
	} else {
		xRatio = 1;
		yRatio = (double)curScreen->height / curScreen->width;
		xRatioInv = 1;
		yRatioInv = (double)curScreen->width / curScreen->height;
	}
	curScreen->xRatio = xRatio;
	curScreen->yRatio = yRatio;
	if (camFunc != 0) {
		camFunc();
	}
}

void setScreenMax(int max) {
	//curScreen->frameMax = max;
}


Screen *getWindow() {
	return curScreen;
}

GLuint getSP(int shader) {
	if (shader == 0) {
		return baseShaderProgram;
	} else if (shader == 1) {
		return textureShaderProgram;
	} else if (shader == 2) {
		return tileShaderProgram;
	} else {
		return textShaderProgram;
	}
}

void setCamFunction(void (*newFunc)(void)){
	camFunc = newFunc;
	camFunc();
}

void setAspectRatio(int x, int y) {
	if (curScreen) {
		curScreen->aspectRatioX = x;
		curScreen->aspectRatioY = y;
	}
}
