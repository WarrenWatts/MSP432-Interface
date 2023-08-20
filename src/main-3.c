//Final Project
//Warren Watts
//Section: 506

#include "msp.h"
#include "PROJ_H.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main (void)
{
	char i, z;
	int k, j;
	float tempC, tempF;
	
	UARTInit();
	pinEnable();
	ADCInit();

	while(1)
	{
		Uprint("MSP432 Menu\n\n\r");
		Uprint("1. RGB Control\n\r");
		Uprint("2. Digital Input\n\r");
		Uprint("3. Temperature Reading\r");
		z = Uscan();
		
		switch(z)
		{
			case('1'):
				RGBControl();
				Uprint("Done\n\n\r");
				continue;
			case('2'):
				buttonPress();
				Uprint("\n\r");
				continue;
			case('3'):
				Uprint("Enter Number of Temperature Reading (1-5):");
				i = Uscan();
				k = i - '0';
				TIMER32_1 -> LOAD = 3000000;
				for(j = 0; j < k; j++)
				{
					char word[25];
					tempC = tempRead();
					tempF = (tempC * (9.0/5.0)) + 32;
					sprintf(word, "Reading %d: %.2f C & %.2f F\n\r", (j+1), tempC, tempF);
					int d = 0;
					while(word[d] != 0)
					{
						EUSCI_A0 ->TXBUF = word[d];
						while((EUSCI_A0->IFG & 2) == 0)
						{
							
						}
						d++;
					}
					TIMER32_1 -> CONTROL |= 0x80;
					while((TIMER32_1 -> RIS & 1) == 0)
					{
						
					}
					TIMER32_1 -> INTCLR &= ~0x01;
					TIMER32_1 -> CONTROL &= ~0x80;
				}
				Uprint("\n\r");
				continue;
			default:
				Uprint("\n\r");
				continue; 
		}
		
	}
}
