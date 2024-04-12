#define F_CPU 8000000UL
#include <avr/interrupt.h>
#include "Slave_config.h"

/* Air Conditioner Variables */
volatile unsigned char counter;
volatile unsigned short set_temp=INITIAL_SET;
volatile unsigned short act_temp;

int main(void)
{
	SPI_Slave_init();
	ADC_init();
	LED_init('D',2);
	LED_init('D',3);
	LED_init('D',4);
	LED_init('D',5);
	LED_init('D',6);
	LED_init('C',0);
	/* Inputs to slave */
	unsigned char func,room_num,signal,temp_signal,order;
	/* Output from slave */
	unsigned char status;
    while(1)
    {	
		/* Receive instructions from master */
        func = SPI_Slave_receive(DUMMY_DATA);
		switch(func)
		{
			case 'M':
			room_num = SPI_Slave_receive(DUMMY_DATA);
			switch(room_num)
			{
				case '1':
				signal = SPI_Slave_receive(DUMMY_DATA);
				switch(signal)
				{
					case 'R':
					status = LED_readstatus('D',2);
					SPI_Slave_receive(status);
					break;
					case 'W':
					order = SPI_Slave_receive(DUMMY_DATA);
					switch(order)
					{
						case '1':
						LED_on('D',2);
						break;
						case '2':
						LED_off('D',2);
						break;
					}
					break;
				}
				break;
				case '2':
				signal = SPI_Slave_receive(DUMMY_DATA);
				switch(signal)
				{
					case 'R':
					status = LED_readstatus('D',3);
					SPI_Slave_receive(status);
					break;
					case 'W':
					order = SPI_Slave_receive(DUMMY_DATA);
					switch(order)
					{
						case '1':
						LED_on('D',3);
						break;
						case '2':
						LED_off('D',3);
						break;
					}
					break;
				}
				break;
				case '3':
				signal = SPI_Slave_receive(DUMMY_DATA);
				switch(signal)
				{
					case 'R':
					status = LED_readstatus('D',4);
					SPI_Slave_receive(status);
					break;
					case 'W':
					order = SPI_Slave_receive(DUMMY_DATA);
					switch(order)
					{
						case '1':
						LED_on('D',4);
						break;
						case '2':
						LED_off('D',4);
						break;
					}
					break;
				}
				break;
				case '4':
				signal = SPI_Slave_receive(DUMMY_DATA);
				switch(signal)
				{
					case 'R':
					status = LED_readstatus('D',5);
					SPI_Slave_receive(status);
					break;
					case 'W':
					order = SPI_Slave_receive(DUMMY_DATA);
					switch(order)
					{
						case '1':
						LED_on('D',5);
						break;
						case '2':
						LED_off('D',5);
						break;
					}
					break;
				}
				break;
			}
			break;
			case 'T':
			signal = SPI_Slave_receive(DUMMY_DATA);
			switch(signal)
			{
				case 'R':
				status = LED_readstatus('D',6);
				SPI_Slave_receive(status);
				break;
				case 'W':
				order = SPI_Slave_receive(DUMMY_DATA);
				switch(order)
				{
					case '1':
					LED_on('D',6);
					break;
					case '2':
					LED_off('D',6);
					break;
				}
				break;
			}
			break;
			case 'A':
			temp_signal = SPI_Slave_receive(DUMMY_DATA);
			switch(temp_signal)
			{
				case 'S':
				set_temp = (SPI_Slave_receive(DUMMY_DATA))*10;
				set_temp+= SPI_Slave_receive(DUMMY_DATA);
				break;
				case 'C':
				signal = SPI_Slave_receive(DUMMY_DATA);
				switch(signal)
				{
					case 'R':
					status = LED_readstatus('C',0);
					SPI_Slave_receive(status);
					break;
					case 'W':
					order = SPI_Slave_receive(DUMMY_DATA);
					switch(order)
					{
						case '1':
						timer0_CTC_init_interrupt();
						break;
						case '2':
						timer0_stop();
						LED_off('C',0);
						break;
					}
					break;
				}
				break;
			}
			break;
		}
    }
}

/* Perform operation of air conditioner (If it was turned ON by master) */
ISR(TIMER0_COMP_vect)
{
	counter++;
	if (counter == 10)
	{
		counter = 0;
		act_temp = 0.25 * ADC_read();
		if (act_temp > set_temp)
		{
			LED_on('C',0);
		}
		else if (act_temp < set_temp)
		{
			LED_off('C',0);
		}
	}
}