#include "TextureManager.h"
linkedList *textureManager;

void makeTextureManager() {
	textureManager = makeList();
}

void addTexture(textureSource *ts) {
	addToList(&textureManager, ts);
}

void deleteTextureManager() {
	linkedList *cur = textureManager;
	while (cur != NULL) {
		linkedList *next = cur->next;
		if (cur->data != NULL) {
			freeTextureSource((textureSource*)(cur->data));
		}
		free(cur);
		cur = next;
	}
}

unsigned int genTexture(unsigned char *data, int wid, int hei) {
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//printf("generating texture of size %ix%i @%p\n", wid, hei, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, wid, hei, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	return tex;
}

textureSource *findTexture(char *name) {
	linkedList *cur = textureManager;
	while (cur != NULL) {
		textureSource *tmp = (textureSource*)cur->data;
		if (tmp != NULL) {
			//printf("checking cur: %s
			if (strcmp(name, tmp->name) == 0) {
				return tmp;
			}
		}
		cur = cur->next;
	}
	return NULL;
}
// if num is zero textue will be processed as individual image which will be split into color layers
// white gen stops texture from flipping the sprites to use them
//   this is because when we generate sprites we flip them 
//		so that the color order doesn't change between usages
textureSource *getTexture(char **name, int num, bool whiteGen) {
	textureSource *ts = findTexture(*name);//textures always saved as first file
	if (ts == NULL) {
		//printf("making new texture %i\n", whiteGen);
		ts = makeTextureFromImages(name, num, whiteGen);
	} else {
		//printf("no need to make new texture\n");
	}
	return ts;
}

textureSource *makeTextureFromImages(char **imgs, int num, bool whiteGen) {
	//printf("texture from images %i\n", num);
	if (num == 1) {
		return makeTexture(*imgs, true);
	} else if (num == 0) {
		return makeTexture(*imgs, false);
	}
	textureSource *ts = (textureSource*)calloc(sizeof(textureSource), 1);
	//printf("was it white generated? %i\n", whiteGen);
	if (!whiteGen) {
		stbi_set_flip_vertically_on_load(true);
	} else {
		stbi_set_flip_vertically_on_load(false);
	}
	ts->name = (char *)calloc(sizeof(char), strlen(imgs[0])+1);
	strcpy(ts->name, imgs[0]);
	ts->numTex = num;
	ts->tex = (unsigned int*)calloc(sizeof(unsigned int), num);
	ts->colors = (float*)calloc(sizeof(float), num * 4);
	for (int i = 0; i < num; i++) {
		for (int j =0; j < 4; j++) {
			(ts->colors)[i*4 + j] = 1;
		}
		unsigned char *data = stbi_load(imgs[i], &(ts->width), &(ts->height), &(ts->channels), 0);
		if (data) {
			unsigned int tex = genTexture(data, ts->width, ts->height);
			(ts->tex)[i] = tex;
			stbi_image_free(data);
		} else {
			printf("%s does not exist, texture creation failed\n", imgs[i]);
			freeTextureSource(ts);
			stbi_image_free(data);
			return 0;
		}
	}
	addTexture(ts);
	return ts;
}


//will make a singlesprite to a single texture
// if single is false, will dynamically create white layered sprite w/color pallete
textureSource *makeTexture(char *img, bool single) {
	textureSource *ts = (textureSource*)calloc(sizeof(textureSource), 1);
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(img, &(ts->width), &(ts->height), &(ts->channels), 0);
	//printf("texture-%s has width of %d\n", img, ts->width);
	if (data) {
		/*
		unsigned int tex = genTexture(data, ts->width, ts->height);
		ts->tex = (unsigned int*)calloc(sizeof(unsigned int), 1);
		*(ts->tex) = tex;
		ts->numTex = 1;
		*/
		ts->name = (char *)calloc(sizeof(char), strlen(img)+1);
		strcpy(ts->name, img);
		if (!single) {
			printf("making layer texture\n");
			makeLayerTexture(ts, data, 0);
			//separateImgByColor(ts, data, 0, false);
		} else {
			//printf("single sprite being made to texture\n");
			unsigned int tex = genTexture(data, ts->width, ts->height);
			ts->tex = (unsigned int*)calloc(sizeof(unsigned int), 1);
			*(ts->tex) = tex;
			ts->numTex = 1;
			ts->colors = (float*)calloc(sizeof(float), 4);
			for (int i = 0; i < 4; i++) {
				(ts->colors)[i] = 1;
			}
		}
		addTexture(ts);
		//printf("number of textures %i\n", ts->numTex);
	} else {
		free(ts);
		printf("failed to load texture from file %s\n", img);
		ts = 0;
	}
	stbi_image_free(data);
	return ts;
}

