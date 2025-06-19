#ifndef CELL
#define CELL

typedef struct Cell {
	float color[3];
	int pos[2];//for debugging
	linkedList *within;
	Form **content;
	int count;
} Cell;

extern int maxCellCount;

Cell *makeCell(int x, int y);
void addToCell(Cell *c, Form *f);
Form *removeFromCell(Cell *c, Form *f);
Form **getCellContents(Cell *c);
bool writeCell(char *file, Cell *c);
void printCell(Cell *c);
void freeCell(Cell *c);
#endif
