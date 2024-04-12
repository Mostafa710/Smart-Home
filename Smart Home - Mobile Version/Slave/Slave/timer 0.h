#ifndef TIMER_0_H_
#define TIMER_0_H_

void timer0_CTC_init_interrupt(void);
void timer0_wave_non_PWM(void);
void timer0_wave_fast_PWM(void);
void timer0_wave_phase_correct_PWM(void);
void timer0_stop(void);

#endif