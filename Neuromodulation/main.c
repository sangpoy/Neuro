/*
 main.c
 */
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"
#include "printf.h"
#include "init.h"
#include "atoi.h"
#include "passer.h"

//struct payload F1, F2, F3, F4, F5;

volatile unsigned int i = 0;
volatile unsigned int k = 0;
int flag = 0;

Data data1;
Queue queue2;
Num number;

Num *Numb = (Num *) &number;
Data *Dat = (Data *) &data1;
Queue *Que = (Queue *) &queue2;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

	P1DIR = 0x0f;								// LED 방향설정
	//init_UCS();
	init();
	LED();
	initQueue(Que, Numb);
	Que->distinguish = 0;
	__bis_SR_register(GIE);       // interrupts enabled

	while (1) {

		if (dequeue(Que) != -1) {
			Que->distinguish++;
			check(Que, Numb);
			if (Que->distinguish % 30 == 0) { // 신호가 30byte
				flag = 1;
			}
		}
		if (flag == 1) {	// 30byte 당 신호 처리
			data_process(Que, Numb, Dat);
			//passer(Dat);
			print(Dat);
			flag = -1;
		}

	}
	__no_operation();                         // For debugger

}

// Echo back RXed character, confirm TX buffer is ready first
//#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_ISR(void) {

	while (UCA1IFG & UCRXIFG) {

		enQueue(Que, UCA1RXBUF);

	}
}
