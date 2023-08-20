#include "msp.h"
#include "PROJ_H.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void ADCInit(void);//initialize the ADC

float tempRead(void); //read temperature sensor

//function definitions
void ADCInit(void)
{
	//Ref_A settings
	REF_A ->CTL0 &= ~0x8; //enable temp sensor
	REF_A ->CTL0 |= 0x30; //set ref voltage
	REF_A ->CTL0 &= ~0x01; //enable ref voltage
	
	//do ADC stuff
	ADC14 ->CTL0 |= 0x10; //turn on the ADC
	ADC14 ->CTL0 &= ~0x02; //disable ADC
	ADC14 ->CTL0 |=0x4180700; //no prescale, mclk, 192 SHT
	ADC14 ->CTL1 &= ~0x1F0000; //configure memory register 0
	ADC14 ->CTL1 |= 0x800000; //route temp sense
	ADC14 ->MCTL[0] |= 0x100; //vref pos int buffer
	ADC14 ->MCTL[0] |= 22; //channel 22
	ADC14 ->CTL0 |=0x02; //enable adc
		
	return;
} 

float tempRead(void)
{
	float temp; //temperature variable
	uint32_t cal30 = TLV ->ADC14_REF2P5V_TS30C; //calibration constant
	uint32_t cal85 = TLV ->ADC14_REF2P5V_TS85C; //calibration constant
	float calDiff = cal85 - cal30; //calibration difference
	ADC14 ->CTL0 |= 0x01; //start conversion
	while((ADC14 ->IFGR0) ==0) 
	{
		//wait for conversion
	}
	temp = ADC14 ->MEM[0]; //assign ADC value
	temp = (temp - cal30) * 55; //math for temperature per manual
	temp = (temp/calDiff) + 30; //math for temperature per manual
	
	return temp; //return temperature in degrees C
}

void UARTInit(void)
{
	EUSCI_A0 ->CTLW0 |= 0x01;
	EUSCI_A0 ->MCTLW = 0;
	EUSCI_A0 ->CTLW0 |= 0x80;
	EUSCI_A0 -> BRW = 0x1A;
	P1 ->SEL0 |= 0x0C;
	P2 ->SEL1 &= ~0x0C;
	EUSCI_A0 ->CTLW0 &= ~0x01;
	
	return;
}

void Uprint(char x[])
{
	int i = 0;
	while(i < strlen(x))
	{
		EUSCI_A0 ->TXBUF = x[i];
		while((EUSCI_A0 ->IFG & 0x02) == 0)
		{
			
		}
		i++;
	}
	return;
}
char Uscan(void)
{
	char y;
	while(1)
	{
		if((EUSCI_A0 ->IFG & 1) == 1)
		{
			y = EUSCI_A0 -> RXBUF;
			Uprint("\n\r");
			EUSCI_A0 ->TXBUF = y;
			while((EUSCI_A0 ->IFG & 2) == 0)
			{
				
			}
			break;
		}
		else
		{
			
		}
	}
	Uprint("\n\r");
	
	return y;
}

void pinEnable(void)
{
	P1 -> SEL0 &= ~0x12;
	P1 -> SEL0 &= ~0x12;
	P1 -> DIR &= ~0x12;
	P1 -> REN |= 0x12;
	P1 -> OUT |= 0x12;
	P2 -> SEL0 &= ~0x07;
	P2 -> SEL1 &= ~0x07;
	P2 -> DIR |= 0x07;
	P2 -> OUT &= ~0x07;
	TIMER32_1 -> CONTROL |= 0x42;
	SysTick -> CTRL |= 0x04;
	
	return;
}

void RGBControl(void)
{
	char x, y, z;
	int h, i, j;
	Uprint("Enter Combination of RGB (1-7):");
	x = Uscan();
	h = (x - '0');
	Uprint("Enter Toggle Time:");
	y = Uscan();
	i = (y - '0');
	Uprint("Enter Number of Blinks:");
	z = Uscan();
	j = (2 *(z - '0'));
	
	SysTick ->LOAD = ((i * 3000000) - 1);
	
	for(int k = 0; k < j; k++)
	{
		P2 ->OUT ^= h;
		SysTick ->CTRL = 0x01;
		while((SysTick ->CTRL & 0x10000) == 0)
		{
		
		}
		SysTick ->CTRL &= ~0x01;
	}
	return;
}

void buttonPress(void)
{
	if(((P1->IN & 0x02) == 0) && ((P1->IN & 0x10) == 0))
	{
		Uprint("Both buttons pressed\n\r");
	}
	else if(((P1->IN & 0x02) == 0) && ((P1->IN & 0x10) != 0))
	{
		Uprint("Button 1 pressed\n\r");
	}
	else if(((P1->IN & 0x02) != 0) && ((P1->IN & 0x10) == 0))
	{
		Uprint("Button 2 pressed\n\r");
	}
	else
	{
		Uprint("No buttons pressed\n\r");
	}
	
	return;
}
