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

#ifndef HAL_DELAY_H_
#define HAL_DELAY_H_

#include <Arduino.h>
#include "hal_timeout.h"

#define hal_delay_ms(ms) hal_timeout_delay_ms(ms)
#define hal_delay_us(us) delayMicroseconds(us)
#define hal_clock_ms() millis()


#endif  // HAL_DELAY_H_
