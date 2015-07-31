/*
 * printf.c
 *
 *  Created on: 2015. 7. 22.
 *      Author: sangpoy
 */
#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include "printf.h"

int fputc(int _c, register FILE *_fp) {
	while (!(UCA1IFG & UCTXIFG))
		;
	UCA1TXBUF = (unsigned char) _c;

	return ((unsigned char) _c);
}

int fputs(const char *_ptr, register FILE *_fp) {
	unsigned int i, len;

	len = strlen(_ptr);

	for (i = 0; i < len; i++) {
		while (!(UCA1IFG & UCTXIFG))
			;
		UCA1TXBUF = (unsigned char) _ptr[i];
	}

	return len;
}

