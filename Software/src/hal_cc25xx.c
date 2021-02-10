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

#include "hal_cc25xx.h"
#include "hal_spi.h"
#include "debug.h"
#include "io.h"
#include "cc25xx.h"
#include "timeout.h"
#include <string.h>


static void hal_cc25xx_init_gpio(void) {
    // antenna switch
    // select first antenna
    hal_cc25xx_set_antenna(0);

    // PA/LNA:
    hal_cc25xx_enter_rxmode();
}

void hal_cc25xx_init(void) {
    hal_spi_init();
    hal_cc25xx_init_gpio();
}

inline uint32_t hal_cc25xx_set_antenna(uint8_t id) {
    // select antenna 0 or 1:
    return id;
}

inline void hal_cc25xx_set_gdo_mode(void) {
    cc25xx_set_register(IOCFG0, 0x5C);  // LNA_PD;
    //cc25xx_set_register(IOCFG0, 0x2F);  // HW_TO_0;
    cc25xx_set_register(IOCFG1, 0x01);
    cc25xx_set_register(IOCFG2, 0x5B);  // PA_PD
    //cc25xx_set_register(IOCFG2, 0x2F);  // HW_TO_0
    //cc25xx_set_register(IOCFG2, 0x6F);  // HW_TO_1
}

inline uint8_t hal_cc25xx_set_register(uint8_t address, uint8_t data) {
    // select device
    hal_spi_csn_lo();

    // wait for ready signal
    while (hal_cc25xx_get_gdo_status()) {}

    uint8_t status = hal_spi_tx(address);
    hal_spi_tx(data);

    // deslect
    hal_spi_csn_hi();
    return status;
}

inline uint8_t hal_cc25xx_get_register(uint8_t address) {
    uint8_t result;

    // select device:
    hal_spi_csn_lo();

    // wait for RDY signal:
    while (hal_cc25xx_get_gdo_status());
 
    // request address (read request has bit7 set)
    /*uint8_t status = */ hal_spi_tx(address | 0x80);
    // debug_put_hex8(status);

    // fetch result:
    result = hal_spi_rx();

    // deselect device
    hal_spi_csn_hi();

    // return result
    return(result);
}

inline uint8_t hal_cc25xx_strobe(uint8_t address) {
    hal_spi_csn_lo();
    uint8_t status = hal_spi_tx(address);
    // debug("s"); debug_put_hex8(status); debug_put_newline();
    hal_spi_csn_hi();
    return status;
}

inline uint8_t hal_cc25xx_get_status(void) {
    hal_spi_csn_lo();
    uint8_t status = hal_spi_tx(0xBD);
    hal_spi_csn_hi();
    return status;
}

uint8_t hal_cc25xx_transmission_completed(void) {
    // after tx cc25xx goes back to RX (configured by mcsm1 register)
    return ((hal_cc25xx_get_status() & (0x70)) == CC2500_STATUS_STATE_RX);
}

inline void hal_cc25xx_enter_rxmode(void) {
    // add pa/lna config bit setting here
    // should be done automatically
    delay_us(25);
}

inline void hal_cc25xx_enter_txmode(void) {
    // add pa/lna config bit setting here
    // should be done automatically
    delay_us(25);
}


inline void hal_cc25xx_enable_receive(void) {
    // switch on rx again
    hal_cc25xx_enter_rxmode();
}


inline uint8_t hal_cc25xx_get_gdo_status(void) {
    if ((CC25XX_SPI_MISO_IN & CC25XX_SPI_MISO_PIN) != 0) {
        return 1;
    } else {
        return 0;
    }
}

inline void hal_cc25xx_read_fifo(uint8_t *buf, uint8_t len) {
    hal_cc25xx_register_read_multi(CC25XX_FIFO | READ_FLAG | BURST_FLAG, buf, len);
}

inline void hal_cc25xx_register_read_multi(uint8_t address, uint8_t *buffer, uint8_t len) {
    // select device:
    hal_spi_csn_lo();

    // wait for ready signal
    while (hal_cc25xx_get_gdo_status()) {}

    // debug("read "); debug_put_uint8(len); debug_flush();
    // request address (read request)
    /* uint8_t status = */ hal_spi_tx(address);

    // fill buffer with read commands:
    memset(buffer, 0xFF, len);

    while (len--) {
        *buffer = hal_spi_rx();
        buffer++;
    }

    // deselect device
    hal_spi_csn_hi();
}

void hal_cc25xx_register_write_multi(uint8_t address, uint8_t *buffer, uint8_t len) {
    // s elect device:
    hal_spi_csn_lo();

    // wait for RDY signal:
    while (hal_cc25xx_get_gdo_status()) {}

    // request address (write request)
    hal_spi_tx(address | BURST_FLAG);

    // send array
    while (len--) {
        hal_spi_tx(*buffer);
        buffer++;
    }

    // deselect device
    hal_spi_csn_hi();
}

inline void hal_cc25xx_process_packet(volatile uint8_t *packet_received,
                                      volatile uint8_t *buffer, uint8_t maxlen) {
    *packet_received = 0;
    if (hal_cc25xx_get_gdo_status() == 1) {
        delay_us(20);
        // data received, fetch da
        timeout2_set_100us(5);
        // there is a bug in the cc2500
        // see p3 http:// www.ti.com/lit/er/swrz002e/swrz002e.pdf
        // workaround: read len register very quickly twice:
        uint8_t len1, len2, len, i;

        // try this 10 times befor giving up:
        for (i = 0; i < 10; i++) {
            len1 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            len2 = hal_cc25xx_get_register_burst(RXBYTES) & 0x7F;
            if (len1 == len2) break;
        }

        // valid len found?
        if (len1 == len2) {
            len = len1;

            // packet received, grab data
            uint8_t tmp_buffer[len];
            hal_cc25xx_read_fifo(tmp_buffer, len);

            // only accept valid packet lenbghts:
            if (len == maxlen) {
                uint8_t i;
                for (i = 0; i < maxlen; i++) {
                    buffer[i] = tmp_buffer[i];
                }
                *packet_received = 1;
            }
        } else {
            // no, ignore this
            debug_putc('B');
            len = 0;
        }
    }
 }

void hal_cc25xx_prepare_tx_packet(volatile uint8_t *buffer, uint8_t len) {
    // flush tx fifo
    hal_cc25xx_strobe(RFST_SFTX);
    // copy to fifo
    hal_cc25xx_register_write_multi(CC25XX_FIFO, (uint8_t *)buffer, buffer[0]+1);
 }
