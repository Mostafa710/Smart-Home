#ifndef MASTER_CONFIG_H_
#define MASTER_CONFIG_H_

/* Include header files */
#include "std_macros.h"
#include "timer 0.h"
#include "EEPROM.h"
#include "USART.h"
#include "LED.h"
#include "LCD.h"
#include "SPI.h"

/* Maximum number of tries */
#define TRIES_ALLOWED 3

/* EEPROM definitions */
#define BLOCKED_LOCATION	0x10
#define PASS_STATUS         0x100
#define ADMIN_PASS_LOCATION 0x101
#define GUEST_PASS_LOCATION 0x105

/* Session timeout ticks */
#define ADMIN_TIMEOUT 12000
#define GUEST_TIMEOUT 6000

/* SPI dummy data */
#define DUMMY_DATA 0xFF
#define NOT_PRESSED 0xFF

#endif