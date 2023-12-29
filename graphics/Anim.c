#include "Anim.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
 /*
	GLuint tc, ts: tc = uniform locations for matrices that determine where in the sprite sheet we are drawing from
*/
#include "AnimList.c"
#include "TextureManager.c"
#include "UI.c"
#include "Tile.c"

GLuint tcTrans;
GLuint tcScale;
GLuint tcColor;
GLuint spriteTrans;
GLuint spriteScale;
GLuint spriteRot;

GLuint tcTransTile;
GLuint tcScaleTile;
GLuint tcColorTile;
GLuint spriteTransTile;
GLuint spriteScaleTile;
GLuint spriteRotTile;

Anim *makeAnim(char **sheet, int spriteNum, bool generated, int rows, int cols) { 
	Anim *a = (Anim*)calloc(sizeof(Anim), 1);
	a->drawOrder = 0;
	a->texture = getTexture(sheet, spriteNum, generated);
	float tPropX = 1;
	float tPropY = 1;
	if (a->texture == NULL) {
		printf("texture not made well\n");
		//probably need to free terxture too
		free(a);
		return NULL;
	} else {
		float cellWidth = a->texture->width / cols;
		float cellHeight = a->texture->height / rows;
		if (cellWidth > cellHeight) {
			tPropX = 1;
			tPropY = (float)cellHeight / cellWidth;
		} else {
			tPropX = (float)cellWidth / cellHeight;
		}
	}
	//printf("texture proportion: %f, %f\n", tPropX, tPropY);
	a->frameX = 1.0/cols;
	a->frameY = 1.0/rows;
	a->frame = 0;
	a->sprite = 0;
	a->speed = 6;
	a->spriteNum = rows;
	for (int i = 0; i < a->spriteNum; i++) {
		float cy = ((a->spriteNum-1) - i) * a->frameY + 1;
		//printf("index: %i is value %f\n", i, cy);
	}
	a->length = (int*)calloc(sizeof(int), rows);
	for (int i = 0; i <  rows; i++) {
		a->length[i] = cols;
	}
	a->ratio[0] = tPropX;
	a->ratio[1] = tPropY;
	a->scale[0] = 1;
	a->scale[1] = 1;
	a->flip[0] = 1;
	a->flip[1] = 1;
	a->offset[0] = 0;
	a->offset[1] = 0;
	a->roto = 3;
	a->vao = -1;
	a->loop = true;
	a->reverse = false;
	a->palette = (float*)calloc(sizeof(float), a->texture->numTex * 4);
	for (int i = 0; i < a->texture->numTex * 4; i++) {
		float f = (a->texture->colors)[i];
		(a->palette)[i] = f;
	}
	//a->palette = a->texture->colors;
	return a;
}

