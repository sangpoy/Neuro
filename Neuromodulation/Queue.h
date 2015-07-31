/*
 * Queue.h
 *
 *  Created on: 2015. 7. 21.
 *      Author: sangpoy
 */

#ifndef NEUROMODULATION_QUEUE_H_
#define NEUROMODULATION_QUEUE_H_

#define Buffer_size 50
#define TRUE 1
#define FALSE 0

typedef struct {
	int rear;	// data insert
	int front;	// data delete
	int temp;
	char Buffer[Buffer_size];
	int distinguish;
} Queue;

typedef struct {
	int stx_num;
	int etx_num;
	int flag;
	int select;
	int etx_before;
	int etx_after;
} Num;

void initQueue(Queue *a, Num *b);
int isFull(Queue *a);
int isEmpty(Queue *a);
void enQueue(Queue *a, char data);
int dequeue(Queue *a);

#endif /* NEUROMODULATION_QUEUE_H_ */
