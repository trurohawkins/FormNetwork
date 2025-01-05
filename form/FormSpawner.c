#include "FormSpawner.h"
int rPower = 10;
int numForms = 0;
int formCount = 0;
FormRecipe **recipes;

void initRecipes(int number, int n_pow) {
	rPower = n_pow;
	numForms = number;
	recipes = (FormRecipe**)calloc(sizeof(FormRecipe*), number);
}

void addRecipe(Form *(*make)(int), int(*save)(Form*), int index) {
	recipes[index] = (FormRecipe*)calloc(sizeof(FormRecipe), 1);
	recipes[index]->makeFunc = make;
	recipes[index]->saveFunc = save;
}

FormRecipe *getRecipe(int id) {
	if (id > -1) {
		int index = id / rPower;
		//printf("index: %i\n", index);
		if (index < numForms) {
			return recipes[index];
		} 
	}

	return 0;
}

int getRecipePow() {
	return rPower;
}

void freeRecipes() {
	for (int i = 0; i < numForms; i++) {
		if (recipes[i] != 0) {
			free(recipes[i]);
		}
	}
	free(recipes);
}
