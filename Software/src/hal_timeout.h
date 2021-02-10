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

#ifndef HAL_TIMEOUT_H_
#define HAL_TIMEOUT_H_
#include <stdint.h>

extern volatile uint8_t timeout1_end;
extern volatile uint8_t timeout2_end;

void hal_timeout_init(void);
void hal_timeout_set(uint16_t ms);
void hal_timeout_set_100us(uint16_t hus);
#define hal_timeout_timed_out() timeout1_end
void hal_timeout_delay_ms(uint16_t timeout);

void hal_timeout2_set(uint16_t ms);
void hal_timeout2_set_100us(uint16_t hus);
//uint8_t hal_timeout2_timed_out(void);
#define hal_timeout2_timed_out() timeout2_end

uint16_t hal_timeout_time_remaining(void);

void SysTick_Handler(void);

#endif  // HAL_TIMEOUT_H_
