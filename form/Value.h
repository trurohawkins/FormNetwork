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
//returns first instance of stat, maybe do average or total later on
//mainly used in Eco for water where this is fine
float *getStatCell(Cell *c, char *stat);
void setStat(Form *f, char *stat, float val);
#endif
