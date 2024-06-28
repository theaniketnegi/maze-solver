#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void initializeQueue(Queue *q)
{
	q->front = NULL;
	q->rear = NULL;
}

int isQueueEmpty(Queue *q)
{
	return q->front == NULL;
}

void qPush(Queue *q, Position value)
{
	Node *node = (Node *)malloc(sizeof(Node));
	if (!node)
	{
		printf("Error allocating memory");
		exit(1);
	}
	node->data = value;
	node->next = NULL;

	if(q->rear==NULL){
		q->front = q->rear = node;
	} else {
		q->rear->next = node;
		q->rear = node;
	}
}

Position qPop(Queue* q){
	if(isQueueEmpty(q)){
		printf("Invalid access");
		exit(1);
	}

	Node* tmp = q->front;
	Position val = tmp->data;
	q->front = q->front->next;

	if(q->front==NULL){
		q->rear=NULL;
	}

	free(tmp);
	return val;
}

