#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "std_macros.h"
#include "DIO.h"

void SPI_Master_init(void)
{
	/* Set SS, MOSI and SCK as output pins */
	DIO_setPINDir('B',4,1);
	DIO_setPINDir('B',5,1);
	DIO_setPINDir('B',7,1);
	/* Enable Master mode */
	SET_BIT(SPCR,MSTR);
	/* Set clock to Fosc/16 */
	SET_BIT(SPCR,SPR0);
	/* Enable SPI */
	SET_BIT(SPCR,SPE);
	/* Deactivate SS pin */
	DIO_writePIN('B',4,1);
}
void SPI_Slave_init(void)
{
	/* Set MISO as output pin */
	DIO_setPINDir('B',6,1);
	/* Enable SPI */
	SET_BIT(SPCR,SPE);
}
char SPI_Master_transmit(char Data)
{
	/* Activate SS pin */
	DIO_writePIN('B',4,0);
	/* Put data in SPDR */
	SPDR = Data;
	/* Wait until the transmission is finished */
	while(READ_BIT(SPSR,SPIF)==0);
	/* Read SPDR */
	return SPDR;
}
char SPI_Slave_receive(char Data)
{
	/* Put data in SPDR */
	SPDR = Data;
	/* Wait until the reception is finished */
	while(READ_BIT(SPSR,SPIF)==0);
	/* Read SPDR */
	return SPDR;
}
void SPI_Master_transmit_string(char *ptr)
{
	while(*ptr != 0)
	{
		SPI_Master_transmit(*ptr);
		ptr++;
		_delay_ms(100);
	}
}