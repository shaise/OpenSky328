/*
    hal_motor.cpp : handle dc motor pwm output

    Copyright 2020 shaise <AT> gmail dot com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http:// www.gnu.org/licenses/>.

    author: shaise <AT> gmail dot com
*/


#include <Arduino.h>
extern "C" {
    #include "hal_timeout.h"
    #include "hal_motor.h"
}

#define TMR2_PRESCALE1    1
#define TMR2_PRESCALE8    2
#define TMR2_PRESCALE32   3
#define TMR2_PRESCALE64   4
#define TMR2_PRESCALE128  5
#define TMR2_PRESCALE256  6
#define TMR2_PRESCALE1024 7

#define TMR2_OVERFLOW_INT 1
#define TMR2_OUTCOMPA_INT 2
#define TMR2_OUTCOMPB_INT 4
#define TMR2_CTC_MODE     2

void hal_timeout_init(void)
{
  cli();//stop interrupts

  //set timer2 interrupt at 10kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 10khz increments
#if F_CPU == 8000000
  OCR2A =  99;// = (8*10^6) / (10000*8) - 1 (must be <256)
#else // 16000000
  OCR2A = 199;// = (16*10^6) / (10000*8) - 1 (must be <256)
#endif
  // turn on CTC mode
  TCCR2A |= TMR2_CTC_MODE;
  // Set CS21 bit for 8 prescaler
  TCCR2B |= TMR2_PRESCALE8;   

  sei();//allow interrupts

}//end setup


// timeout zone
volatile uint8_t timeout1_end = 1;
volatile uint8_t timeout2_end = 1;
uint16_t timeout1_count, timeout2_count;


void hal_timeout_set(uint16_t ms)
{
    hal_timeout_set_100us(ms * 10);
}

void hal_timeout_set_100us(uint16_t hus)
{
    timeout1_end = 1;
    if (hus == 0)
        return;
    timeout1_count = hus;
    timeout1_end = 0;
    // enable timer compare interrupt (if not enabled)
    TIMSK2 |= TMR2_OUTCOMPA_INT;
}

//void hal_timeout_delay_ms(uint16_t timeout);
void hal_timeout_delay_ms(uint16_t timeout) {
    hal_timeout_set_100us(timeout * 10);
    while (!timeout1_end) {}
}

void hal_timeout2_set(uint16_t ms)
{
    hal_timeout2_set_100us(ms * 10);
}

void hal_timeout2_set_100us(uint16_t hus)
{
    timeout2_end = 1;
     if (hus == 0)
        return;
    timeout2_count = hus;
    timeout2_end = 0;
    // enable timer compare interrupt (if not enabled)
    TIMSK2 |= TMR2_OUTCOMPA_INT;
}

uint16_t hal_timeout_time_remaining()
{
    cli();
    volatile uint16_t res = timeout1_count;
    sei();
    return res;
}

// motor zone
#ifdef MOTOR_ENABLED
void hal_motor_init()
{
    HAL_MOTOR_PORT &= ~HAL_MOTOR_PORT_MASK; // make sure motor starts off
    HAL_MOTOR_PORT_DIR |= HAL_MOTOR_PORT_MASK;
}

static uint8_t motor_active = 0;

void hal_motor_set_power(uint8_t pow)
{
    if (pow < 2)
    {
        motor_active = 0;
        hal_motor_off();
        OCR2B = 250;
    } 
    else if (pow > 98)
    {
        motor_active = 0;
        hal_motor_on();
        OCR2B = 250;
    }
    else
    {
        OCR2B = pow * 2 - 1;
        motor_active = 1;
        // enable timer compare interrupt (if not enabled)
        TIMSK2 |= TMR2_OUTCOMPA_INT | TMR2_OUTCOMPB_INT;
    }
}

ISR(TIMER2_COMPB_vect)
{
    if (motor_active)
    {
        hal_motor_off();
    }
}
#endif


//void SysTick_Handler(void);

ISR(TIMER2_COMPA_vect)
{
#ifdef MOTOR_ENABLED
    if (motor_active)
    {
        hal_motor_on();
    }
#endif
    if (!timeout1_end)
    {
        timeout1_count--;
        if (timeout1_count == 0)
            timeout1_end = 1;
    }

    if (!timeout2_end)
    {
        timeout2_count--;
        if (timeout2_count == 0) {
            timeout2_end = 1;
        }
    }

#ifdef MOTOR_ENABLED
    if (timeout1_end && timeout2_end && !motor_active)
#else
    if (timeout1_end && timeout2_end)
#endif
    {
        // diable further timer compare interrupt
        TIMSK2 &= ~(TMR2_OUTCOMPA_INT | TMR2_OUTCOMPB_INT);
    }
}


