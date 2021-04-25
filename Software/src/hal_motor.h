/*
    hal_motor.h : handle dc motor pwm output

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

#ifndef HAL_MOTOR_H_
#define HAL_MOTOR_H_

#include "config.h"
#include <avr/io.h>

#ifdef MOTOR_ENABLED

// power is between 0 and 100
void hal_motor_init();
void hal_motor_set_power(uint8_t *pow);
#define hal_motor_on() (HAL_MOTOR_PORT |= HAL_MOTOR_PORT_MASK)
#define hal_motor_off() (HAL_MOTOR_PORT &= ~HAL_MOTOR_PORT_MASK)

#else // MOTOR_ENABLE

#define hal_motor_init() \
    {                    \
    }
#define hal_motor_set_power(pow) \
    {                            \
    }
#define hal_motor_on() \
    {                  \
    }
#define hal_motor_off() \
    {                   \
    }

#endif // MOTOR_ENABLE
#endif //  HAL_MOTOR_H_
