#ifndef IL
#define IL

typedef struct intList
{
	int data;
	struct intList *next;
} intList;

intList *makeIntList();
void addToIntList(intList **head, int item);
int removeIntList(intList **head, int item);
void freeIntList(intList **head);
#endif
