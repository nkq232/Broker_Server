#ifndef THREADQUEUE_H_
#define THREADQUEUE_H_

struct Node {
	int* confd;
	struct Node* next;
	
};

typedef struct Node nodex;

int* dequeue();
void enqueue();

#endif