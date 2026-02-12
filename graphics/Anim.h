#ifndef ANIM
#define ANIM
#include <stdbool.h>
typedef struct textureSource {
	char *name;
	unsigned int *tex;
	float *colors;
	int numTex;
	int width;
	int height;
	int channels;
} textureSource;

typedef struct colorLayer {
	unsigned char color[4];
	unsigned char* data;
} colorLayer;

typedef struct colorLayerInfo {
	int num;
	colorLayer *layers;
} colorLayerInfo;

void makeTextureManager();
void addTexture(textureSource *ts);
void deleteTextureManager();
unsigned int genTexture(unsigned char *data, int wid, int hei);
textureSource *makeTexture(char *img, bool single);
textureSource *makeTextureFromImages(char **imgs, int num, bool whiteGen);
textureSource *makeWhiteLayerTextureFiles(char *img);
void makeLayerTexture(textureSource *ts, unsigned char *data, int numColors);
void writeLayerTextureToFile(textureSource *ts, unsigned char *data, char *paletteName);
void genLayerTexture(textureSource *ts, colorLayerInfo *layers);
int countColors(textureSource *ts, unsigned char* data);
colorLayerInfo *separateImgByColor(textureSource *ts, unsigned char *data, int numColors);
textureSource *findTexture(char *img);
textureSource *getTexture(char **name, int num, bool whiteGen);
void freeTextureSource(textureSource *ts);
void freeColorLayerInfo(colorLayerInfo *layers);
void writeTextureToFile(textureSource *ts, colorLayerInfo *layers);
void writePalette(textureSource *ts, char *name);

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

Anim *makeAnim(char **sheet, int spriteNum, bool generated, int rows, int col);
char **makeSheet(char *baseFile, int numColors);
Anim *makeAnimSheet(char *baseFile, int numColors, int rows, int col);
unsigned int makeSpriteTexture(char *sheet, int rows, int col);

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
//GLuint
void animAddVao(Anim *a, unsigned int vao);
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
float *getPalette(Anim *a);
bool compareAnims(void *a1, void *a2);
#endif
