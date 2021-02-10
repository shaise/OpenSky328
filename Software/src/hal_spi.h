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

#ifndef HAL_SPI_H_
#define HAL_SPI_H_

#include <Arduino.h>
#include <stdint.h>
#include "config.h"
#include "delay.h"

void hal_spi_init(void);
void hal_spi_dma_xfer(uint8_t *buffer, uint8_t len);
#define hal_spi_csn_lo() { CC25XX_SPI_CSN_OUT &= ~CC25XX_SPI_CSN_PIN; delay_us(1); }
#define hal_spi_csn_hi() { delay_us(1); CC25XX_SPI_CSN_OUT |= CC25XX_SPI_CSN_PIN; }
uint8_t hal_spi_tx(uint8_t value);
#define hal_spi_rx()  hal_spi_tx(0)
uint8_t hal_spi_read_address(uint8_t address);

#endif  // HAL_SPI_H_
