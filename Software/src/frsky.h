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

#ifndef FRSKY_H_
#define FRSKY_H_

#include <stdint.h>
#include "main.h"
#include "hal_cc25xx.h"

#define FRSKY_HOPTABLE_SIZE 47

// #define FRSKY_COUNT_RXSTATS 100
#define FRSKY_COUNT_RXSTATS 50

// extern EXTERNAL_MEMORY uint8_t frsky_txid[2];
// extern EXTERNAL_MEMORY uint8_t frsky_hop_table[FRSKY_HOPTABLE_SIZE];
// extern EXTERNAL_MEMORY int8_t frsky_freq_offset;
extern EXTERNAL_MEMORY uint8_t frsky_current_ch_idx;
extern EXTERNAL_MEMORY uint8_t frsky_diversity_count;
// rssi
extern EXTERNAL_MEMORY uint8_t frsky_rssi;
extern EXTERNAL_MEMORY uint8_t frsky_link_quality;
// pll calibration
extern EXTERNAL_MEMORY uint8_t frsky_calib_fscal1_table[FRSKY_HOPTABLE_SIZE];
extern EXTERNAL_MEMORY uint8_t frsky_calib_fscal2;
extern EXTERNAL_MEMORY uint8_t frsky_calib_fscal3;
// extern EXTERNAL_MEMORY int16_t frsky_freq_offset_acc;

#define FRSKY_PACKET_LENGTH 17
#define FRSKY_PACKET_BUFFER_SIZE (FRSKY_PACKET_LENGTH + 3)
extern EXTERNAL_MEMORY volatile uint8_t frsky_packet_buffer[FRSKY_PACKET_BUFFER_SIZE];
extern EXTERNAL_MEMORY volatile uint8_t frsky_packet_received;
extern EXTERNAL_MEMORY volatile uint8_t frsky_packet_sent;

void frsky_init(void);
void frsky_show_partinfo(void);
void frsky_configure(void);
uint8_t frsky_bind_jumper_set(void);
void frsky_do_bind(void);
void frsky_configure_address(void);
void frsky_autotune(void);
void frsky_enter_rxmode(uint8_t channel);
void frsky_tune_channel(uint8_t ch);
void frsky_handle_overflows(void);
void frsky_fetch_txid_and_hoptable(void);
void frsky_calib_pll(void);
void frsky_main(void);
uint8_t frsky_extract_rssi(uint8_t rssi_raw);
void frsky_increment_channel(int8_t cnt);
void frsky_set_channel(uint8_t hop_index);
void frsky_send_telemetry(uint8_t telemetry_id);
void frsky_update_ppm(void);

extern uint8_t conn_lost;

// packet data example:
// BIND:   [11 03 01 16 68 14 7E BF 15 56 97 00 00 00 00 00 00 0B F8 AF ]
// NORMAL: [11 16 68 ... ]
// TX:                 11 16 68 7A 1B 0B CA CB CF C4 88 85 CB CB CB 92 8B 78 21 AF
// TELEMETRY WITH HUB: 11 16 68 60 64 5B 00 00 5E 3B 09 00 5E 5E 3B 09 00 5E 48 B1
#define FRSKY_VALID_FRAMELENGTH(_b) (_b[0] == 0x11)
#define FRSKY_VALID_CRC(_b) (_b[19] & 0x80)
#define FRSKY_VALID_TXID(_b) ((_b[1] == storage.frsky_txid[0]) && (_b[2] == storage.frsky_txid[1]))
#define FRSKY_VALID_PACKET_BIND(_b) \
    (FRSKY_VALID_FRAMELENGTH(_b) && FRSKY_VALID_CRC(_b) && (_b[2] == 0x01))
#define FRSKY_VALID_PACKET(_b) \
    (FRSKY_VALID_FRAMELENGTH(_b) && FRSKY_VALID_CRC(_b) && FRSKY_VALID_TXID(_b))

/*
#define FRSKY_HUB_TELEMETRY_HEADER 0x5E
#define FRSKY_HUB_TELEMETRY_VOLTAGE 0x39 // not really documented, seems to be volt in 0.1V steps...
#define FRSKY_HUB_TELEMETRY_VOLTAGE_BEFORE 0x3A
#define FRSKY_HUB_TELEMETRY_VOLTAGE_AFTER  0x3B
#define FRSKY_HUB_TELEMETRY_CURRENT        0x28
*/

#endif // FRSKY_H_
