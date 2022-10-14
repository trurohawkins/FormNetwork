typedef struct linkedList
{
	void *data;
	struct linkedList *next;
} linkedList_t;

#include <stdio.h>
#include <stdlib.h>
linkedList_t *paramsToList(int ac, char *const *av);

void addToList(linkedList_t *head, void *item);

void freeList(linkedList_t **ll);

void printList(linkedList_t *list);

int listSize(linkedList_t const *begin);

void revList(linkedList_t **begin);

void applyOnNodes(linkedList_t *begin, int (*f)(void *));

int function(void *a);

void applyOnMatchingNodes(linkedList_t *begin, int (*f)(void *), void const *data_ref, int(*cmp)());

linkedList_t *findNode(linkedList_t *begin, int (*cmp)(), void const *dataRef);

void deleteNodes(linkedList_t **begin, int (*cmp)(), void const *dataRef);

void concatList(linkedList_t **list1, linkedList_t **list2);

void addToList(linkedList_t *head, void *item) {
	while(head->data != 0) {
		if (head->next == 0) {
			head->next =(linkedList_t*)malloc(sizeof(struct linkedList)); 
		}
		head = head->next;
	}
	head->data = item;
}

linkedList_t *paramsToList(int ac, char *const *av)
{
	linkedList_t* head = (linkedList_t*)malloc(sizeof(struct linkedList));
	head->data = av[0];
	linkedList_t *tmp = head;
	int i = 1;
	while (i < ac) {
		tmp->next = (linkedList_t*)malloc(sizeof(struct linkedList));
		tmp->next->data = av[i];
		tmp = tmp->next;
		i++;
	}
	tmp->next = NULL;
	return head;
}

void freeList(linkedList_t **ll) {
	linkedList_t *cur = *ll;
	linkedList_t *next;
	while (cur != NULL) {
		next = cur->next;
		free(cur);
		cur = next;
	}	
}

void printList(linkedList_t *list)
{
	while (list != NULL) {
		//putStr((char*)list->data);
		printf("%i\n", *((int*)list->data));
		list = list->next;
	}
}

int listSize(linkedList_t const *begin)
{
	int size = 0;
	while (begin != NULL) {
		begin = begin->next;
		size++;
	}
	return size;
}

void revList(linkedList_t **begin)
{
	linkedList_t *cur = begin[0];	
	linkedList_t* prev = NULL;
	linkedList_t* next = cur->next;
	while (cur != NULL) {
		cur->next = prev;
		prev = cur;
		cur = next;
		if (cur != NULL) {
			next = cur->next;
		}
	}
	begin[0] = prev;
}

void applyOnNodes(linkedList_t *begin, int (*f)(void *))
{
	while (begin != NULL) {
		f(begin->data);
		begin = begin->next;	
	}
}

int function(void *a)
{
	//putStr((char*)a);
	return 1;
}

void applyOnMatchingNodes(linkedList_t *begin, int (*f)(void *), void const *data_ref, int(*cmp)())
{
	while (begin != NULL) {
		if (cmp(begin->data, data_ref) == 0) {
			f(begin->data);
		}
		begin = begin->next;
	}
}

linkedList_t *findNode(linkedList_t *begin, int (*cmp)(), void const *dataRef)
{	
	while (begin != NULL) {
		if (cmp(begin->data, dataRef) == 0) {
			return begin;
		} else {
			begin = begin->next;
		}
	}
	return NULL;
}

void deleteNodes(linkedList_t **begin, int (*cmp)(), void const *dataRef)
{
	if (cmp(begin[0]->data, dataRef) == 0) {
		linkedList_t *oldHead = begin[0];
		begin[0] = begin[0]->next;
		free(oldHead);
	}
	linkedList_t *tmp = begin[0]->next;
	linkedList_t *prev = begin[0];
	while (tmp != NULL) {
		if (cmp(tmp->data, dataRef) == 0) {
			prev->next = tmp->next;
			free(tmp);		
			tmp = prev->next;
		} else {
			tmp = tmp->next;
		}
		prev = prev->next;
	}
}

void concatList(linkedList_t **list1, linkedList_t **list2)
{
	linkedList_t *tmp = list1[0];
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = list2[0];
}