char **makeSheet(char *baseFile, int numColors) {
	char **sprites = (char**)calloc(sizeof(char*), max(1, numColors));
	int sLen = strlen(baseFile);
	if (numColors > 1) {
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
	return sprites;
}

Anim *makeAnimSheet(char *baseFile, int numColors, int rows, int cols) {
	char **sheet = makeSheet(baseFile, numColors);
	Anim *a = NULL;
	if (numColors > 1) {
		a = makeAnim(sheet, numColors, true, rows, cols);
	} else {
		a = makeAnim(sheet, numColors, false, rows, cols);
	}
	for (int i = 0; i < numColors; i++) {
		free(sheet[i]);
	}
	free(sheet);
	return a;
}

void addSprite(Anim *a, int index, int len) {
	if (index >= 0 && index < a->spriteNum) {
		a->length[index] = len;
	}
}

void animAddVao(Anim *a, unsigned int vao) {
	a->vao = vao;
}

/*
void animGenVao(Anim *a) {
	if (a->texture == 0) {
		a->vao = makeSpriteVao(1,1);
	} else {
		float tPropX = 1;
		float tPropY = 1;
		if (a->texture->width > a->texture->height) {
			tPropY = (float)a->texture->height / a->texture->width;
		} else {
			tPropX = (float)a->texture->width / a->texture->height;
		}
		a->vao = makeSpriteVao(tPropX, tPropY);
	}
}
*/
void animate(Anim *a) {
	//printf("cur sprite: %i\n", a->sprite);
	if (a->length[a->sprite] > 0) {
		if (a->speedCounter > a->speed) {
			if (!a->reverse) {
				if (a->frame + 1 < a->length[a->sprite]) {
					a->frame++;
				} else {
					if (a->loop) {
						a->frame = 0;
					}
					if (a->animEnd) {
						a->animEnd(a);
					}
				}
			} else {
				if (a->frame - 1 > -1) {
					a->frame--;
				} else {
					if (a->loop) {
						a->frame = a->length[a->sprite] - 1;
					}
					if (a->animEnd) {
						a->animEnd(a);
					}
				}
			}
			a->speedCounter = 0;
		} else {
			a->speedCounter++;
		}
	}
}

void changeSprite(Anim *a, int index) {
	if (index >= 0 && index < a->spriteNum && a->sprite != index) {
		if (a->length[index] != -1) {
			a->sprite = index;
			a->frame = 0;
			a->speedCounter = 0;
		}
	}
}

float getCoordX(Anim *a) {
	//printf("%f\n", a->frame * a->frameX);
	return a->frame * a->frameX;
}

float getCoordY(Anim *a) {
	float cy = ((a->spriteNum-1) - a->sprite) * a->frameY + 1;
	return cy;//((a->sprite) * -a->frameY);
}

void setSpriteTexture(Anim *a) {
	float tMat [] = {
		1.0, 0.0, getCoordX(a),
		0.0, 1.0, getCoordY(a),
		0.0, 0.0, 1.0,
	};
	float sMat [] = {
		a->frameX, 0.0, 0.0,
		0.0, a->frameY, 0.0,
		0.0, 0.0, 1.0,
	};
	/*
	glUniformMatrix3fv(a->texCoords, 1, GL_TRUE, tMat);
	glUniformMatrix3fv(a->texScale, 1, GL_TRUE, sMat);
	*/
	glUniformMatrix3fv(tcTrans, 1, GL_TRUE, tMat);
	glUniformMatrix3fv(tcScale, 1, GL_TRUE, sMat);
}

void drawSprite(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp) {
	sMatrix[3] = (-1 + xSize/2) + ((xp + a->offset[0]) * xSize);// + -a->flip[0] * 0.01f;
	sMatrix[7] = (-1 + ySize/2) + ((yp + a->offset[1]) * ySize);// + 0.01f;	
	sMatrix[0] = 1;//xSize * a->scale[0] * a->flip[0];
	sMatrix[5] = 1;//ySize * a->scale[1] * a->flip[1];
	glUniformMatrix4fv(spriteTrans, 1, GL_TRUE, sMatrix);
	drawAnim(a, sMatrix, xSize, ySize);
}

void setSpriteTransMat(float *matrix) {
	glUniformMatrix4fv(spriteTrans, 1, GL_TRUE, matrix);
}

void drawAnim(Anim *a, float *matrix, float xSize, float ySize) {
	matrix[3] = 0;
	matrix[7] = 0;
	matrix[0] = xSize * a->ratio[0] * a->scale[0] * convertInvert(a->invert[0]);//a->flip[0];
	matrix[5] = ySize * a->ratio[1] * a->scale[1] * convertInvert(a->invert[1]);//a->flip[1];
	glUniformMatrix4fv(spriteScale, 1, GL_TRUE, matrix);
	setSpriteTexture(a);
	float rad = rotoToRadian(a->roto);
	float rMatrix[] = {
		cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0 ,0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glUniformMatrix4fv(spriteRot, 1, GL_TRUE, rMatrix);
	glUniform2f(tcTrans, getCoordX(a), getCoordY(a));
	glBindVertexArray(a->vao);
	textureSource *ts = a->texture;
	for (int i = 0; i < ts->numTex; i++) {
		int step = i * 4;
		glUniform4f(tcColor,(a->palette)[step],(a->palette)[step+1], (a->palette)[step+2], (a->palette)[step+3]);
		glBindTexture(GL_TEXTURE_2D, (a->texture->tex)[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

void drawUIAnim(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp) {
	sMatrix[3] = xp;//(-1 + xSize/2) + ((xp + a->offset[0]) * xSize);// + -a->flip[0] * 0.01f;
	sMatrix[7] = yp;//(-1 + ySize/2) + ((yp + a->offset[1]) * ySize);// + 0.01f;	
	sMatrix[0] = 1;//xSize * a->scale[0] * a->flip[0];
	sMatrix[5] = 1;//ySize * a->scale[1] * a->flip[1];
	glUniformMatrix4fv(spriteTrans, 1, GL_TRUE, sMatrix);
	drawAnim(a, sMatrix, xSize, ySize);
}

void setUpTiles(Anim *a, float *sMatrix, double xSize, double ySize) {
	float mat[] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	glUniformMatrix4fv(spriteRotTile, 1, GL_TRUE, mat);
	glUniformMatrix4fv(spriteTransTile, 1, GL_TRUE, mat);
	mat[3] = 0;
	mat[7] = 0;
	// for some reason gets rid of tile distortion lines when resizing screen
	xSize += 0.00001;
	ySize += 0.00001;
	mat[0] = xSize * a->ratio[0] * a->scale[0] * convertInvert(a->invert[0]);//a->flip[0];
	mat[5] = ySize * a->ratio[1] * a->scale[1] * convertInvert(a->invert[1]);//a->flip[1];
	glUniformMatrix4fv(spriteScaleTile, 1, GL_TRUE, mat);
	float tMat [] = {
		1.0, 0.0, getCoordX(a),
		0.0, 1.0, getCoordY(a),
		0.0, 0.0, 1.0,
	};
	float sMat [] = {
		a->frameX, 0.0, 0.0,
		0.0, a->frameY, 0.0,
		0.0, 0.0, 1.0,
	};
	glUniformMatrix3fv(tcTransTile, 1, GL_TRUE, tMat);
	glUniformMatrix3fv(tcScaleTile, 1, GL_TRUE, sMat);
	//setSpriteTexture(a);
	//float rad = rotoToRadian(a->roto);
	/*
	float rMatrix[] = {
		cos(rad), -sin(rad), 0.0, 0.0,
		sin(rad), cos(rad), 0.0, 0.0,
		0.0, 0.0, 1.0 ,0.0,
		0.0, 0.0, 0.0, 1.0
	};
	*/
	//glUniform2f(tcTrans, getCoordX(a), getCoordY(a));
	//glBindVertexArray(a->vao);
	textureSource *ts = a->texture;
	int step = 0 * 4;
	glUniform4f(tcColorTile,(a->palette)[step],(a->palette)[step+1], (a->palette)[step+2], (a->palette)[step+3]);
	glBindTexture(GL_TEXTURE_2D, (ts->tex)[0]);
}

void useTile(Anim *a) {
	glBindVertexArray(a->vao);
	textureSource *ts = a->texture;
	int step = 0 * 4;
	glUniform4f(tcColor,(a->palette)[step],(a->palette)[step+1], (a->palette)[step+2], (a->palette)[step+3]);
	glBindTexture(GL_TEXTURE_2D, (ts->tex)[0]);

}

void drawTiles(Anim *a, float *sMatrix, float xSize, float ySize, float xp, float yp) {
	sMatrix[3] = (-1 + xSize/2) + ((xp + a->offset[0]) * xSize);// + -a->flip[0] * 0.01f;
	sMatrix[7] = (-1 + ySize/2) + ((yp + a->offset[1]) * ySize);// + 0.01f;	
	//printf("drawing x&y: %f, %f\n", sMatrix[3], sMatrix[7]);
	sMatrix[0] = 1;//xSize * a->scale[0] * a->flip[0];
	sMatrix[5] = 1;//ySize * a->scale[1] * a->flip[1];
	glUniformMatrix4fv(spriteTransTile, 1, GL_TRUE, sMatrix);
	textureSource *ts = a->texture;
	//printf("%u\n", (a->texture->tex)[0]);
	//printf("%i\n", ts->numTex);
	//for (int i = 0; i < ts->numTex; i++) {
		//int step = i * 4;
		//glUniform4f(tcColor,(a->palette)[step],(a->palette)[step+1], (a->palette)[step+2], (a->palette)[step+3]);
		//glBindTexture(GL_TEXTURE_2D, (ts->tex)[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//}
}

void loadPalette(Anim *a, float *palette) {
	for (int i = 0; i < a->texture->numTex; i++) {
		int step = i * 4;
		a->palette[step] = palette[step];
		a->palette[step+1] = palette[step+1];
		a->palette[step+2] = palette[step+2];
		a->palette[step+3] = palette[step+3];
	}
}

void setScale(Anim *a, float x, float y) {
	a->scale[0] = x;
	a->scale[1] = y;
}

void setOffset(Anim *a, float x, float y) {
	a->offset[0] = x;
	a->offset[1] = y;
}

void setFlipX(Anim *a, int x) {
	a->flip[0] = x;
}

void setFlipY(Anim *a, int y) {
	a->flip[1] = y;
}

void setRoto(Anim *a, int degree) {
	a->roto = degree;
}

void setInvert(Anim *a, int axis, bool flipped) {
	a->invert[axis] = flipped;
}


void setDrawOrder(Anim *a, int o) {
	a->drawOrder = o;
}


void getUniformValue(GLuint texShader, char *name, GLuint *dest) {
	GLuint t = glGetUniformLocation(texShader, name);
	if (t == -1) {
		printf("shader doesnt have a var %s\n", name);
	} else {
		*dest = t;
		//printf("got value %u for %s\n", t, name);
	}
}

void setTexTrans(GLuint tt) {
	if (tt == -1) {
		printf("frag doesnt have a var tcTrans\n");
	} else {
		tcTrans = tt;
	}
}

void setTexScale(GLuint ts) {
	if (ts == -1) {
		printf("frag doesnt have a var tcScale\n");
	} else {
		tcScale = ts;
	}
}

void setTexColor(GLuint tc) {
	if (tc == -1) {
		printf("frag doesnt have a var colorShift\n");
	} else {
		tcColor = tc;
	}
}

void initTexInts() {
	GLuint texShader = getSP(1);
	getUniformValue(texShader, "tcScale", &tcScale);
	getUniformValue(texShader, "tcTrans", &tcTrans);
	getUniformValue(texShader, "colorShift", &tcColor);
	getUniformValue(texShader, "tMat", &spriteTrans);
	getUniformValue(texShader, "sMat", &spriteScale);
	getUniformValue(texShader, "rMat", &spriteRot);

	glUseProgram(texShader);
	float tscMat [] = {
		//1.0f/6, 0.0, 0.0,
		//0.0, 1.0f/4, 0.0,
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0
	};
	float ttcMat [] = {
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,//-1.0f/4,
		0.0, 0.0, 1.0,
	};
	glUniformMatrix3fv(tcScale, 1, GL_TRUE, tscMat);
	glUniformMatrix3fv(tcTrans, 1, GL_TRUE, ttcMat);
	glUniform3f(tcColor, 255, 255, 255);
	GLuint tileShader = getSP(2);
	getUniformValue(tileShader, "tcScale", &tcScaleTile);
	getUniformValue(tileShader, "tcTrans", &tcTransTile);
	getUniformValue(tileShader, "colorShift", &tcColorTile);
	getUniformValue(tileShader, "tMat", &spriteTransTile);
	getUniformValue(tileShader, "sMat", &spriteScaleTile);
	getUniformValue(tileShader, "rMat", &spriteRotTile);
}

float rotoToRadian(int d) {
	if (d == 0) {
		return 1.5708;
	} else if (d == 2) {
		return 4.71239;
	}
	return 0;
}

void freeAnim(Anim *a) {
	free(a->palette);
	free(a->length);
	free(a);
}

bool compareAnims(void *a1, void *a2) {
	return (Anim*)a1 == (Anim*)a2;
}

