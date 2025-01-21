#ifndef TEXMAN
#define TEXMAN

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
#endif
