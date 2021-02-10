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

#include "cc25xx.h"
#include "debug.h"
#include "timeout.h"
#include "delay.h"

uint8_t cc25xx_current_antenna;

void cc25xx_init(void) {
    debug("cc25xx: init\n"); debug_flush();
    cc25xx_current_antenna = 0;
    hal_cc25xx_init();
    hal_cc25xx_set_antenna(cc25xx_current_antenna);
}

void cc25xx_switch_antenna(void) {
    // switch to next antenna
    if (cc25xx_current_antenna) {
        cc25xx_current_antenna = hal_cc25xx_set_antenna(0);
    } else {
        cc25xx_current_antenna = hal_cc25xx_set_antenna(1);
    }
}

void cc25xx_wait_for_transmission_complete(void) {
    // after STX we go back to RX state (see MCSM1 register)
    // so wait a maximum of 9ms for completion
    timeout2_set_100us(90);

    while (!timeout2_timed_out()) {
        if (hal_cc25xx_transmission_completed()) {
            // done with tx, return
            return;
        }
        delay_us(20);
    }

    // if we reach this point, tx timed out:
    debug("!TX");
}
