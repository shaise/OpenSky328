extern "C" {
  #include "hal_pwm.h"
}
#include <Arduino.h>
/*#define TCCR1A  _SFR_MEM8 (0x80)
#define TCCR1B  _SFR_MEM8 (0x81)
#define OCR1A   _SFR_MEM16 (0x88)
#define OCR1B   _SFR_MEM16 (0x8A)
#define TIMSK1  _SFR_MEM8 (0x6F)

#define ICNC1   7
#define ICES1   6
#define WGM13   4
#define WGM12   3
*/

/*
* and to enable the timer, you do stuff like:
*/
#define TMR01_PRESCALE1    1
#define TMR01_PRESCALE8    2
#define TMR01_PRESCALE64   3
#define TMR01_PRESCALE256  4
#define TMR01_PRESCALE1024 5

#define TMR1_OVERFLOW_INT 1
#define TMR1_OUTCOMPA_INT 2
#define TMR1_OUTCOMPB_INT 4

#define TMR1_CTC_OCR1A    8


void hal_pwm_init(){

  cli();//stop interrupts

  //set timer1 to 2MHz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 20ms interval
  #if F_CPU == 8000000
  OCR1A = 20000; // = 20000 * 1us timer period (1MHz)
  #elif F_CPU == 16000000
  OCR1A = 40000; // = 40000 * 0.5us timer period (2MHz)
  #endif
  // turn on CTC mode
  TCCR1B |= TMR1_CTC_OCR1A;
  TCCR1B |= TMR01_PRESCALE8;
  // enable timer compare interrupt
  TIMSK1 |= TMR1_OUTCOMPA_INT | TMR1_OUTCOMPB_INT;
#ifdef FR_OLD
  DDRD |= B11111100; // portd pwm pins set to output
  PORTD &= B00000011;
  DDRB |= B00000011; // portb pwm pins set to output
  PORTB &= B11111100;
#else
  DDRD |= B11000000; // portd pwm pins set to output
  PORTD &= B00111111;
  DDRB |= B00111111; // portb pwm pins set to output
  PORTB &= B11000000;
#endif // FR_OLD


  millis();
  sei();//allow interrupts

}//end setup


void hal_pwm_failsafe_exit(void)
{
  
}

void hal_pwm_failsafe_enter(void)
{
  
}
