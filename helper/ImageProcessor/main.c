#include <unistd.h>
#include <fcntl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../graphics/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../graphics/stb_image_write.h"

#include "../helper.c"
#include "../../graphics/glfwMain.c"
#include "../../graphics/TextureManager.c"

//void separateImgByColor(unsigned char *data, textureSource *ts, char *paletteName);
//void writePalette(textureSource *ts, char *name);

int main(int argc, char **argv) {
	if (argc > 1) {
		textureSource *ts = (textureSource*)calloc(sizeof(textureSource), 1);
		ts->name = (char *)calloc(sizeof(char), strlen(argv[1])+1);
		strcpy(ts->name, argv[1]);
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(argv[1], &(ts->width), &(ts->height), &(ts->channels), 0);
		if (data) {
		if (argc > 2) {
				writeLayerTextureToFile(ts, data,  argv[2]);
			} else {
				writeLayerTextureToFile(ts, data, 0);
				printf("no var name given, palette not created\n");
			}
		} else {
			printf("file %s not found\n", argv[1]);
		}
	} else {
		printf("no file name given\n");
	}
}
/*
void separateImgByColor(unsigned char *data, textureSource *ts, char *paletteName) {
	linkedList *colors = makeList();
	int numColors = 0;
	for (int i = 0; i < ts->height * ts->width * ts->channels; i+=ts->channels) {
		if (data[i+3] != 0) {
			unsigned char *color = (unsigned char*)calloc(sizeof(unsigned char),3);
			color[0] = data[i];
			color[1] = data[i+1];
			color[2] = data[i+2];
			if (cmpList(&colors, color, compareColor) == true) {
				free(color);
			} else {
				addToList(&colors, color);
				numColors++;
			}
		}
	}
	freeList(&colors);
	colors = makeList();
	int dataLength = ts->height * ts->width * ts->channels;
	colorLayer *layers = (colorLayer*)calloc(sizeof(colorLayer), numColors);
	for (int i = 0; i < numColors; i++) {
		layers[i].data = (unsigned char*)calloc(sizeof(unsigned char), dataLength);
	}
	ts->colors = (float*)calloc(sizeof(float), numColors * 3);
	int colorSeen = 0;
	unsigned char *color;
	//float *basePalette =
	for (int i = 0; i < dataLength; i+=ts->channels) {
		if (data[i+3] != 0) {
			color = (unsigned char*)calloc(sizeof(unsigned char),3);
			color[0] = data[i];
			color[1] = data[i+1];
			color[2] = data[i+2];
			if (cmpList(&colors, color, compareColor) == false) {
				//we havent seen this color
				//process it by assigning it an empty color layer
				addToList(&colors, color);
				for (int j = 0; j < 3; j++) {
					int step = colorSeen * 3;
					layers[colorSeen].color[j] = color[j];
					(ts->colors)[step + j] = (float)color[j]/255;
				}
				//set any color on layer to pure white full opacity
				for (int j = 0; j < 4; j++) {
					layers[colorSeen].data[i+j] = 255;
				}
				colorSeen++;
			} else {
				for (int j = 0; j < numColors; j++) {
					if (compareColor(layers[j].color, color)) {
						//printf("found color-%u,%u,%u at %i\n",color[0], color[1], color[2], j);
						for (int k = 0; k < 4; k++) {
							layers[j].data[i+k] = 255;
						}
						break;
					}
				}
				free(color);
			}
		}
	}
	*/
	/*
	for (int i = 0; i < numColors*3; i++) {
		printf(" %i ",(ts->colors)[i]); 
	}
	printf("\n");
	ts->tex = (unsigned int*)calloc(sizeof(unsigned int), numColors);
	for (int i = 0; i < numColors; i++) {
		unsigned int tex = genTexture(layers[i].data, ts->width, ts->height);
		(ts->tex)[i] = tex;
	}
	*/
	/*
	ts->numTex = numColors;
	writeTextureToFile(ts, layers);
	if (paletteName != 0) {
		writePalette(ts, paletteName);
	}
	freeList(&colors);
	for (int i = 0; i < numColors; i++) {
		free(layers[i].data);
	}
	free(layers);

}

void writeTextureToFile(textureSource *ts, colorLayer *layers) {
	int sLen = strlen(ts->name);
	char *fName = (char*)calloc(sizeof(char), sLen + 2);
	strncpy(fName, ts->name, sLen-4);
	for (int i = 0; i < ts->numTex; i++) {
		char *cur = (char*)calloc(sizeof(char), sLen + 2);
		strcpy(cur, fName);
		cur[sLen - 4] = (unsigned char)i + 48;
		strcat(cur, ".png");
		printf("creating file %s\n", cur);
		stbi_write_png(cur, ts->width, ts->height, ts->channels, layers[i].data, ts->width * ts->channels);	
		free(cur);
	}
	free(fName);
}

void writePalette(textureSource *ts, char *name) {
	int sLen = strlen(name);
	char *fName = (char*)calloc(sizeof(char), sLen + 13);
	strncpy(fName, name, sLen);
	printf("file name = %s\n");
	strcat(fName, "Palette0.c");
	printf("creating palette %s at %s\n", name, fName);
	FILE* fptr = fopen(fName, "w");
	char *varName = (char*)calloc(sizeof(char), 16 + strlen(name));
	strcpy(varName, "float ");
	strcat(varName, name);
	strcat(varName, "Palette0");
	strcat(varName, "[");
	fprintf(fptr, varName);
	fprintf(fptr, "%i", ts->numTex*3);
	fprintf(fptr, "] = {");

	for (int i = 0; i < ts->numTex * 3; i++) {
		fprintf(fptr, "%f", ts->colors[i]);
		if (i != (ts->numTex * 3) -1) {
			fprintf(fptr, ", ");
		}
	}
	fprintf(fptr, "};\n");
	fclose(fptr);
}
*/
