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

#include "pwm.h"
#include "main.h"
#include "debug.h"
#include "wdt.h"
#include "failsafe.h"
#include "storage.h"

#ifdef PWM_ENABLED

// pwm signal per ch:
//   p     FILL_UP_TO_20.0ms
// |`````|__________________________
//
// p   = pulse time where 1ms = min servo, 1.5ms = mid, 2ms = max

typedef struct _s_channel_info {
    uint16_t pulse_ticks;
    uint16_t inv_mask;
    struct _s_channel_info *next;
} s_channel_info;

// double buffer chanel info
static s_channel_info channel_info_a[PWM_MAX_CHANNELS];
static s_channel_info channel_info_b[PWM_MAX_CHANNELS];
static s_channel_info end_chanel = { PWM_END_PULSE_TICKS, 0, 0};

static s_channel_info *p_back_channel_info = channel_info_a;
static s_channel_info *p_front_channel_info = channel_info_b;
static s_channel_info *p_current_channel_info = &end_chanel;
static s_channel_info *p_next_isr_chan_inf;

static uint16_t pwm_all_bitmask;
static uint8_t pwm_newdata_ready = 0;
static uint8_t frame_isr_valid = 0;

//uint16_t cnt1 = 0, cnt2;


void pwm_init(void) {
    uint8_t i;
    debug("pwm: init\n"); debug_flush();
    pwm_all_bitmask = 0;
    // initialise
    for (i = 0; i < PWM_MAX_CHANNELS; i++) {
        pwm_all_bitmask |= HAL_PWM_PORT_BITMASK(i);
    }
    end_chanel.inv_mask = ~pwm_all_bitmask;
    //pwm_enter_failsafe();

    hal_pwm_init();

    debug("pwm: max_pulse: ");
    debug_put_uint16(end_chanel.pulse_ticks);   
    debug("\npwm: init done\n"); debug_flush();
}

static inline s_channel_info *get_nearest_channel(uint16_t pulse_ticks)
{
    s_channel_info *p_prev = NULL;
    s_channel_info *p_chan_inf = p_current_channel_info;
    while (p_chan_inf != NULL) {
        if (pulse_ticks < p_chan_inf->pulse_ticks)
            return p_prev;
        p_prev = p_chan_inf;
        p_chan_inf = p_chan_inf->next;
    } 
    return p_prev;
}

uint8_t i_free_chan = 0;
extern uint16_t show_data;

// add channel data sorted by pulse length
static void add_channel(uint16_t pulse_ticks, uint8_t ch)
{
    uint16_t inv_mask;
    s_channel_info *chan_inf, *prev_chan_inf;

    if (i_free_chan >= PWM_MAX_CHANNELS)
        return;

    if (pulse_ticks > PWM_MAX_PULSE_TICKS)
        pulse_ticks = PWM_MAX_PULSE_TICKS;
    if (pulse_ticks < PWM_MIN_PULSE_TICKS)
        pulse_ticks = PWM_MIN_PULSE_TICKS;
 
    prev_chan_inf = get_nearest_channel(pulse_ticks);
    //debug_putc(prev_chan_inf == NULL ? '.' : ',');
    //return;
    inv_mask = ~HAL_PWM_PORT_BITMASK(ch);
    //if (ch == 0) show_data = pulse_ticks;

    // check if pulse_ticks is too close to prev one. if so, merge to avoid missing an interrupt 
    if (prev_chan_inf != NULL)
    {
       if ((pulse_ticks - prev_chan_inf->pulse_ticks) < PWM_MIN_TICKCOUNT)
        {
            prev_chan_inf->inv_mask &= inv_mask;
            return;
        }
        chan_inf = prev_chan_inf->next;
    }
    else
    {
        chan_inf = p_current_channel_info;
    }
    
    // check if pulse_ticks is too close to next one as well.
    if ((chan_inf->pulse_ticks - pulse_ticks) < PWM_MIN_TICKCOUNT)
    {
        chan_inf->inv_mask &= inv_mask;
        return;           
    }

    chan_inf = &(p_back_channel_info[i_free_chan]);
    chan_inf->inv_mask = inv_mask;
    chan_inf->pulse_ticks = pulse_ticks;
    if (prev_chan_inf == NULL) {
        chan_inf->next = p_current_channel_info;
        p_current_channel_info = chan_inf;
    }
    else
    {
        chan_inf->next = prev_chan_inf->next;
        prev_chan_inf->next = chan_inf;
    }    
    i_free_chan++;
}


