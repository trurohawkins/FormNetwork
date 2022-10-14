#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include<stdlib.h>
#include <stdio.h>
#include <errno.h>

char *readFile(char *file);
char *fileRec(int fd, int cCount);
char *getFileName(char *str);
char *fileToString(char *fileName);

int fileSize = 0;

//agrv: [1]name of c file [>1].glsl files
int main(int argc, char **argv) {
	if (argc > 2) {
		FILE* fptr = fopen(argv[1], "w");
		fprintf(fptr, "#ifndef CONTROLLERDB\n#define CONTROLLERDB\n");
		//fprintf(fptr, "#ifndef GLSLLIB\n#define GLSLLIB\n");
		fclose(fptr);
		for (int i = 2; i < argc; i++) {
			//char *file = readFile(argv[i]);
			char *file = fileToString(argv[i]);
			if (file != NULL) {
				char *name = getFileName(argv[i]);
				int nameSize = strlen(name);
				int fullSize = fileSize + 12 + nameSize + 6 + 1; 
				char *dest = (char*)malloc(fullSize +1);
				strcpy(dest, "const char* ");
				strcat(dest, name);
				strcat(dest, " =  ");
				dest[15 + nameSize] = 34;
				strcat(dest, file);
				dest[fullSize-3] = 34;
				dest[fullSize-2] = ';';
				dest[fullSize-1] = '\n';
				dest[fullSize] = '\0';
				fptr = fopen(argv[1], "a");
				fprintf(fptr, dest);
				fclose(fptr);
				free(name);
				free(dest);
				free(file);
			}
		}
		fptr = fopen(argv[1], "a");
		fprintf(fptr, "#endif\n");
		fclose(fptr);
		return 0;
	} else {
		printf("no file given\n");
		return 1;
	}
}

char *readFile(char *file) {
	char *a;
	int fd = open(file, O_RDONLY);
	if (fd != -1) {
		a = fileRec(fd, 0);
		char *string = (char*)a;
		close(fd);
		return string;
	} else {
		return NULL;
	}
}

char *fileRec(int fd, int cCount) {
	char c;
	char *tmp;
	if (read(fd, &c, 1) != 0) {
		int inc = 0;
		if (c == '\n') {
			inc = 1;
		}
		tmp = fileRec(fd, cCount + 1 + inc);
		if (inc == 0) {
			tmp[cCount] = c;
		} else {
			tmp[cCount] = 92;
			tmp[cCount+1] = 110;
		}
		return tmp;
	} else {
		tmp = (char*)calloc(cCount+1, sizeof(char));
		fileSize = cCount;
		tmp[cCount] = '\0';
		return tmp;
	}
}

char *fileToString(char *txt)
{
	FILE *fptr;
	int n = 0;
	int c;

	fptr = fopen(txt, "r");
	char *t = NULL;
	if (fptr != NULL) {
		fseek(fptr, 0, SEEK_END);
		long fSize = ftell(fptr);
		//printf("file size: %i\n", fSize);
		printf("first size %i\n", fSize);
		fseek(fptr, 0, SEEK_SET);
		while ((c = fgetc(fptr)) != EOF) {
			if (c == '\n') {
				fSize++;
			}
		}
		fileSize = fSize;
		printf("2nd size %i\n", fSize);
		fseek(fptr, 0, SEEK_SET);
		t = (char*)malloc((fSize+1) * sizeof(char));
		while ((c = fgetc(fptr)) != EOF) {
			if (c != '\n') {
				t[n++] = (char)c;
			} else {
				t[n++] = 92;
				t[n++] = 110;
			}
		}
		//printf("wrote into: %i\n", n);
		t[fSize] = '\0';
		fseek(fptr, 0, SEEK_SET);
		fclose(fptr);
	}
	return t;
}

char *getFileName(char *str) {
	int count = 0;
	char *tmp = str;
	while (*tmp != '.' && *tmp != '\n') {
		count++;
		tmp++;
	}
	char *name = (char *)malloc(count + 1);
	for (int i = 0; i < count; i++) {
		name[i] = str[i];
	}
	name[count] = '\0';
	return name;
}
