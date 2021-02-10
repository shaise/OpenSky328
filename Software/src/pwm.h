/*
    Copyright 2017 fishpepper <AT> gmail.com

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

   author: fishpepper <AT> gmail.com
*/

#ifndef PWM_H_
#define PWM_H_
#include <stdint.h>
#include "main.h"
#include "hal_pwm.h"

#ifndef PWM_ENABLED

// ppm is not used then
#define pwm_init() {}
#define pwm_update(a) {}
#define pwm_exit_failsafe() {}
#define pwm_enter_failsafe() {}


#else
void pwm_init(void);
void pwm_update(EXTERNAL_MEMORY uint16_t *data);
void pwm_exit_failsafe(void);
void pwm_enter_failsafe(void);
void pwm_isr(void);

#endif  // PWM_ENABLED

// #define PPM_US_TO_TICKCOUNT(us) HAL_PPM_US_TO_TICKCOUNT(us)
#define PWM_FRSKY_TO_TICKCOUNT(_frsky) HAL_PWM_FRSKY_TO_TICKCOUNT(_frsky)
#define PWM_FRAME_LEN HAL_PWM_US_TO_TICKCOUNT(20000L)
#define PWM_MAX_PULSE_LEN 2020
#define PWM_MIN_PULSE_LEN 980
#define PWM_MAX_PULSE_TICKS HAL_PWM_US_TO_TICKCOUNT(PWM_MAX_PULSE_LEN)
#define PWM_MIN_PULSE_TICKS HAL_PWM_US_TO_TICKCOUNT(PWM_MIN_PULSE_LEN)
#define PWM_END_PULSE_TICKS HAL_PWM_US_TO_TICKCOUNT(PWM_MAX_PULSE_LEN + HAL_PWM_MAX_INTERRUPT_TIME_US)
#define PWM_MIN_TICKCOUNT HAL_PWM_US_TO_TICKCOUNT(HAL_PWM_MAX_INTERRUPT_TIME_US)
#define PWM_SYNC_PULS_LEN_TICKS (HAL_PWM_US_TO_TICKCOUNT(PWM_SYNC_DURATION_US))


#endif  // PPM_H_
