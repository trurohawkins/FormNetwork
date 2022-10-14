#ifndef LL
#define LL
typedef struct linkedList
{
	void *data;
	struct linkedList *next;
	struct LinkedList *prev;
} linkedList;

#include <stdio.h>
#include <stdlib.h>

linkedList *makeList();

void addToList(linkedList **head, void *item);
void *printList(linkedList **head, char *listName, void (*print)(void*));
bool cmpList(linkedList **head, void *item, bool (*cmp)(void*, void*));
void *checkList(linkedList **head, bool (*chk)(void*));
bool isInList(linkedList **head, void *item);
bool isInListInt(linkedList **head, int item);
void *indexList(linkedList **head, int index);
void *removeFromList(linkedList **head, void *item);
void *removeFromListCheck(linkedList **head, bool (*chk)(void*));
void *removeFromListInt(linkedList **head, int item);
void **getContents(linkedList **head, int num);
void freeList(linkedList **ll);
void deleteList(linkedList **ll, void (*deleteFunc)(void*));
void freeListSaveObj(linkedList **ll);
#endif
