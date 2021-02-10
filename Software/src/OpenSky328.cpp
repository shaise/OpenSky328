/*
*/
//#include <SPI.h>
#include <Arduino.h>
#include <EEPROM.h>
const int dataReadyPin = 12;
const int chipSelectPin = 2;

extern "C" {
  #include "hal_debug.h"
  #include "hal_storage.h"
  #include "main.h"
  #include "hal_io.h"
  #include "hal_wdt.h"
  #include "hal_adc.h"
  #include "hal_soft_serial.h"
  #include "debug.h"
}

int main(void)
{
  init();
  //delay(5000);
  Serial.begin(115200);
  Serial.println("starting...\n");
  debug_init();
  opensky_main();
  return 0;
}

// hal commands

// debug
void hal_debug_start_transmission(uint8_t data)
{
  Serial.print((char)data);
}

void hal_debug_init(void) 
{
  Serial.println("hal debug init.\n");
}


// storage
void hal_storage_init(void)
{
  
}

void hal_storage_write(uint8_t *buffer, uint16_t len)
{
  for (uint32_t i = 0; i < len; i++)
  {
    EEPROM.write(i, *buffer);
    buffer++;
  }  
}

void hal_storage_read(uint8_t *storage_ptr, uint16_t len)
{
  for (uint32_t i = 0; i < len; i++)
  {
    *storage_ptr = EEPROM.read(i);
    storage_ptr++;
  }
}

// io
void hal_io_init(void)
{
  BIND_DIR &= ~BIND_PIN;
  BIND_OUT |= BIND_PIN; // make input pullup
  DEBUG_PIN_DIR |= (DEBUG_PIN_0 | DEBUG_PIN_1);
}

uint8_t hal_io_bind_request(void) 
{ 
  //Serial.println(BIND_PORT & BIND_PIN);
  return (BIND_IN & BIND_PIN) == 0; 
}

// watch dog
void hal_wdt_init(void) {}
void hal_wdt_reset(void) {}


// analog
void hal_adc_init(void) {}

uint8_t hal_adc_get_scaled(uint8_t ch)
{
  ch = ch == 0 ? ADC0_PIN : ADC1_PIN;
  return analogRead(ch) >> 2;
}
void hal_adc_process(void) {}

// soft serial
void hal_soft_serial_init(void) {}

