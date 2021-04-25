/*
    hal_timeout.cpp : handle timeout counters and dc motor pwm output

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
extern "C"
{
#include "hal_timeout.h"
#include "hal_motor.h"
#include "debug.h"
#include "frsky.h"
}

#define TMR2_PRESCALE1 1
#define TMR2_PRESCALE8 2
#define TMR2_PRESCALE32 3
#define TMR2_PRESCALE64 4
#define TMR2_PRESCALE128 5
#define TMR2_PRESCALE256 6
#define TMR2_PRESCALE1024 7

#define TMR2_OVERFLOW_INT 1
#define TMR2_OUTCOMPA_INT 2
#define TMR2_OUTCOMPB_INT 4
#define TMR2_CTC_MODE 2

void hal_timeout_init(void)
{
    cli(); //stop interrupts

    //set timer2 interrupt at 10kHz
    TCCR2A = 0; // set entire TCCR2A register to 0
    TCCR2B = 0; // same for TCCR2B
    TCNT2 = 0;  //initialize counter value to 0
                // set compare match register for 10khz increments
#if F_CPU == 8000000
    OCR2A = 99; // = (8*10^6) / (10000*8) - 1 (must be <256)
#else           // 16000000
    OCR2A = 199; // = (16*10^6) / (10000*8) - 1 (must be <256)
#endif
    // turn on CTC mode
    TCCR2A |= TMR2_CTC_MODE;
    // Set CS21 bit for 8 prescaler
    TCCR2B |= TMR2_PRESCALE8;

    sei(); //allow interrupts

} //end setup

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
void hal_timeout_delay_ms(uint16_t timeout)
{
    hal_timeout_set_100us(timeout * 10);
    while (!timeout1_end)
    {
    }
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
#define FORWARD_MULTIPLIER (200 / (100 - MOTOR_ZERO))
#define BACKWARD_MULTIPLIER (200 / MOTOR_BREAK)
uint8_t motor_pins_mask[] = HAL_MOTOR_PORT_MASKS;
static uint8_t motor_active = 0;
static uint8_t motor_set_busy = 0;
// motor data: [m1 pwmval][m1 mask][m2 pwmval][m2 mask]....[mn pwmval][mn mask]
static uint8_t motor_data[2 * (MOTOR_COUNT + 1)];    // add one dummy data as end marker
static uint8_t motor_data_buffered[2 * MOTOR_COUNT]; // add one dummy data as end marker
static uint8_t *motor_data_ptr;
static uint8_t motor_data_count;
static uint8_t all_motor_pins_mask;

void hal_motor_init()
{
    all_motor_pins_mask = 0;
    for (uint8_t i = 0; i < sizeof(motor_pins_mask); i++)
        all_motor_pins_mask |= motor_pins_mask[i];
    HAL_MOTOR_PORT_DIR |= all_motor_pins_mask;
    HAL_MOTOR_PORT &= ~all_motor_pins_mask; // make sure motor starts off
    // set dummy end of data
    motor_data[0] = 250;
    motor_data[MOTOR_COUNT * 2] = 250;
    motor_data[MOTOR_COUNT * 2 + 1] = 0;
}

#define MINIMUM_PWM_SPACING 10
#define MINIMUM_PWM_VALUE 20

// insert all motor values sorted
static void insert_sorted(uint8_t pwmval, uint8_t motor_mask)
{
    uint8_t tval;
    uint8_t *data_ptr = motor_data_buffered;

    for (uint8_t i = 0; i < motor_data_count; i++)
    {
        tval = *data_ptr;
        if (pwmval < tval)
        {
            *data_ptr = pwmval;
            pwmval = tval;
            data_ptr++;
            tval = *data_ptr;
            *data_ptr = motor_mask;
            motor_mask = tval;
            data_ptr++;
        }
        else
            data_ptr += 2;
    }
    *data_ptr = pwmval;
    data_ptr++;
    *data_ptr = motor_mask;
    motor_data_count++;
}

// make sure we have minimum space between interrupts, otherwise interrupts can be missed
static void ensure_period_spacing()
{
    uint8_t *data_ptr = motor_data_buffered;
    uint8_t tval = MINIMUM_PWM_VALUE;
    for (uint8_t i = 0; i < motor_data_count; i++)
    {
        uint8_t pwmval = *data_ptr;
        if (pwmval >= 250)
            break;
        if (pwmval < tval)
        {
            *data_ptr = tval;
        }
        tval = pwmval + MINIMUM_PWM_SPACING;
        data_ptr += 2;
    }
}

static uint8_t old_mode = 0;

void print_buffer(void)
{
    debug_putc('(');
    debug_putc('0' + old_mode);
    for (uint8_t i = 0; i < MOTOR_COUNT * 2; i++)
    {
        debug_putc('-');
        debug_put_hex8(motor_data_buffered[i]);
    }
    debug_putc(')');
}

static uint8_t print_buff = 0;

void hal_motor_set_power(uint8_t *pow_ptr)
{
    uint8_t pwmval;
    print_buff = 0;
    if (conn_lost)
    {
        HAL_MOTOR_PORT &= ~all_motor_pins_mask;
        return;
    }
    if (motor_set_busy)
    {
        //debug_putc('B');
        return;
    }
    motor_active = 0;
    motor_data_count = 0;
#ifdef MOTOR_BIDIR
    for (uint8_t i = 0; i < MOTOR_COUNT * 2; i += 2, pow_ptr++)
    {
        uint8_t pow = *pow_ptr;
        uint8_t mask1 = motor_pins_mask[i];
        uint8_t mask2 = motor_pins_mask[i + 1];
        uint8_t pwm_mask = 0;
        pwmval = 250;
        if (pow > 98)
        {
            //mode = 5;
            HAL_MOTOR_PORT &= ~mask2;
            HAL_MOTOR_PORT |= mask1;
        }
        else if (pow > MOTOR_ZERO)
        {
            //mode = 4;
            HAL_MOTOR_PORT &= ~mask2;
            pwm_mask = mask1;
            pwmval = (pow - MOTOR_ZERO) * FORWARD_MULTIPLIER - 1;
            motor_active = 1;
        }
        else if (pow > MOTOR_FREERUN)
        {
            //mode = 3;
            HAL_MOTOR_PORT &= ~(mask2 | mask1);
        }
        else if (pow > MOTOR_BREAK)
        {
            //mode = 2;
            HAL_MOTOR_PORT |= (mask2 | mask1);
        }
        else if (pow > 2)
        {
            //mode = 1;
            HAL_MOTOR_PORT &= ~(mask2 | mask1);
            pwm_mask = mask2;
            pwmval = (MOTOR_BREAK - pow) * BACKWARD_MULTIPLIER - 1;
            motor_active = 1;
        }
        else
        {
            //mode = 0;
            HAL_MOTOR_PORT &= ~mask1;
            HAL_MOTOR_PORT |= mask2;
        }
        insert_sorted(pwmval, pwm_mask);
    }
    if (print_buff)
    {
        print_buffer();
    }
#else
    for (uint8_t i = 0; i < MOTOR_COUNT; i++, pow_ptr++)
    {
        uint8_t pow = *pow_ptr;
        pwmval = 250;
        if (pow < 2)
        {
            HAL_MOTOR_PORT &= ~motor_pins_mask[i];
            //debug_putc('^');
        }
        else if (pow > 98)
        {
            HAL_MOTOR_PORT |= motor_pins_mask[i];
            //debug_putc('=');
        }
        else
        {
            pwmval = pow * 2 - 1;
            motor_active = 1;
        }
        insert_sorted(pwmval, motor_pins_mask[i]);
    }
#endif
    if (motor_active)
    {
        motor_set_busy = 1;
        ensure_period_spacing();
        // enable timer compare interrupt (if not enabled)
        TIMSK2 |= TMR2_OUTCOMPA_INT | TMR2_OUTCOMPB_INT;
    }
}

// OCR2B = 250;
ISR(TIMER2_COMPB_vect)
{
    if (motor_active)
    {
        motor_data_ptr++;
        HAL_MOTOR_PORT &= ~(*motor_data_ptr);
        motor_data_ptr++;
        OCR2B = *motor_data_ptr;
    }
}
#endif

//void SysTick_Handler(void);

ISR(TIMER2_COMPA_vect)
{
#ifdef MOTOR_ENABLED
    if (motor_active)
    {
        uint8_t i;
        if (motor_set_busy)
        {
            // copy buffered data
            for (i = 0; i < (MOTOR_COUNT * 2); i++)
                motor_data[i] = motor_data_buffered[i];
            //debug_put_hex8(motor_data[0]);
            print_buff = 0;
            motor_set_busy = 0;
        }
        motor_data_ptr = motor_data;
        while (*motor_data_ptr < 250)
        {
            motor_data_ptr++;
            HAL_MOTOR_PORT |= *motor_data_ptr;
            motor_data_ptr++;
        }
        motor_data_ptr = motor_data;
        OCR2B = *motor_data_ptr;
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
        if (timeout2_count == 0)
        {
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
