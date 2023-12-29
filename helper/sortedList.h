#ifndef SORTEDLIST
#define SORTEDLIST

typedef struct SortedList {
	void *data;
	int val;
	struct SortedList *pre;
	struct SortedList *next;
} SortedList;

SortedList *makeSlist(void *data, int val);
void sortedAdd(SortedList **head, void *item, int val);
void freeSlist(SortedList *head);
void printSlist(SortedList *head);
#endif
