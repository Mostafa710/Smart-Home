#include <avr/interrupt.h>
#include "std_macros.h"

void timer0_CTC_init_interrupt(void)
{
	/* select CTC mode */
	SET_BIT(TCCR0,WGM01);
	/* load a value in OCR0 */
	OCR0=78;
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* enable interrupt */
	sei();
	SET_BIT(TIMSK,OCIE0);
}
void timer0_wave_non_PWM(void)
{
	/* set OC0 as output pin */
	SET_BIT(DDRB,3);
	/* select CTC mode */
	SET_BIT(TCCR0,WGM01);
	/* load a value in OCR0 */
	OCR0=64;
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* toggle OC0 on compare match */
	SET_BIT(TCCR0,COM00);
}
void timer0_wave_fast_PWM(void)
{
	/* set OC0 as output pin */
	SET_BIT(DDRB,3);
	/* select fast PWM mode */
	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,WGM01);
	/* load a value in OCR0 */
	OCR0=64;
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* clear OC0 on compare match, set OC0 at BOTTOM (non-inverting mode) */
	SET_BIT(TCCR0,COM01);
}
void timer0_wave_phase_correct_PWM(void)
{
	/* set OC0 as output pin */
	SET_BIT(DDRB,3);
	/* select phase correct PWM mode */
	SET_BIT(TCCR0,WGM00);
	/* load a value in OCR0 */
	OCR0=64;
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* set OC0 on compare match when up-counting, clear OC0 on compare match when down-counting */
	SET_BIT(TCCR0,COM00);
	SET_BIT(TCCR0,COM01);
}
void timer0_stop(void)
{
	TCCR0 = 0;
}