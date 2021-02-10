#ifndef __PIN_CONFIG_H__
#define __PIN_CONFIG_H__
#include "main.h"

/* TODO find correct pins for LEDs */
#define LED_RED_PIN               (1<<5)
#define LED_RED_OUT              PORTC
#define LED_RED_DIR               DDRC
#define LED_GREEN_PIN             (1<<4)
#define LED_GREEN_OUT            PORTC
#define LED_GREEN_DIR             DDRC

#define ADC0_PIN                  A6
#define ADC1_PIN                  A7

#define BIND_PIN                  (1<<3)
#define BIND_OUT                  PORTC
#define BIND_IN                   PINC
#define BIND_DIR                  DDRC

#define DEBUG_PIN_0               (1<<1)
#define DEBUG_PIN_1               (1<<2)
#define DEBUG_PIN_DIR             DDRC
#define DEBUG_PIN_OUT             PORTC

// system supports only one of the following: PWM, SBUS, PPM

// enable SBUS output -> this will DISABLE ppm!
//#define SBUS_ENABLED
// Note: default/futaba is INVERTED=1! (use this for a cc3d etc)
//       inverted     => idle = LOW
//       non inverted => idle = high
#define SBUS_INVERTED

// PPM 
// #define PPM_ENABLED
// invert SBUS output (normal is non inverted)
// #define PPM_INVERTED

// hub telemetry input (soft serial)
// #define HUB_TELEMETRY_ON_SBUS_UART
// #define HUB_TELEMETRY_INVERTED 
// #define PPM_INVERTED

// PWM
#define PWM_ENABLED
#define PWM_MAX_CHANNELS 8

// failsafe: bind button press time in ms the will initiate failsafe programming sequence
#define FAILSAFE_PRESS_TIME 50   // 5 seconds
#define FAILSAFE_PROGRAM_WAIT_TIME 100   // 10 seconds

// MOTOR - if enabled will output a pwm waveform based on channel 3, suitable for DC motor
#define MOTOR_ENABLED
#define MOTOR_CHANNEL 3

#define SBUS_USART                 USART1
#define SBUS_USART_CLK             RCC_APB2Periph_USART1
#define SBUS_USART_RCC             2
#define SBUS_USART_GPIO            GPIOA
#define SBUS_USART_GPIO_CLK        RCC_APB2Periph_GPIOA
#define SBUS_USART_GPIO_CLK_RCC    2
#define SBUS_USART_TX_PIN          GPIO_Pin_9
#define SBUS_USART_IRQHANDLER      USART1_IRQHandler
#define SBUS_USART_IRQn            USART1_IRQn

#define USE_SOFTWARE_SPI

#ifdef FR_OLD
#define CC25XX_SPI_SCK_PIN          (1<<5) 
#define CC25XX_SPI_SCK_OUT          PORTB
#define CC25XX_SPI_SCK_DIR          DDRB

#define CC25XX_SPI_MOSI_PIN         (1<<3) 
#define CC25XX_SPI_MOSI_OUT         PORTB
#define CC25XX_SPI_MOSI_DIR         DDRB

#define CC25XX_SPI_MISO_PIN         (1<<4) 
#define CC25XX_SPI_MISO_OUT         PORTB
#define CC25XX_SPI_MISO_IN          PINB
#define CC25XX_SPI_MISO_DIR         DDRB

#define CC25XX_SPI_CSN_PIN          (1<<2) 
#define CC25XX_SPI_CSN_OUT          PORTB
#define CC25XX_SPI_CSN_DIR          DDRB
#else
#define CC25XX_SPI_SCK_PIN          (1<<3) 
#define CC25XX_SPI_SCK_OUT          PORTD
#define CC25XX_SPI_SCK_DIR          DDRD

#define CC25XX_SPI_MOSI_PIN         (1<<2) 
#define CC25XX_SPI_MOSI_OUT         PORTD
#define CC25XX_SPI_MOSI_DIR         DDRD

