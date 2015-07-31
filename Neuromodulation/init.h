/*
 * init.h
 *
 *  Created on: 2015. 7. 22.
 *      Author: sangpoy
 */
#include "Queue.h"

#ifndef INIT_H_
#define INIT_H_

#define STX 0x02
#define ETX 0x03

typedef struct {
	int amplitude;
	int duration;
} Payload;

typedef struct {
	int temporary[15];
	int electrode;		// 전극형태
	int pulseshape;		// 형태
	Payload F1, F2, F3, F4, F5;
} Data;

//typedef struct {
//	int stx_num;
//	int etx_num;
//	int flag;
//} Num;

//Data data;

void LED(void);
void init(void);
void init_UCS(void);
void check(Queue *a, Num *b);
void print(Data *c);
void data_process(Queue *a, Num *b, Data *c);
#endif /* INIT_H_ */
