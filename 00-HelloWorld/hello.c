#include <stdint.h>
#include "reg.h"

#define USART_FLAG_TXE	((uint16_t) 0x0080)

int puts(const char *str)
{
	while (*str) {
		while (!(*(USART2_SR) & USART_FLAG_TXE));
		*(USART2_DR) = *str++ & 0xFF;
	}
	return 0;
}

/*
The register reference information can be found from:
www.st.com
	RM0008
	Reference manual
	STM32F101xx, STM32F102xx, STM32F103xx, STM32F105xx and
	STM32F107xx advanced Arm®-based 32-bit MCUs
*/

void main(void)
{

	/* 
	APB2 peripheral clock enable register (RCC_APB2ENR)
	Bit 0 AFIOEN: Alternate function IO clock enable
	           1: Alternate Function IO clock enable
	Bit 2 IOPAEN: IO port A clock enable
           	   1: IO port A clock enabled
	*/	
	*(RCC_APB2ENR) |= (uint32_t) (0x00000001 | 0x00000004);
	
	/*
	APB1 peripheral clock enable register (RCC_APB1ENR)
	Bit 17 USART2EN: USART2 clock enable
	              1: USART2 clock enabled
	*/
	*(RCC_APB1ENR) |= (uint32_t) (0x00020000);

	/*  GPIOA 
	MODE3 = 01: Output mode, max speed 10 MHz.
	CNF3  = 00: General purpose output push-pull
	MODE2 = 10: Output mode, max speed 2 MHz
	CNF2  = 11: Alternate function output Open-drain
	MODE8 ~ MODE15 = 1 (reset values)
	*/
	
	*(GPIOA_CRL) = 0x00004B00;
	*(GPIOA_CRH) = 0x44444444;

	/* USART2 Configuration
	   Bit 3 TE: Transmitter enable
	   Bit 2 RE: Receiver enable
	   Bit 13 UE: USART enable
	*/
	*(USART2_CR1) = 0x0000000C;
	*(USART2_CR1) |= 0x2000;

	puts("Hello World!\n");

	while (1);
}