textureSource *makeWhiteLayerTextureFiles(char *img) {
	textureSource *ts = (textureSource*)calloc(sizeof(textureSource), 1);
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(img, &(ts->width), &(ts->height), &(ts->channels), 0);
	if (data) {
		ts->name = (char *)calloc(sizeof(char), strlen(img)+1);
		strcpy(ts->name, img);
		printf("making layer texture files\n");
		makeLayerTexture(ts, data, 0);
		writeLayerTextureToFile(ts, data, "0");
		// could probably be done without adding texture file
		addTexture(ts);
		//printf("number of textures %i\n", ts->numTex);
	} else {
		free(ts);
		printf("failed to load texture from file %s\n", img);
		ts = 0;
	}
	stbi_image_free(data);
	return ts;
}

int countColors(textureSource *ts, unsigned char *data) {
	linkedList *colors = makeList();
	int numColors = 0;
	for (int i = 0; i < ts->height * ts->width * ts->channels; i+=ts->channels) {
		if (data[i+3] != 0) {
			unsigned char *color = (unsigned char*)calloc(sizeof(unsigned char),4);
			color[0] = data[i];
			color[1] = data[i+1];
			color[2] = data[i+2];
			color[3] = data[i+3];
			if (cmpList(&colors, color, compareColor) == true) {
				free(color);
			} else {
				addToList(&colors, color);
				numColors++;
			}
		}
	}
	freeList(&colors);
	return numColors;
}

void makeLayerTexture(textureSource *ts, unsigned char *data, int numColors) {
	colorLayerInfo *layers = separateImgByColor(ts, data, numColors);
	genLayerTexture(ts, layers);
	freeColorLayerInfo(layers);
}

void writeLayerTextureToFile(textureSource *ts, unsigned char *data, char *paletteName) {
	colorLayerInfo *layers = separateImgByColor(ts, data, 0);
	writeTextureToFile(ts, layers);
	//writePalette(ts, paletteName);
	freeColorLayerInfo(layers);
}

/* 
 * takes in PNG and converts each color to a separate file and colors them white
 * 	allows for dynamic paletting of sprites
 * ts - textureSource tp be filled in
 * data - raw data of png file
 * numColors - if you know how many colors there are you can save some time by including it
 *	otherwise <=0 will count the colors in the file for you
 */
