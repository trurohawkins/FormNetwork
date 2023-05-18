#ifndef ANIM
#define ANIM
#include "glfwMain.h"
//#include <stdio.h>
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <GL/gl.h>
#include <stdbool.h>
#include "../helper/helper.h"
#include "TextureManager.h"

typedef struct Anim {
	int drawOrder;
	int speed;
	int speedCounter;
	float frameX;
	float frameY;
	int frame;
	int sprite;
	int spriteNum;
	int *length;
	float scale[2];
	float ratio[2];
	int flip[2];
	float offset[2];
	int roto;
	bool invert[2];
	bool loop;
	bool reverse;
	textureSource *texture;
	float *palette;
	unsigned int vao;
	void *data;
	void (*animEnd)(struct Anim*);
} Anim;

#include "AnimList.h"
#include "Tile.h"
#include "UI.h"

Anim *makeAnim(char **sheet, int spriteNum, bool generated, int rows, int col);
char **makeSheet(char *baseFile, int numColors);
Anim *makeAnimSheet(char *baseFile, int numColors, int rows, int col);
unsigned int makeSpriteTexture(char *sheet, int rows, int col);
void getUniformValue(GLuint texShader, char *name, GLuint *dest);
void setTexTrans(GLuint tt);
void setTexScale(GLuint ts);
void setTexColor(GLuint tc);
void initTexInts();
float rotoToRadian(int d);
void freeAnim(Anim *a);
void setScale (Anim* a, float x, float y);
void setOffset (Anim* a, float x, float y);
void setFlipX(Anim *a, int x);
void setFlipY(Anim *a, int y);
void setRotation(Anim *a, int d);
void setRoto(Anim *a, int degree);
void setInvert(Anim *a, int axis, bool flipped);
void setDrawOrder(Anim *a, int o);
void addSprite(Anim *a, int index, int len);
void animAddVao(Anim *a, GLuint vao);
//void animGenVao(Anim *a);
void animate(Anim *a);
void changeSprite(Anim *a, int index);
float getCoordX(Anim *a);
float getCoordY(Anim *a);
void setSpriteTexture(Anim *a);
void setSpriteTransMat(float *matrix);
void drawSprite(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp);
void drawAnim(Anim *a, float *sMatrix, float xSize, float ySize);
void drawUIAnim(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp);
void setUpTiles(Anim *a, float *sMatrix, double xSize, double ySize);
void drawTiles(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp);
void loadPalette(Anim *a, float *palette);
bool compareAnims(void *a1, void *a2);
#endif
