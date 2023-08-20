#ifndef PROJ_H
#define PROJ_H

void UARTInit(void);
char Uscan(void);
void pinEnable(void);
void RGBControl(void);
void buttonPress(void);
void Uprint(char x[]);
void ADCInit(void);//initialize the ADC
float tempRead(void); //read temperature sensor



#endif
