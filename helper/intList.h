#ifndef IL
#define IL

typedef struct intList
{
	int data;
	struct intList *next;
} intList;

intList *makeIntList(int data);
void addToIntList(intList **head, int item);
bool addToIntListSingle(intList **head, int item);
int removeFromIntList(intList **head, int item);
void checkAndRemoveInt(intList **head, bool (*chk)(int));
int countList(intList *head);
void freeIntList(intList **head);
#endif
