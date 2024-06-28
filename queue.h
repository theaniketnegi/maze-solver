#ifndef QUEUE_H
#define QUEUE_H


typedef struct Position
{
	int row;
	int col;
} Position;

typedef struct Node
{
	Position data;
	struct Node *next;
} Node;

typedef struct Queue
{
	Node *front;
	Node *rear;
} Queue;

void initializeQueue(Queue *q);
int isQueueEmpty(Queue *q);
void qPush(Queue *q, Position value);
Position qPop(Queue *q);

#endif