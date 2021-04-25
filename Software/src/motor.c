/*
    motor.c : handle dc motor pwm output

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

#include "motor.h"
#include "debug.h"

#ifdef MOTOR_ENABLED

void motor_update(uint16_t *rx_vals)
{
    uint8_t motor_vals[MOTOR_COUNT];
    if (rx_vals == 0)
    {
        for (int i = 0; i < MOTOR_COUNT; i++)
            motor_vals[i] = 0;
    }
    else
    {
        for (int i = 0; i < MOTOR_COUNT; i++, rx_vals++)
        {
            // convert frsky range (~1480...3020) -> to power (0 - 100)
            // we narrow this to 1600 - 3000 to make sure we reach both sides
            uint16_t data = *rx_vals;
            if (data < 1620)
                data = 0; // safty - 1480 to 1620 is considered 0
            else
            {
                data -= 1600;
                data /= 14; // (3000 - 1600) / 100
                if (data > 100)
                    data = 100;
            }
            motor_vals[i] = (uint8_t)data;
        }
    }
    hal_motor_set_power(motor_vals);
}

#endif // MOTOR_ENABLED
