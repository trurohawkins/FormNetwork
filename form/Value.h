#ifndef VALUE
#define VALUE

typedef struct Value {
	char *key;
	float value;
} Value;

Value *makeValue(char *key, float value);
void freeValue(void *v);
void addStat(Form *f, char *k, float val);
float *getStat(Form *f, char *stat);
void setStat(Form *f, char *stat, float val);
#endif
