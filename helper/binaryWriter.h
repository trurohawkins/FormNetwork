bool writeBinaryInt(char *file, int *val, int num);
int fput32le(int x, char *file);
int *readBinaryInt(FILE *fptr, int num);
