#ifndef CELL
#define CELL

typedef struct Cell {
	float color[3];
	int pos[2];//for debugging
	int solid;
	linkedList *within;
	int count;
} Cell;

Cell *makeCell(int x, int y);
void addToCell(Cell *c, Form *f);
Form *removeFromCell(Cell *c, Form *f);
// removes solid forms from Cell
linkedList *getSolidForm(Cell *c);
// copies references of solid Forms from Cell
linkedList *checkSolidForm(Cell *c);
// checks if there is at least one solid Form in the Cell
bool isSolidForm(Cell *c);
Form **getCellContents(Cell *c);
bool writeCell(char *file, Cell *c);
void printCell(Cell *c);
void freeCell(Cell *c);
#endif
