#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Master_config.h"

volatile unsigned char flag1,flag2,flag3,flag4;
volatile unsigned char timeout_source;
volatile unsigned short timeout_counter;

int main(void)
{
	/* Some Initializations */
	unsigned char arr[4];
	unsigned char led_status,tv_status;
	unsigned char aircond_status,temp;
	unsigned char tries;
	unsigned char val,i;
	SPI_Master_init();
	LED_init('C',0);
	LED_init('C',1);
	LED_init('C',2);
	LCD_init();
	/* Starting the program */
	LCD_send_string("Welcome to Smart");
	LCD_move_cursor(2,1);
	LCD_send_string("Home System");
	/* Delay for connecting HC-05 */
	_delay_ms(5000);
	USART_init(9600);
    while(1)
    {
		/* Set admin's and guest's passwords at first time */
		if (EEPROM_read(PASS_STATUS) == 0xFF)
		{
			LCD_clear_screen();
			LCD_send_string("Login for first");
			LCD_move_cursor(2,1);
			LCD_send_string("time");
			_delay_ms(1000);
			LCD_clear_screen();
			/* Entering admin's password */
			LCD_send_string("Set Admin Pass");
			LCD_move_cursor(2,1);
			LCD_send_string("Admin Pass:");
			for (i=0 ; i<=3 ; i++)
			{
				val = USART_receive_data();
				LCD_send_char(val);
				EEPROM_write(ADMIN_PASS_LOCATION+i,val);
				_delay_ms(500);
				LCD_move_cursor(2,12+i);
				LCD_send_char('*');
			}
			LCD_clear_screen();
			LCD_send_string("Password is");
			LCD_move_cursor(2,1);
			LCD_send_string("saved");
			_delay_ms(1000);
			LCD_clear_screen();
			/* Entering guest's password */
			LCD_send_string("Set Guest Pass");
			LCD_move_cursor(2,1);
			LCD_send_string("Guest Pass:");
			for (i=0 ; i<=3 ; i++)
			{
				val = USART_receive_data();
				LCD_send_char(val);
				EEPROM_write(GUEST_PASS_LOCATION+i,val);
				_delay_ms(500);
				LCD_move_cursor(2,12+i);
				LCD_send_char('*');
			}
			LCD_clear_screen();
			LCD_send_string("Password is");
			LCD_move_cursor(2,1);
			LCD_send_string("saved");
			_delay_ms(1000);
			/* Writing in PASS_STATUS location */
			EEPROM_write(PASS_STATUS,0x00);
		}
		/* Check BLock Mode */
		if (EEPROM_read(BLOCKED_LOCATION) != 0xFF)
		{
			LED_on('C',0);
			LCD_clear_screen();
			LCD_send_string("Login blocked");
			_delay_ms(1000);
			LCD_clear_screen();
			LCD_send_string("Try again after");
			LCD_move_cursor(2,1);
			LCD_send_string("20 seconds");
			for (i=19 ; i>0 ; i--)
			{
				_delay_ms(1000);
				if(i >= 10)
				{
					LCD_move_cursor(2,1);
					LCD_send_char(i/10+48);
					LCD_send_char(i%10+48);
				}
				else
				{
					LCD_move_cursor(2,1);
					LCD_send_char(i%10+48);
					LCD_send_char(' ');
				}
			}
			_delay_ms(1000);
			LED_off('C',0);
			EEPROM_write(BLOCKED_LOCATION,0xFF);
		}
		/* Selecting mode and check password */
		tries = TRIES_ALLOWED;
		LCD_clear_screen();
		LCD_send_string("Select Mode");
		LCD_move_cursor(2,1);
		LCD_send_string("1:Admin  2:Guest");
		val = USART_receive_data();
		if (val == '1')
		{
			flag1 = 1;
			while (flag1 == 1)
			{
				LCD_clear_screen();
				LCD_send_string("Admin Mode");
				LCD_move_cursor(2,1);
				LCD_send_string("Enter Pass:");
				for (i=0 ; i<=3 ; i++)
				{
					val = USART_receive_data();
					LCD_send_char(val);
					arr[i] = val;
					_delay_ms(500);
					LCD_move_cursor(2,12+i);
					LCD_send_char('*');
				}
				if ( (arr[0]==EEPROM_read(ADMIN_PASS_LOCATION)) && (arr[1]==EEPROM_read(ADMIN_PASS_LOCATION+1)) && (arr[2]==EEPROM_read(ADMIN_PASS_LOCATION+2)) && (arr[3]==EEPROM_read(ADMIN_PASS_LOCATION+3)) )
				{
					
					LED_on('C',1);
					LCD_clear_screen();
					LCD_send_string("Right Password");
					LCD_move_cursor(2,1);
					LCD_send_string("Admin Mode");
					_delay_ms(1000);
					timeout_source = 'A';
					timer0_CTC_init_interrupt();
					flag2 = 1;
					while (flag2 == 1)
					{
						LCD_clear_screen();
						LCD_send_string("1:Room1  2:Room2");
						LCD_move_cursor(2,1);
						LCD_send_string("3:Room3  4:More");
						val = USART_receive_data();
						if(flag2 == 0)
						{
							val = NOT_PRESSED;
						}
						if ( val == '1' || val == '2' || val == '3' )
						{
							switch (val)
							{
								case '1':
								SPI_Master_transmit('M');
								SPI_Master_transmit('1');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room1   S:ON");
									}
									else
									{
										LCD_send_string("Room1   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('1');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('1');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if(val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
								case '2':
								SPI_Master_transmit('M');
								SPI_Master_transmit('2');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room2   S:ON");
									}
									else
									{
										LCD_send_string("Room2   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('2');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('2');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if(val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
								case '3':
								SPI_Master_transmit('M');
								SPI_Master_transmit('3');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room3   S:ON");
									}
									else
									{
										LCD_send_string("Room3   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('3');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('3');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if(val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
							}
						}
						else if (val == '4')
						{
							LCD_clear_screen();
							LCD_send_string("1:Room4     2:TV");
							LCD_move_cursor(2,1);
							LCD_send_string("3:Air Cond.4:RET");
							val = USART_receive_data();
							if(flag2 == 0)
							{
								val = NOT_PRESSED;
							}
							if ( val == '1' || val == '2' || val == '3' )
							{
								switch (val)
								{
									case '1':
									SPI_Master_transmit('M');
									SPI_Master_transmit('4');
									SPI_Master_transmit('R');
									led_status = SPI_Master_transmit(DUMMY_DATA);
									flag3 = 1;
									while (flag3 == 1)
									{
										LCD_clear_screen();
										if(led_status == 1)
										{
											LCD_send_string("Room4   S:ON");
										}
										else
										{
											LCD_send_string("Room4   S:OFF");
										}
										LCD_move_cursor(2,1);
										LCD_send_string("1-ON 2-OFF 0-RET");
										val = USART_receive_data();
										if(flag3 == 0)
										{
											val = NOT_PRESSED;
										}
										if ( val == '0' || val == '1' || val == '2' )
										{
											flag3 = 0;
											switch (val)
											{
												case '0':
												break;
												case '1':
												SPI_Master_transmit('M');
												SPI_Master_transmit('4');
												SPI_Master_transmit('W');
												SPI_Master_transmit('1');
												break;
												case '2':
												SPI_Master_transmit('M');
												SPI_Master_transmit('4');
												SPI_Master_transmit('W');
												SPI_Master_transmit('2');
												break;
											}
										}
										else if(val != NOT_PRESSED)
										{
											LCD_clear_screen();
											LCD_send_string("Wrong Choice");
											_delay_ms(1000);
										}
									}
									break;
									case '2':
									SPI_Master_transmit('T');
									SPI_Master_transmit('R');
									tv_status = SPI_Master_transmit(DUMMY_DATA);
									flag3 = 1;
									while (flag3 == 1)
									{
										LCD_clear_screen();
										if(tv_status == 1)
										{
											LCD_send_string("TV      S:ON");
										}
										else
										{
											LCD_send_string("TV      S:OFF");
										}
										LCD_move_cursor(2,1);
										LCD_send_string("1-ON 2-OFF 0-RET");
										val = USART_receive_data();
										if(flag3 == 0)
										{
											val = NOT_PRESSED;
										}
										if ( val == '0' || val == '1' || val == '2' )
										{
											flag3 = 0;
											switch (val)
											{
												case '0':
												break;
												case '1':
												SPI_Master_transmit('T');
												SPI_Master_transmit('W');
												SPI_Master_transmit('1');
												break;
												case '2':
												SPI_Master_transmit('T');
												SPI_Master_transmit('W');
												SPI_Master_transmit('2');
												break;
											}
										}
										else if(val != NOT_PRESSED)
										{
											LCD_clear_screen();
											LCD_send_string("Wrong Choice");
											_delay_ms(1000);
										}
									}
									break;
									case '3':
									flag3 = 1;
									while (flag3 == 1)
									{
										LCD_clear_screen();
										LCD_send_string("1:Set Temp.");
										LCD_move_cursor(2,1);
										LCD_send_string("2:Control  0:RET");
										val = USART_receive_data();
										if(flag3 == 0)
										{
											val = NOT_PRESSED;
										}
										if ( val == '0' || val == '1' || val == '2' )
										{
											switch (val)
											{
												case '0':
												flag3 = 0;
												break;
												case '1':
												LCD_clear_screen();
												LCD_send_string("Set Temp.:--");
												LCD_send_char(0xDF);
												LCD_send_char('C');
												LCD_move_cursor(1,11);
												val = USART_receive_data();
												if(flag3 == 0)
												{
													val = NOT_PRESSED;
												}
												else
												{
													LCD_send_char(val);
												}
												if(val != NOT_PRESSED)
												{
													temp = (val - 48)*10;
													val = USART_receive_data();
													if(flag3 == 0)
													{
														val = NOT_PRESSED;
													}
													else
													{
														LCD_send_char(val);
													}
													_delay_ms(300);
													if(val != NOT_PRESSED)
													{
														temp += (val - 48);
														SPI_Master_transmit('A');
														SPI_Master_transmit('S');
														SPI_Master_transmit(temp/10);
														SPI_Master_transmit(temp%10);
														LCD_clear_screen();
														LCD_send_string("Temperature Sent");
														_delay_ms(1000);
													}
												}
												break;
												case '2':
												SPI_Master_transmit('A');
												SPI_Master_transmit('C');
												SPI_Master_transmit('R');
												aircond_status = SPI_Master_transmit(DUMMY_DATA);
												flag4 = 1;
												while (flag4 == 1)
												{
													LCD_clear_screen();
													if(aircond_status == 1)
													{
														LCD_send_string("Air Cond.   S:ON");
													}
													else
													{
														LCD_send_string("Air Cond.  S:OFF");
													}
													LCD_move_cursor(2,1);
													LCD_send_string("1-ON 2-OFF 0-RET");
													val = USART_receive_data();
													if(flag4 == 0)
													{
														val = NOT_PRESSED;
													}
													if ( val == '0' || val == '1' || val == '2' )
													{
														flag4 = 0;
														switch(val)
														{
															case '0':
															break;
															case '1':
															SPI_Master_transmit('A');
															SPI_Master_transmit('C');
															SPI_Master_transmit('W');
															SPI_Master_transmit('1');
															break;
															case '2':
															SPI_Master_transmit('A');
															SPI_Master_transmit('C');
															SPI_Master_transmit('W');
															SPI_Master_transmit('2');
															break;
														}
													}
													else if(val != NOT_PRESSED)
													{
														LCD_clear_screen();
														LCD_send_string("Wrong Choice");
														_delay_ms(1000);
													}
												}
												break;
											}
										}
										else if(val != NOT_PRESSED)
										{
											LCD_clear_screen();
											LCD_send_string("Wrong Choice");
											_delay_ms(1000);
										}
									}
									break;
								}
							}
							else if( (val != NOT_PRESSED) && (val != '4') )
							{
								LCD_clear_screen();
								LCD_send_string("Wrong Choice");
								_delay_ms(1000);
							}
						}
						else if(val != NOT_PRESSED)
						{
							LCD_clear_screen();
							LCD_send_string("Wrong Choice");
							_delay_ms(1000);
						}
					}
				}
				else
				{
					tries--;
					if(tries == 0)
					{
						flag1 = 0;
						EEPROM_write(BLOCKED_LOCATION,0x00);
					}
					else
					{
						LCD_clear_screen();
						LCD_send_string("Wrong Password");
						LCD_move_cursor(2,1);
						LCD_send_string("Tries Left:");
						LCD_send_char(tries+48);
						_delay_ms(1000);
					}
				}
			}
		}
		else if (val == '2')
		{
			flag1 = 1;
			while (flag1 == 1)
			{
				LCD_clear_screen();
				LCD_send_string("Guest Mode");
				LCD_move_cursor(2,1);
				LCD_send_string("Enter Pass:");
				for (i=0 ; i<=3 ; i++)
				{
					val = USART_receive_data();
					LCD_send_char(val);
					arr[i] = val;
					_delay_ms(500);
					LCD_move_cursor(2,12+i);
					LCD_send_char('*');
				}
				if ( (arr[0]==EEPROM_read(GUEST_PASS_LOCATION)) && (arr[1]==EEPROM_read(GUEST_PASS_LOCATION+1)) && (arr[2]==EEPROM_read(GUEST_PASS_LOCATION+2)) && (arr[3]==EEPROM_read(GUEST_PASS_LOCATION+3)) )
				{
					LED_on('C',2);
					LCD_clear_screen();
					LCD_send_string("Right Password");
					LCD_move_cursor(2,1);
					LCD_send_string("Guest Mode");
					_delay_ms(1000);
					timeout_source = 'G';
					timer0_CTC_init_interrupt();
					flag2 = 1;
					while (flag2 == 1)
					{
						LCD_clear_screen();
						LCD_send_string("1:Room1  2:Room2");
						LCD_move_cursor(2,1);
						LCD_send_string("3:Room3  4:Room4");
						val = USART_receive_data();
						if(flag2 == 0)
						{
							val = NOT_PRESSED;
						}
						if ( val == '1' || val == '2' || val == '3' || val == '4' )
						{
							switch (val)
							{
								case '1':
								SPI_Master_transmit('M');
								SPI_Master_transmit('1');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room1   S:ON");
									}
									else
									{
										LCD_send_string("Room1   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('1');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('1');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if (val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
								case '2':
								SPI_Master_transmit('M');
								SPI_Master_transmit('2');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room2   S:ON");
									}
									else
									{
										LCD_send_string("Room2   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('2');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('2');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if (val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
								case '3':
								SPI_Master_transmit('M');
								SPI_Master_transmit('3');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room3   S:ON");
									}
									else
									{
										LCD_send_string("Room3   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('3');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('3');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if (val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
								case '4':
								SPI_Master_transmit('M');
								SPI_Master_transmit('4');
								SPI_Master_transmit('R');
								led_status = SPI_Master_transmit(DUMMY_DATA);
								flag3 = 1;
								while (flag3 == 1)
								{
									LCD_clear_screen();
									if(led_status == 1)
									{
										LCD_send_string("Room4   S:ON");
									}
									else
									{
										LCD_send_string("Room4   S:OFF");
									}
									LCD_move_cursor(2,1);
									LCD_send_string("1-ON 2-OFF 0-RET");
									val = USART_receive_data();
									if(flag3 == 0)
									{
										val = NOT_PRESSED;
									}
									if ( val == '0' || val == '1' || val == '2' )
									{
										flag3 = 0;
										switch (val)
										{
											case '0':
											break;
											case '1':
											SPI_Master_transmit('M');
											SPI_Master_transmit('4');
											SPI_Master_transmit('W');
											SPI_Master_transmit('1');
											break;
											case '2':
											SPI_Master_transmit('M');
											SPI_Master_transmit('4');
											SPI_Master_transmit('W');
											SPI_Master_transmit('2');
											break;
										}
									}
									else if (val != NOT_PRESSED)
									{
										LCD_clear_screen();
										LCD_send_string("Wrong Choice");
										_delay_ms(1000);
									}
								}
								break;
							}
						}
						else if(val != NOT_PRESSED)
						{
							LCD_clear_screen();
							LCD_send_string("Wrong Choice");
							_delay_ms(1000);
						}
					}
				}
				else
				{
					tries--;
					if (tries == 0)
					{
						flag1 = 0;
						EEPROM_write(BLOCKED_LOCATION,0x00);
					}
					else
					{
						LCD_clear_screen();
						LCD_send_string("Wrong Password");
						LCD_move_cursor(2,1);
						LCD_send_string("Tries Left:");
						LCD_send_char(tries+48);
						_delay_ms(1000);
					}
				}
			}
		}
		else
		{
			LCD_clear_screen();
			LCD_send_string("Wrong Choice");
			_delay_ms(1000);
		}
    }
}

ISR(TIMER0_COMP_vect)
{
	timeout_counter++;
	switch (timeout_source)
	{
		case 'A':
		if(timeout_counter == ADMIN_TIMEOUT)
		{
			LED_off('C',1);
			timeout_counter = 0;
			flag1 = flag2 = flag3 = flag4 = 0;
			CLR_BIT(TCCR0,CS00);
			CLR_BIT(TCCR0,CS02);
			LCD_clear_screen();
			LCD_send_string("Session Timeout");
			_delay_ms(1000);
		}
		break;
		case 'G':
		if (timeout_counter == GUEST_TIMEOUT)
		{
			LED_off('C',2);
			timeout_counter = 0;
			flag1 = flag2 = flag3 = 0;
			CLR_BIT(TCCR0,CS00);
			CLR_BIT(TCCR0,CS02);
			LCD_clear_screen();
			LCD_send_string("Session Timeout");
			_delay_ms(1000);
		}
		break;
	}
}