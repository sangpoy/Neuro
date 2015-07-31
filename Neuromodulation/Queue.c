/*
 * Queue.c
 *
 *  Created on: 2015. 7. 21.
 *  Author: sangpoy
 */

#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

/*
 *  초기화
 */
void initQueue(Queue *a, Num *b) {
	a->rear = 0;		// data insert
	a->front = 0;		// data delete
	b->flag = -1;
}

/*
 *  포화 검사
 */
int isFull(Queue *a) {
	if (((a->rear + 1) % Buffer_size) == a->front) {
		return TRUE;
	}

	else {
		return FALSE;
	}
}

/*
 *  공백 검사
 */
int isEmpty(Queue *a) {
	if (a->rear == a->front) {
		return TRUE;
	}

	else {
		return FALSE;
	}
}
/*
 *  데이터 입력
 */
void enQueue(Queue *a, char data) {
	if (isFull(a)) {
		a->rear=1;
		a->front=0;
		a->Buffer[a->rear]=data;
	} else {
		a->rear = (a->rear + 1) % Buffer_size;
		a->Buffer[a->rear] = data;
	}
}

/*
 *  데이터 추출
 */
int dequeue(Queue *a) {

	if (isEmpty(a)) {
	//	r->empty = -1;
		return -1;
	} else {
		//rr = a->front;
		a->temp = a->front+1;
		a->front = (a->front + 1) % Buffer_size;
		//r->rchar = a->Buffer[rr];
		return a->temp;
	}
}