colorLayerInfo *separateImgByColor(textureSource *ts, unsigned char *data, int numColors) {
	if (numColors <= 0) {
		numColors = countColors(ts, data);
	}
	ts->numTex = numColors;
	linkedList *colors = makeList();
	int dataLength = ts->height * ts->width * ts->channels;
	colorLayerInfo *layers = (colorLayerInfo*)calloc(sizeof(colorLayerInfo), 1);
	layers->num = numColors;
	layers->layers	= (colorLayer*)calloc(sizeof(colorLayer), numColors);
	for (int i = 0; i < numColors; i++) {
		(layers->layers)[i].data = (unsigned char*)calloc(sizeof(unsigned char), dataLength);
	}
	ts->colors = (float*)calloc(sizeof(float), numColors * 4);
	int colorSeen = 0;
	unsigned char *color;
	for (int i = 0; i < dataLength; i+=ts->channels) {
		if (data[i+3] != 0) {
			color = (unsigned char*)calloc(sizeof(unsigned char),4);
			color[0] = data[i];
			color[1] = data[i+1];
			color[2] = data[i+2];
			color[3] = data[i+3];
			if (cmpList(&colors, color, compareColor) == false) {
				//we havent seen this color
				//process it by assigning it an empty color layer
				addToList(&colors, color);
				for (int j = 0; j < 4; j++) {
					//int step = (numColors * 3) - ((colorSeen+1) * 3);
					int step = colorSeen * 4;
					//layers[numColors-1-colorSeen].color[j] = color[j];
					(layers->layers)[colorSeen].color[j] = color[j];
					(ts->colors)[step + j] = (float)color[j]/255;
				}
				//set any color on layer to pure white and solid, so palette can be applied purely
				for (int j = 0; j < 4; j++) {
					(layers->layers)[colorSeen].data[i+j] = 255;
				}
				colorSeen++;
			} else {
				for (int j = 0; j < numColors; j++) {
					if (compareColor((layers->layers[j]).color, color)) {
						//printf("found color-%u,%u,%u at %i\n",color[0], color[1], color[2], j);
						for (int k = 0; k < 4; k++) {
							(layers->layers)[j].data[i+k] = 255;
						}
						break;
					}
				}
				free(color);
			}
		}
	}
	freeList(&colors);
	/*
	*/
	return layers;
}

void genLayerTexture(textureSource *ts, colorLayerInfo *layers) {
	ts->tex = (unsigned int*)calloc(sizeof(unsigned int), layers->num);
	for (int i = 0; i < layers->num; i++) {
		unsigned int tex = genTexture((layers->layers)[i].data, ts->width, ts->height);
		(ts->tex)[i] = tex;
	}
}


void freeTextureSource(textureSource* ts) {
	free(ts->name);
	free(ts->tex);
	free(ts->colors);
	free(ts);
}

void freeColorLayerInfo(colorLayerInfo *layers)  {
	for (int i = 0; i < layers->num; i++) {
		free((layers->layers)[i].data);
		//free((layers->layers[i]));
	}
	free(layers->layers);
	free(layers);
}

void writeTextureToFile(textureSource *ts, colorLayerInfo *layers) {
	int sLen = strlen(ts->name);
	char *fName = (char*)calloc(sizeof(char), sLen + 2);
	strncpy(fName, ts->name, sLen-4);
	for (int i = 0; i < ts->numTex; i++) {
		char *cur = (char*)calloc(sizeof(char), sLen + 2);
		strcpy(cur, fName);
		cur[sLen - 4] = (unsigned char)i + 48;
		strcat(cur, ".png");
		//printf("creating file %s\n", cur);
		stbi_write_png(cur, ts->width, ts->height, ts->channels, (layers->layers)[i].data, ts->width * ts->channels);	
		free(cur);
	}
	free(fName);
}

void writePalette(textureSource *ts, char *name) {
	int sLen = strlen(name);
	char *fName = (char*)calloc(sizeof(char), sLen + 13);
	strncpy(fName, name, sLen);
	//printf("file name = %s\n");
	strcat(fName, "Palette0.c");
	//printf("creating palette %s at %s\n", name, fName);
	FILE* fptr = fopen(fName, "w");
	char *varName = (char*)calloc(sizeof(char), 16 + strlen(name));
	strcpy(varName, "float ");
	strcat(varName, name);
	strcat(varName, "Palette0");
	strcat(varName, "[");
	fprintf(fptr, varName);
	fprintf(fptr, "%i", ts->numTex*4);
	fprintf(fptr, "] = {");

	for (int i = 0; i < ts->numTex * 4; i++) {
		fprintf(fptr, "%f", ts->colors[i]);
		if (i != (ts->numTex * 4) -1) {
			fprintf(fptr, ", ");
		}
	}
	fprintf(fptr, "};\n");
	fclose(fptr);
}
