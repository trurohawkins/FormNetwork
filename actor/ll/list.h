typedef struct linkedList
{
	void *data;
	struct linkedList *next;
} linkedList_t;

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
