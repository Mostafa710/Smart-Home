#ifndef LED_H_
#define LED_H_

void LED_init(unsigned char portname,unsigned char pinnumber);
void LED_on(unsigned char portname,unsigned char pinnumber);
void LED_off(unsigned char portname,unsigned char pinnumber);
void LED_toggle(unsigned char portname,unsigned char pinnumber);
unsigned char LED_readstatus(unsigned char portname,unsigned char pinnumber);

#endif