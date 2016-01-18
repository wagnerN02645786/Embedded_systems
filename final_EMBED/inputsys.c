#include "MKL25Z4.h"                    // Device header
	void delay(int bb);
	void initializer();
	void CMD_WR();
	void DATA_WR();
	void welcome();
	int HexPad();
	char Int_ToChar(int i);
	int Char_ToInt(char c);
	void UART0_initT(void);
	void UART0_initR(void);
	void Ports_init(void);
	void PORTD_IRQHandler(void);
	void Port_inter();
	int num1=7,num2=5;
	int num1C=0x37,num2C=0x35;
int main(void){
		int num3,num4, num;
		//char c1, c2;
		Ports_init(); //Do I need to initialize the ports everytime??
		initializer();
		Port_inter();
		while(1)
		{
					//Transmitting
				num=num1*10;
			num=num+num2;
			
				UART0_initT();
				while(!(UART0->S1 & 0x80)) 
					{}
				UART0->D = num; //* send a char *
						while(!(UART0->S1 & 0x80)) 
					{}
			num=0;
						//Recieving
				UART0_initR();
				while(!(UART0->S1 & 0x20)) 
					{} //* wait for receive buffer full *
				num = UART0->D;
						
						//Converting number to ASCII
			/*	if(num>=10)
				{
					num=num+1;
					num1=num%10;
					num=num|0x30;
				}
				else
				{
					num=num|0x30;
				}*/
						num4=num%10;
						num3=(num-num4)/10;
						num3=num3|0x30;
						num4=num4|0x30;
				PTC->PDOR=0x43;//Writes to LCD
				DATA_WR();
				PTC->PDOR=0x3A;//Writes to LCD
				DATA_WR();
				PTC->PDOR=num3;//Writes to LCD
				DATA_WR();
				PTC->PDOR=num4;//Writes to LCD
				DATA_WR();
				PTC->PDOR=0xA0;//Writes to LCD
				DATA_WR();
				PTC->PDOR=0x53;//Writes to LCD
				DATA_WR();
				PTC->PDOR=0x3A;//Writes to LCD
				DATA_WR();
				PTC->PDOR=num1C;//Writes to LCD
				DATA_WR();
				PTC->PDOR=num2C;//Writes to LCD
				DATA_WR();
				delay(200);
				PTC->PDOR=0x01;//clear display
				CMD_WR();
				PTC->PDOR=0x80;//line 1 space 1
				CMD_WR();
		}
	}
void PORTD_IRQHandler(void)
{
	if (PORTD->ISFR & 0x00000010) {
		delay(200);
		PTC->PDOR=0xC0;//Second Row
				CMD_WR();

		PTC->PDOR=0x45;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x4E;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x54;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x45;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x52;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0xA0;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x54;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x45;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x4D;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x50;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0x3A;//Writes to LCD
				DATA_WR();
		PTC->PDOR=0xA0;//Writes to LCD
				DATA_WR();
				
				
		num1=HexPad();
		num1C=num1|0x30;
		//PTC->PDOR=num1C;//Writes to LCD
			//	DATA_WR();
			delay(500);
		num2=HexPad();
		num2C=num2|0x30;
		//PTC->PDOR=num2C;//Writes to LCD
			//	DATA_WR();
		
		delay(1000);
		PTC->PDOR=0x01;//clear display
				CMD_WR();
		//PTC->PDOR=0x80;//line 1 space 1
			//	CMD_WR();
		
		
		
		PORTD->ISFR = 0x0010; /* clear interrupt flag */
}
}
		
