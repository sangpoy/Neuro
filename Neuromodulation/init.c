/*
 * init.c
 *
 *  Created on: 2015. 7. 22.
 *      Author: sangpoy
 *      참고 - http://cafe.naver.com/embeddedworkshop/726
 */
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "printf.h"
#include "init.h"
#include "Queue.h"
#include "atoi.h"

void init(void) {

	P4SEL |= BIT4 + BIT5;
	UCA1CTL1 |= UCSWRST;                      // **Put state machine in reset**
	UCA1CTL1 |= UCSSEL_2;                    // SM

	UCA1BR0 = 6;                             // 1MHz 9600 (see User's Guide)
	UCA1BR1 = 0;                              // 1MHz 9600
	UCA1MCTL |= UCBRS_0 + UCBRF_13 + UCOS16;   				// Modln UCBRSx=1
	//UCA1MCTL |= 0x81 + 0x20;   						// Modln UCBRSx=1
	UCA1CTL1 &= ~UCSWRST;               // **Initialize USCI state machine**
	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

//	UCA1BR0 = 9;
//	UCA1BR1 = 0;
//	UCA1MCTL |= UCBRS1;   						// Modln UCBRSx=1
//	UCA1CTL1 &= ~UCSWRST;               // **Initialize USCI state machine**
//	UCA1IE |= UCRXIE;                         // Enable USCI_A1 RX interrupt

}

void init_UCS(void) {
	P2DIR |= BIT2;                            // SMCLK set out to pins
	P2SEL |= BIT2;

	UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	UCSCTL4 |= SELS_4;                        // Set ACLK = REFO
	UCSCTL4 |= SELA_2;

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;                     // Select DCO range 24MHz operation
	UCSCTL2 = FLLD_1 + 374;                   // Set DCO Multiplier for 12MHz
											  // (N + 1) * FLLRef = Fdco
											  // (374 + 1) * 32768 = 12MHz
											  // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
//	__delay_cycles(375000);

	// Loop until XT1,XT2 & DCO fault flag is cleared
	do {
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		// Clear XT2,XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	} while (SFRIFG1 & OFIFG);                   // Test oscillator fault flag
}

void LED(void) {
	P1OUT = 0x0f;
	__delay_cycles(262144);		//0.25sec
	P1OUT = 0x00;
	__delay_cycles(262144);
	P1OUT = 0x0f;
	__delay_cycles(262144);
	P1OUT = 0x00;
	__delay_cycles(262144);
	P1OUT = 0x0f;
	__delay_cycles(262144);
	P1OUT = 0x00;
}
/*
 *  버퍼를 스캔하고, stx,etx를 찾는다.
 */
void check(Queue *a, Num *b) {
	int count;

	b->etx_before = 0;
	b->etx_after = 49;


	for (count = 1; count < Buffer_size; count++) {
		if (a->Buffer[count] == STX) {
			b->stx_num = count;
		} else if (a->Buffer[count] == ETX) {

			switch (b->flag) {
			case -1: // 최초실행시
				b->etx_before = count;
				b->flag = 0;
				break;
			case 0: // 첫번째
				b->etx_before = count;
				b->flag = 1;
				break;
			case 1: // 두번째
				b->etx_after = count;
				b->flag = 0;
				break;
			}

			if ((b->etx_before < b->etx_after) & (b->etx_after < b->stx_num)) {
				b->etx_num = b->etx_before;
				b->select = 1;
			} else if ((b->etx_after > b->stx_num) & (b->etx_before < b->stx_num)) {
				b->etx_num = b->etx_after;
				b->select = 2;
			} else if ((b->etx_after > b->stx_num) & (b->etx_before > b->stx_num)) {
				b->etx_num = b->etx_before;
				b->select = 2;
			}

		}
	}

}

void data_process(Queue *a, Num *b, Data *c) {
	int count;
	int flag = 0;
	char temp[15];	//분리
	int index = 0;
	int j = 0;

	if (b->select == 1) {
		for (count = (b->stx_num + 1); count < Buffer_size; count++) {
			__delay_cycles(100);
			if (a->Buffer[count] != ';') {
				temp[j] = a->Buffer[count];
				j++;
			} else {
				temp[j] = '\0';
				c->temporary[index] = atoi(temp);
				index++;
				j = 0;

			}

			flag = 1;

		}

	}

	else if (b->select == 2) {
		//	printf("===  ");
		__delay_cycles(100);
		for (count = (b->stx_num + 1); count < b->etx_num; count++) {

			if (a->Buffer[count] != ';') {

				temp[j] = a->Buffer[count];
				j++;
			} else {
				temp[j] = '\0';
				c->temporary[index] = atoi(temp);
				index++;
				j = 0;

			}

		}

	}

	if ((b->select == 1) & (flag == 1)) {

		//	printf("***  ");
		for (count = 0; count < b->etx_before; count++) {
			__delay_cycles(100);
			if (a->Buffer[count] != ';') {
				temp[j] = a->Buffer[count];
				j++;
			} else {
				temp[j] = '\0';
				c->temporary[index] = atoi(temp);
				index++;
				j = 0;

			}
		}

	}

	__delay_cycles(100);

	c->electrode = c->temporary[0];
	c->pulseshape = c->temporary[1];
	c->F1.amplitude = c->temporary[2];
	c->F1.duration = c->temporary[3];
	c->F2.amplitude = c->temporary[4];
	c->F2.duration = c->temporary[5];
	c->F3.amplitude = c->temporary[6];
	c->F3.duration = c->temporary[7];
	c->F4.amplitude = c->temporary[8];
	c->F4.duration = c->temporary[9];
	c->F5.amplitude = c->temporary[10];
	c->F5.duration = c->temporary[11];

}
/*
 *  디버깅용 출력
 */
void print(Data *c) {

	printf("%d ", c->electrode);
	printf("%d ", c->pulseshape);
	printf("%d ", c->F1.amplitude);
	printf("%d ", c->F1.duration);
	printf("%d ", c->F2.amplitude);
	printf("%d ", c->F2.duration);
	printf("%d ", c->F3.amplitude);
	printf("%d ", c->F3.duration);
	printf("%d ", c->F4.amplitude);
	printf("%d ", c->F4.duration);
	printf("%d ", c->F5.amplitude);
	printf("%d \n", c->F5.duration);

}

