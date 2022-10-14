#ifndef FORMSPAWNER
#define FORMSPAWNER

typedef struct FormRecipe {
	int ID;
	Form *(*makeFunc)(int);
	int (*saveFunc)(Form*);
} FormRecipe;


void initRecipes(int number, int pow);
void addRecipe(Form *(*make)(int), int(*save)(Form*), int index);
FormRecipe *getRecipe(int id);
int getRecipePow();
void freeRecipes();
#endif
