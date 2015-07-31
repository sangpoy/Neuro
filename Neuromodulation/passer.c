/*
 * passer.c
 *
 *  Created on: 2015. 7. 31.
 *      Author: sangpoy
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "init.h"
#include "printf.h"
#include "passer.h"
/*
 *  처리...
 */
void passer(Data *a) {
	// 전극번호
	switch (a->electrode) {
	case 0:
		printf("electrode is 0\n");
		break;
	case 1:
		printf("electrode is 1\n");
		break;
	case 2:
		printf("electrode is 2\n");
		break;
	case 3:
		printf("electrode is 3\n");
		break;
	case 4:
		printf("electrode is 4\n");
		break;
	case 5:
		printf("electrode is 5\n");
		break;
	case 6:
		printf("electrode is 6\n");
		break;
	case 7:
		printf("electrode is 7\n");
		break;
	default:
		printf("electrode else....\n");
		break;

	}

	__delay_cycles(100);

	// pulse shape
	switch (a->pulseshape) {
	case 0:
		printf("pulseshape 0\n");
		break;
	case 1:
		printf("pulseshape 1\n");
		break;
	case 2:
		printf("pulseshape 2\n");
		break;
	case 3:
		printf("pulseshape 3\n");
		break;
	case 4:
		printf("pulseshape 4\n");
		break;
	case 5:
		printf("pulseshape 5\n");
		;
		break;
	case 6:
		printf("pulseshape 6\n");
		break;
	case 7:
		printf("pulseshape 7\n");
		;
		break;
	case 8:
		printf("pulseshape 8\n");
		break;
	default:
		printf("pulseshape else...\n");
		break;

	}
	__delay_cycles(100);

	printf("F1 amplitude %d\n", a->F1.amplitude);
	printf("F1 duration %d\n", a->F1.duration);
	__delay_cycles(100);

	printf("F2 amplitude %d\n", a->F2.amplitude);
	printf("F2 duration %d\n", a->F2.duration);
	__delay_cycles(100);

	printf("F3 amplitude %d\n", a->F3.amplitude);
	printf("F3 duration %d\n", a->F3.duration);
	__delay_cycles(100);

	printf("F4 amplitude %d\n", a->F4.amplitude);
	printf("F4 duration %d\n", a->F4.duration);
	__delay_cycles(100);

	printf("F5 amplitude %d\n", a->F5.amplitude);
	printf("F5 duration %d\n", a->F5.duration);
	__delay_cycles(100);
}

