#include "binaryWriter.h"

bool writeBinaryInt(char *file, int *val, int num) {
	FILE *fptr = fopen(file, "ab");
	if (fptr != NULL) {
		size_t elementSize = sizeof(int);
		size_t elementNum = num;
		size_t elementsWritten = fwrite(val, elementSize, elementNum, fptr);
		fclose(fptr);
		if (elementsWritten == elementNum) {
			return true;
		}
	}
	printf("unable to write %i to file %s\n", val, file);
	return false;
}

int fput32le(int x, char *file) {
	FILE *fptr = fopen(file, "wb");
	if (fptr != NULL) {
		unsigned int rep = x;
		int e1, e2;
		e1 = fputc(rep & 0xFF, fptr);
		e2 = fputc((rep >> 8) & 0xFF, fptr);
		//e1 = fputc(rep & 0xFFFF, fptr);
		//e2 = fputc((rep >> 16) & 0xFFFF, fptr);

		if (e1 == EOF || e2 == EOF) {
			return EOF;
		}
	}
	return 0;
}

int *readBinaryInt(FILE *fptr, int num) {
	//FILE *fptr = fopen(file, "rb");
	if (fptr != NULL) {
		int *val = (int*)calloc(num, sizeof(int));;
		fread(val, sizeof(int), num, fptr);
		//fclose(fptr);
		return val;
	}
	return 0;
}

