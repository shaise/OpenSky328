/*
    hal_pwm.h : handle up to 8 ch servo output

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

#ifndef HAL_PWM_H_
#define HAL_PWM_H_

#include "config.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void hal_pwm_init(void);

void hal_pwm_failsafe_exit(void);
void hal_pwm_failsafe_enter(void);

#ifdef FR_OLD
// counter runs with 1MHz = 1us resolution
#define HAL_PWM_US_TO_TICKCOUNT(us) ((us)-1)
#else
// counter runs with 2MHz = 0.5us resolution
#define HAL_PWM_US_TO_TICKCOUNT(us) (((us)*2) - 1)
#endif // FR_OLD

// from frsky to ticks coresponding to 500...2500 us
// frsky seems to send us*1.5 (~1480...3020)
// -> divide by 1.5 (=*2/3) to get us -> multiply by 2 to get ticks
#define HAL_PWM_FRSKY_TO_US(_frsky) ((_frsky)*2 / 3)
#define HAL_PWM_FRSKY_TO_TICKCOUNT(_frsky) (HAL_PWM_US_TO_TICKCOUNT(HAL_PWM_FRSKY_TO_US(_frsky)))

#if defined(RXBOARD_FROLD)
// pwm channels bit assignement: Port B: xxxxxx87, Port D: 654321xx
#define PORTD_PWM_BITS B11111100
#define PORTB_PWM_BITS B00000011
#define HAL_PWM_PORT_BITMASK(ch) ((uint16_t)0x4 << (ch))
#elif defined(RXBOARD_FR8_FRTINY)
#define PORTD_PWM_BITS B11000000
#define PORTB_PWM_BITS B00111111
// pwm channels bit assignement: Port B: xx876543, Port D: 21xxxxxx
#define HAL_PWM_PORT_BITMASK(ch) ((uint16_t)0x40 << (ch))
#elif defined(RXBOARD_FRLVH)
#define PORTD_PWM_BITS B00000000
#define PORTB_PWM_BITS B00110000
// pwm channels bit assignement: Port B: xx43xxxx, Port D: xxxxxxxx
#define HAL_PWM_PORT_BITMASK(ch) ((uint16_t)0x400 << (ch))
#elif defined(RXBOARD_FR4)
#define PORTD_PWM_BITS B00000000
#define PORTB_PWM_BITS B00111100
// pwm channels bit assignement: Port B: xx4321xx, Port D: xxxxxxxx
#define HAL_PWM_PORT_BITMASK(ch) ((uint16_t)0x400 << (ch))
#endif // RXBOARD_XXXX
#define HAL_PWM_CLEAR_BITS(inv_mask)            \
    {                                           \
        PORTD &= (uint8_t)(inv_mask);           \
        PORTB &= *((uint8_t *)&(inv_mask) + 1); \
    }
#define HAL_PWM_SET_BITS(mask)              \
    {                                       \
        PORTD |= (uint8_t)(mask);           \
        PORTB |= *((uint8_t *)&(mask) + 1); \
    }

#define HAL_PWM_MAX_INTERRUPT_TIME_US 20

#define PWM_TIMER_FRAME_ISR(void) ISR(TIMER1_COMPA_vect)
#define PWM_TIMER_CH_ISR(void) ISR(TIMER1_COMPB_vect)

#define HAL_PWM_UPDATE_NEXTTICK(x) (OCR1B = (x))
#define HAL_PWM_ISR_DISABLE() cli()
#define HAL_PWM_ISR_ENABLE() sei()
#define HAL_PWM_ISR_FLAG_SET() 1
#define HAL_PWM_ISR_CLEAR_FLAG()

#endif // HAL_PWM_H_
