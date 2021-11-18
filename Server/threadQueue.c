#include "threadQueue.h"
#include <stdlib.h>

nodex* head = NULL;
nodex* current = NULL;
// Thêm vào link list
void enqueue(int* confd){
	nodex *new = malloc(sizeof(nodex));
	new->confd = confd;
	new->next = NULL;
	if (current == NULL)
	{
		/* code */
		head = new;
	} else {
		current->next = new;
	}
	current = new;
}
// Xóa khỏi link list
// Check if NULL
// Return socket
int* dequeue() {
	if (head == NULL)
	{
		/* code */
		return NULL;
	}
	int* check = head->confd;
	nodex *temp = head;
	head = head->next;
	if (head == NULL)
	{
		/* code */
		current = NULL;
	}
	free(temp);
	return check;
}