#define CC25XX_SPI_MISO_PIN         (1<<4) 
#define CC25XX_SPI_MISO_OUT         PORTD
#define CC25XX_SPI_MISO_IN          PIND
#define CC25XX_SPI_MISO_DIR         DDRD

#define CC25XX_SPI_CSN_PIN          (1<<5) 
#define CC25XX_SPI_CSN_OUT          PORTD
#define CC25XX_SPI_CSN_DIR          DDRD
#endif // FR_OLD

#define CC25XX_SPI_GDO1_PIN         CC25XX_SPI_MISO_PIN
#define CC25XX_SPI_GDO1_IN          CC25XX_SPI_MISO_IN
#define CC25XX_SPI_GDO1_DIR         CC25XX_SPI_MISO_DIR

/* 
    XSR uses the Skyworks SE2431 range extender
    See: http:// www.skyworksinc.com/uploads/documents/SE2431L_202410J.pdf

    Looks to be a similar design to this: http:// www.ti.com/tool/CC2530-SE2431L

    Which means the RX pin selection is not needed (it is RX if TX is off)

    Either ANT can be used for both RX and TX.

    High is logic 1, Low is logic 0

    ANT (PB5) selection is 0 (left) or 1 (right)
    CTX (PA15) is 0 then RX LNA mode active, or 1 then tx mode
    CPS (PB3) is 0 then bypass mode is active, 1 is bypass off.
    CSD (PB4) with 1 to enable chip (0 = sleep)
*/ 

/* TODO revisit base on the above */
#define CC25XX_ANT_SW_CTX_GPIO     GPIOC
#define CC25XX_ANT_SW_CTX_GPIO_CLK RCC_APB2Periph_GPIOC
#define CC25XX_ANT_SW_CTX_GPIO_CLK_RCC 2
#define CC25XX_ANT_SW_CTX_PIN      GPIO_Pin_15


#define CC25XX_ANT_SW_CRX_GPIO     GPIOC
#define CC25XX_ANT_SW_CRX_GPIO_CLK RCC_APB2Periph_GPIOC
#define CC25XX_ANT_SW_CRX_GPIO_CLK_RCC 2
#define CC25XX_ANT_SW_CRX_PIN      GPIO_Pin_14


#define CC25XX_LNA_SW_CTX_GPIO     GPIOA
#define CC25XX_LNA_SW_CTX_GPIO_CLK RCC_APB2Periph_GPIOA
#define CC25XX_LNA_SW_CTX_GPIO_CLK_RCC 2
#define CC25XX_LNA_SW_CTX_PIN      GPIO_Pin_15


#define CC25XX_LNA_SW_CRX_GPIO     GPIOB
#define CC25XX_LNA_SW_CRX_GPIO_CLK RCC_APB2Periph_GPIOB
#define CC25XX_LNA_SW_CRX_GPIO_CLK_RCC 2
#define CC25XX_LNA_SW_CRX_PIN      GPIO_Pin_4

/* NOTE no analogue input for XSR, and no servo pins */

// cppm output is on PB12
#define PPM_PIN                   2
/* TODO: ??? chose timer for pin */
//#define PPM_TIMER_IRQHANDLER      TIM3_IRQHandler

/*
    hub telemetry input is RX USART3 (SPORT)

    PB11 for RX and PB10 for TX both inverted once leaving board.
    Output Enable (OE) is PA2
*/    
#define SOFT_SERIAL_GPIO 0
#define SOFT_SERIAL_PIN  1

// THIS CONFIGURES IRQ PRIORITIES - DO NOT MESS THIS UP!
// this is the most critical stuff:
#define NVIC_PRIO_PPM 1
// sbus is a tx interrupt, this can be delayed at no cost
#define NVIC_PRIO_SBUS 6
// this is very time critical, but as telemetry data is not that important
// we allow this to be interrupted (=corrupted) occasionally
#define NVIC_PRIO_SOFT_SERIAL 2
// debugging data is not critical
#define NVIC_PRIO_DEBUG_UART 7
// systick prio
#define NVIC_PRIO_SYSTICK 0

#define DEFAULT_FSCAL_VALUE -62

#endif // __PIN_CONFIG_H__
