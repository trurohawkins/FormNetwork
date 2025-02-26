#ifndef IL
#define IL

typedef struct intList
{
	int data;
	struct intList *next;
} intList;

intList *makeIntList(int data);
void addToIntList(intList **head, int item);
int removeFromIntList(intList **head, int item);
void checkAndRemoveInt(intList **head, bool (*chk)(int));
void freeIntList(intList **head);
#endif
