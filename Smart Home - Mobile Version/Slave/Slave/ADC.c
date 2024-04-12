#include <avr/io.h>
#include "std_macros.h"

void ADC_init()
{
	//Assuming channel A0 (ADC0)
	SET_BIT(ADMUX,REFS0);
	SET_BIT(ADMUX,REFS1);
	SET_BIT(ADCSRA,ADEN);
	SET_BIT(ADCSRA,ADPS1);
	SET_BIT(ADCSRA,ADPS2);
}
unsigned short ADC_read()
{
	unsigned short val;
	SET_BIT(ADCSRA,ADSC);
	while(READ_BIT(ADCSRA,ADSC)==1);
	/* while(READ_BIT(ADCSRA,ADIF)==0);
	   SET_BIT(ADCSRA,ADIF); */
	val= ADCL;
	val|=(ADCH<<8);
	return val;
}