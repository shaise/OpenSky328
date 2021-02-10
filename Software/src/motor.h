/*
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

#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>
#include "main.h"
#include "hal_motor.h"

#ifndef MOTOR_ENABLED
#define motor_init() hal_motor_init()
void motor_update(uint16_t data);
#else
#define motor_init() hal_motor_init()
void motor_update(int16_t data);

#endif  // MOTOR_ENABLED


#endif  // MOTOR_H_