void Port_inter()
{
	__disable_irq(); /* disable all IRQs */

	// configure PTD4 for interrupt
	PORTD->PCR[4] |= 0x00100; /* make it GPIO */
	PORTD->PCR[4] |= 0x00003; /* enable pull-up */
	PTD->PDDR &= ~0x0010; /* make pin input */
	PORTD->PCR[4] &= ~0xF0000; /* clear interrupt selection */
	PORTD->PCR[4] |= 0x80000; /* enable both edge interrupt 8=zero C=1*/ 
	NVIC->ISER[0] |= 0x80000000; /* enable INT31 (bit 31 of ISER[0]) */
	__enable_irq(); /* global enable IRQs */
}
void delay(int bb)
{
	int i=0;
	int j=0;
	for(i=0;i<bb;i++)
	{
		for(j=0;j<7000;j++)
		{}
	}
}
void Ports_init(void)//Initialization of ports
{
	SIM->SCGC5=0x3D80;
		//Hex Keypad E(3,2,1,0) B(3,2,1,0) left to right
		//PORT B Initialized col
		PORTB->PCR[0] =0x0103;
		PORTB->PCR[1] =0x0103;
		PORTB->PCR[2] =0x0103;
		PORTB->PCR[3] =0x0103;
				
		//PORT E Initialized row
		PORTE->PCR[0] =0x0103;
		PORTE->PCR[1] =0x0103;
		PORTE->PCR[2] =0x0103;
		PORTE->PCR[3] =0x0103;
		
		//Port C initialized LCD screen
		PORTC->PCR[0]=0x0100;		//DB0
		PORTC->PCR[1]=0x0100;		//DB1
		PORTC->PCR[2]=0x0100;		//DB2
		PORTC->PCR[3]=0x0100;		//DB3
		PORTC->PCR[4]=0x0100;		//DB4
		PORTC->PCR[5]=0x0100;		//DB5
		PORTC->PCR[6]=0x0100;		//DB6
		PORTC->PCR[7]=0x0100;		//DB7
		
		//Port D Initialized LCD screen
		PORTD->PCR[0]=0x0100;		//RS
		PORTD->PCR[1]=0x0100;		//R/W
		PORTD->PCR[2]=0x0100;		//E

		PTC->PDDR=0xFFFFFFFF;
		PTD->PDDR=0xFFFFFFF7;
}
	int HexPad()
{
	int row=4;
		int col=4;
		int B=500;
		int num;
		PTB->PDDR= 0xFFFFFFFF;
		PTE->PDDR= 0xFFFFFFF0;

		//PTC->PDOR=0x00; //Need then DATA_WR() called to show numbers
		
	//Row
	
		do
		{
			if(PTE->PDIR==0x07)
			{
				row=0;
			}
			else if (PTE->PDIR==0x0B)
			{
				row=1;
			}
			else if(PTE->PDIR==0x0D)
			{
				row=2;
			}
			else if (PTE->PDIR==0x0E)
			{
				row=3;
			}
		}while(row==4);
		
		
		PTB->PDDR= 0xFFFFFFF0;
		PTE->PDDR= 0xFFFFFFFF;
		//Get Columb
		
		
		do
		{
			if(PTB->PDIR==0x07)
			{
				col=0;
			}
			else if (PTB->PDIR==0x0B)
			{
				col=1;
			}
			else if(PTB->PDIR==0x0D)
			{
				col=2;
			}
			else if (PTB->PDIR==0x0E)
			{
				col=3;
			}
		}while(col==4);
		
		
		if(row==0 && col==0)
		{
			PTC->PDOR=0x31;
			DATA_WR();
			num=1;
		}
		else if(row==1 && col==0)
		{
			PTC->PDOR=0x32;
			DATA_WR();
			num=2;
		}
		else if(row==2 && col==0)
		{
			PTC->PDOR=0x33;
			DATA_WR();
			num=3;
		}
		else if(row==3 && col==0)
		{
			PTC->PDOR=0x34;
			DATA_WR();
			num=15;
		}
																	//Row 1
		else if(row==0 && col==1)
		{
			PTC->PDOR=0x34;
			DATA_WR();
			num=4;
		}
		else if(row==1 && col==1)
		{
			PTC->PDOR=0x35;
			DATA_WR();
			num=5;
		}
		else if(row==2 && col==1)
		{
			PTC->PDOR=0x36;
			DATA_WR();
			num=6;
		}
		else if(row==3 && col==1)
		{
			PTC->PDOR=0x45;
			DATA_WR();
			num=14;
		}
																	//Row 2
		else if(row==0 && col==2)
		{
			PTC->PDOR=0x37;
			DATA_WR();
			num=7;
		}
		else if(row==1 && col==2)
		{
			PTC->PDOR=0x38;
			DATA_WR();
			num=8;
		}
		else if(row==2 && col==2)
		{
			PTC->PDOR=0x39;
			DATA_WR();
			num=9;
		}
		else if(row==3 && col==2)
		{
			PTC->PDOR=0x44;
			DATA_WR();
			num=13;
		}
																		//Row3
		else if(row==0 && col==3)
		{
			PTC->PDOR=0x41;
			DATA_WR();
			num=10;
		}
		else if(row==1 && col==3)
		{
			PTC->PDOR=0x30;
			DATA_WR();
			num=0;
		}
		else if(row==2 && col==3)
		{
			PTC->PDOR=0x42;
			DATA_WR();
			num=11;
		}
		else if(row==3 && col==3)
		{
			PTC->PDOR=0x43;
			DATA_WR();
			num=12;
		}

	return num;
}
char Int_ToChar(int i)//Converts Integer to Characters
{
	char c;
	switch (i)
	{
		case 0:
			c='0';
		case 1:
			c='1';
		case 2:
			c='2';
		case 3:
			c='3';
		case 4:
			c='4';
		case 5:
			c='5';
		case 6:
			c='6';
		case 7:
			c='7';
		case 8:
			c='8';
		case 9:
			c='9';
		case 10:
			c='A';
		case 11:
			c='B';
		case 12:
			c='C';
		case 13:
			c='D';
		case 14:
			c='E';
		case 15:
			c='F';
			
	}
	return c;
}
int Char_ToInt(char c)//Converts Characters to Integers
{
	int i;
	switch (c)
	{
		case '0':
			i=0;
		case '1':
			i=1;
		case '2':
			i=2;
		case '3':
			i=3;
		case '4':
			i=4;
		case '5':
			i=5;
		case '6':
			i=6;
		case '7':
			i=7;
		case '8':
			i=8;
		case '9':
			i=9;
		case 'A':
			i=10;
		case 'B':
			i=11;
		case 'C':
			i=12;
		case 'D':
			i=13;
		case 'E':
			i=14;
		case 'F':
			i=15;	
	}
	return i;
}