void pwm_update(EXTERNAL_MEMORY uint16_t *data) 
{
    uint8_t i = 0;
    
    HAL_PWM_ISR_DISABLE();
    pwm_newdata_ready = 0;
    HAL_PWM_ISR_ENABLE();

    p_current_channel_info = &end_chanel;
    pwm_all_bitmask = 0;
    i_free_chan = 0;
    for (i = 0; i < PWM_MAX_CHANNELS; i++) {
        if (data[i] > 0)
        {
            // convert to ticks for timer
            // input is 0..4095, we should map this to 1000..2000us
            // frsky seems to send us*1.5 (~1480...3020) -> divide by 1.5 (=*2/3) to get us
            add_channel(PWM_FRSKY_TO_TICKCOUNT(data[i]), i);
            pwm_all_bitmask |= HAL_PWM_PORT_BITMASK(i);
        }
        else
        {
            /* zero in data - usually happens when starting the transmitter with throttle not zero */
        }
    }

    frame_isr_valid = pwm_all_bitmask != 0;

    p_next_isr_chan_inf = p_current_channel_info;
    pwm_newdata_ready = 1;
 }

void pwm_exit_failsafe(void) 
{
    // debug("pwm: exit FS\n");
}

void pwm_enter_failsafe(void) {
    uint8_t i = 0;

    HAL_PWM_ISR_DISABLE();
    pwm_newdata_ready = 0;
    HAL_PWM_ISR_ENABLE();

    i_free_chan = 0;
    p_current_channel_info = &end_chanel;
    for (i = 0; i < PWM_MAX_CHANNELS; i++) {
         add_channel(HAL_PWM_US_TO_TICKCOUNT((uint16_t)storage.fail_safe_state[i] * 10), i);
    }

    p_next_isr_chan_inf = p_current_channel_info;
    pwm_newdata_ready = 1;   
    debug("pwm: entered FS\n");
}

// this handles the reloading of the
// channel data to the timer cmp register
static uint8_t ch_isr_valid = 0;
static s_channel_info *p_isr_chan_inf, *p_active_isr_chan_inf;
static uint16_t pwm_isr_all_bitmask = 0;


PWM_TIMER_CH_ISR(void) 
{
    if (!HAL_PWM_ISR_FLAG_SET())
        return;

    //HAL_PWM_CLEAR_BITS(B00000011);
    //cnt2 = TCNT1;
    //return;

    if (ch_isr_valid) 
    {
        HAL_PWM_CLEAR_BITS(p_isr_chan_inf->inv_mask);
        p_isr_chan_inf = p_isr_chan_inf->next;
        if (p_isr_chan_inf != NULL)
            HAL_PWM_UPDATE_NEXTTICK(p_isr_chan_inf->pulse_ticks);
        else
            ch_isr_valid = 0;      
    }
    
    HAL_PWM_ISR_CLEAR_FLAG();    
}

PWM_TIMER_FRAME_ISR(void)
{
    s_channel_info *chan_inf;
    if (!frame_isr_valid)
        return;
    
    HAL_PWM_SET_BITS(pwm_isr_all_bitmask);
    //HAL_PWM_UPDATE_NEXTTICK(4000);
    //cnt1 = TCNT1;
    //return;

    if (pwm_newdata_ready) 
    {
        // switch buffers
        p_active_isr_chan_inf = p_next_isr_chan_inf;
        pwm_isr_all_bitmask = pwm_all_bitmask;
        chan_inf = p_front_channel_info;
        p_front_channel_info = p_back_channel_info;
        p_back_channel_info = chan_inf; 
    }
    p_isr_chan_inf = p_active_isr_chan_inf;
    HAL_PWM_UPDATE_NEXTTICK(p_isr_chan_inf->pulse_ticks);
    ch_isr_valid = 1;
}

#endif  // PWM_ENABLED

