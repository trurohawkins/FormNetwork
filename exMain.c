#include <stdlib.h>
#include <stdio.h>
#include "2Darray.c"

int main() {
	//int *arr = calloc(10, sizeof(int));
	/*
	 * int *arr = makeArray(12);
	printf("my pointer arr is at location %p, and its value is %i\n", arr, *arr);
	int num = 420;
	int *ptr = &num;
	printf("my int num is %i at %p, and my pointer is %p at %p\n", *ptr, &num, ptr, &ptr);
	arr[3] = 56;// arr[3] == *(arr + 3)
	printf("arr[3] = %i and *(arr + 3) = %i\n", arr[3], *(arr + 3));
	free(arr);
	*/
	int **world = makeWorld(10, 10);
	deleteWorld(world, 10);
	return 0;
}
