#include <MKL25Z4.H>
void delayMs(int n);
void initialTimer(void);
void ADC0_init(void);
void UART0_initT(void); 
void UART0_initR(void);
int result;
int set;

int main (void) {
	
	int in=1;
initialTimer();
	
	PORTC->PCR[0]=0x100;//MainFan
	PORTC->PCR[1]=0x100;//Heat
	PORTC->PCR[2]=0x100;//Cool
	PORTC->PCR[3]=0x100;//AtticFan
PTC->PDDR=0xFFFFFFFF;
	PTC->PDOR=0x00;
ADC0_init(); /* Configure ADC0 */
	
	PTC->PDOR=0x1;//Main Fan on
	
	
	while (1) 
	{
			//SIM->SCGC5 = 0x3D80;

		/*if((result+3<set) || (in==1))
		{
			PTC->PDOR=0x3;	//Main Fan, Heat
			if (result+1>=set)
				in=0;
		}
	
			if((result < set) && (result > set-3))
			{
				PTC->PDOR=0x5;//Main Fan, Cold
			}
			
			
				if((result > set+3))
				{
					PTC->PDOR=0xD;//Main Fan, Cold, Attic Fan
				}
				
				
					if((result > set) && (result < set+3))
					{
						PTC->PDOR=0xD;//Main Fan, Cold, Attic Fan
					}
					
					
							if(result==set)
							{
								PTC->PDOR=0x0;//ALL OFF
							}*/
							delayMs(100);
					}
				}
				
			
		
	

void TPM0_IRQHandler(void) {

		int in=1;
		ADC0->SC1[0] = 0; /* start conversion on channel 0 */
		while(!(ADC0->SC1[0] & 0x80)) { } /* wait for COCO */
		result = ADC0->R[0]; /* read conversion result and clear COCO flag */
		result = result * 330.0 / 65536; /* convert voltage to temperature */
		
		
		result= (result*1.8)+32;//Farhrenheit conversion
		
		UART0_initR();
		//UART0->C2 = 0x04; /* enable receive */
		while(!(UART0->S1 & 0x20)) 
			{} /* wait for receive buffer full */
			set = UART0->D; /* read the char received */
			//-------------------------------------------------------
				
	if((result+3<set) || (in==1))
		{
			PTC->PDOR=0x3;	//Main Fan, Heat
			if (result+1>=set)
				in=0;
		}
		
			if((result < set) && (result > set-3))
			{
				PTC->PDOR=0x5;//Main Fan, Cold
			}
			
		
				if((result > set+3))
				{
					PTC->PDOR=0xD;//Main Fan, Cold, Attic Fan
				}
				
				
					if((result > set) && (result < set+3))
					{
						PTC->PDOR=0xD;//Main Fan, Cold, Attic Fan
					}
					
					
							if(result==set)
							{
								PTC->PDOR=0x0;//ALL OFF
							}
					
				
				
			
		
				
				//------------------------------------------------------
			UART0_initT();
				//UART0->C2 = 0x08; /* enable receive */
			while(!(UART0->S1 & 0x80)) { }
			UART0->D = result; /* send a char */
		//LED_set(c);
			delayMs(1); /* leave a gap between messages */
			
	
	delayMs(100);
	TPM0->SC |= 0x80; /* clear TOF */
}
void initialTimer()
{
				__disable_irq(); /* global disable IRQs */
				
			SIM->SCGC5 |= 0x400; /* enable clock to Port B */
			SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
				
			PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
			PTB->PDDR |= 0x40000; /* make PTB18 as output pin */
			PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
			PTB->PDDR |= 0x80000; /* make PTB19 as output pin */
				
			PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
			PTD->PDDR |= 0x02; /* make PTD1 as output pin */
				
			SIM->SOPT2 |= 0x01000000; /* use MCGFLLCLK as timer counter clock */
			SIM->SCGC6 |= 0x01000000; /* enable clock to TPM0 */
				
			TPM0->SC = 0; /* disable timer while configuring */
			TPM0->SC = 0x07; /* prescaler /128 */
			TPM0->MOD = 0xFFFF; /* max modulo value */
			TPM0->SC |= 0x80; /* clear TOF */
			TPM0->SC |= 0x40; /* enable timeout interrupt */
			TPM0->SC |= 0x08; /* enable timer */

			NVIC->ISER[0] |= 0x00020000; /* enable IRQ17 (bit 17 of ISER[0]) */

			SIM->SCGC6 |= 0x02000000; /* enable clock to TPM1 */

			NVIC->ISER[0] |= 0x00040000; /* enable IRQ18 (bit 18 of ISER[0]) */

			__enable_irq(); /* global enable IRQs */

			SIM->SCGC5|=0x3D80;
			PORTC->PCR[0]= 0x100;

			PTC->PDDR=0xFFFFFFFF;
}
/* Delay n milliseconds
* The CPU core clock is set to MCGFLLCLK at 41.94 MHz in SystemInit().
*/
void delayMs(int n) {
int i;
int j;
for(i = 0 ; i < n; i++)
for (j = 0; j < 7000; j++) {}
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
	void UART0_initR(void) {
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
void ADC0_init(void)
{
SIM->SCGC5 |= 0x2000; /* clock to PORTE */
PORTE->PCR[20] = 0; /* PTE20 analog input */
SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
ADC0->SC2 &= ~0x40; /* software trigger */
ADC0->SC3 |= 0x07; /* 32 samples average */
/* clock div by 4, long sample time, single ended 16 bit, bus clock */
ADC0->CFG1 = 0x40 | 0x10 | 0x0C | 0x00;
}