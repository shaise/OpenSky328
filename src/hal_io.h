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

#ifndef HAL_IO_H_
#define HAL_IO_H_

#include <stdint.h>
#include "config.h"

void hal_io_init(void);
uint8_t hal_io_bind_request(void);
#define DEBUG_PIN_LOW(pin) (DEBUG_PIN_OUT &= ~(pin))
#define DEBUG_PIN_HIGH(pin) (DEBUG_PIN_OUT |= (pin))

#endif  // HAL_IO_H_
