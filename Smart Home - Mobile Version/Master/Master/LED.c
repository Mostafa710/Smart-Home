#include "DIO.h"

void LED_init(unsigned char portname,unsigned char pinnumber)
{
	DIO_setPINDir(portname,pinnumber,1);
}
void LED_on(unsigned char portname,unsigned char pinnumber)
{
	DIO_writePIN(portname,pinnumber,1);
}
void LED_off(unsigned char portname,unsigned char pinnumber)
{
	DIO_writePIN(portname,pinnumber,0);
}
void LED_toggle(unsigned char portname,unsigned char pinnumber)
{
	DIO_togglePIN(portname,pinnumber);
}
unsigned char LED_readstatus(unsigned char portname,unsigned char pinnumber)
{
	return DIO_readPIN(portname,pinnumber);
}