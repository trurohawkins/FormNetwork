#ifndef CELL
#define CELL

typedef struct Cell {
	float color[3];
	int pos[2];//for debugging
	bool solid;
	linkedList *within;
	int count;
} Cell;

Cell *makeCell(int x, int y);
void addToCell(Cell *c, Form *f);
void removeFromCell(Cell *c, Form *f);
Form *getSolidForm(Cell *c);
Form *checkSolidForm(Cell *c);
Form **getCellContents(Cell *c);
bool writeCell(char *file, Cell *c);
void printCell(Cell *c);
void freeCell(Cell *c);

#endif