void initializer()
{
	delay(8);			//16ms
	PTC->PDOR=0x30;
	CMD_WR();
	delay(3);			//6ms
	PTC->PDOR=0x30;
	CMD_WR();
	delay(1);			//2ms
	PTC->PDOR=0x30;
	CMD_WR();
	
		
	PTC->PDOR=0x38;		//N"number of lines" F"5x7dots"
	CMD_WR();
	PTC->PDOR=0x08;		//Display off
	CMD_WR();
	PTC->PDOR=0x01;		//Clear Display
	CMD_WR();
	PTC->PDOR=0x06;		//Entry mode set Shifts to the right
	CMD_WR();
	PTC->PDOR=0x0F;		//Turn on display
	CMD_WR();
	
}
void CMD_WR()		//When you are initializing
{
	int b=1;
	PTD->PDOR=0x0;
	delay(b);			//2ms
	PTD->PDOR=0x4;
	delay(b);
	PTD->PDOR=0x0;
	delay(b);
}

void DATA_WR()		//Any time you need to write something
{
	int b=1;
	PTD->PDOR=0x1;
	delay(b);			//2ms
	PTD->PDOR=0x5;
	delay(b);
	PTD->PDOR=0x1;
	delay(b);
} 

void UART0_initT(void) 
	{
		SIM->SCGC4 |= 0x0400; /* enable clock for UART0 */
		SIM->SOPT2 |= 0x04000000; /* use FLL output for UART Baud rate generator */
		UART0->C2 = 0; /* turn off UART0 while changing configurations */
		UART0->BDH = 0x00;
		UART0->BDL = 0x17; /* 115200 Baud */
		UART0->C4 = 0x0F; /* Over Sampling Ratio 16 */
		UART0->C1 = 0x00; /* 8-bit data */
		UART0->C2 = 0x08; /* enable transmit */
		SIM->SCGC5 |= 0x0200; /* enable clock for PORTA */
		PORTA->PCR[2] = 0x0200; /* make PTA2 UART0_Tx pin */
}
	void UART0_initR(void)
{
		SIM->SCGC4 |= 0x0400; /* enable clock for UART0 */
		SIM->SOPT2 |= 0x04000000; /* use FLL output for UART Baud rate generator */
		UART0->C2 = 0; /* turn off UART0 while changing configurations */
		UART0->BDH = 0x00;
		UART0->BDL = 0x17; /* 115200 Baud */
		UART0->C4 = 0x0F; /* Over Sampling Ratio 16 */
		UART0->C1 = 0x00; /* 8-bit data */
		UART0->C2 = 0x04; /* enable receive */
		SIM->SCGC5 |= 0x0200; /* enable clock for PORTA */
		PORTA->PCR[1] = 0x0200; /* make PTA1 UART0_Rx pin */
}