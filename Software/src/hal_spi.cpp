#include <Arduino.h>
#include <../../SPI/src/SPI.h>

extern "C" {
    #include "hal_spi.h"
    #include "debug.h"
}

void hal_spi_init(void)
{
    CC25XX_SPI_CSN_DIR |= CC25XX_SPI_CSN_PIN;
    CC25XX_SPI_CSN_OUT |= CC25XX_SPI_CSN_PIN;
#ifdef USE_SOFTWARE_SPI
    // set port directions
    CC25XX_SPI_SCK_DIR |= CC25XX_SPI_SCK_PIN;
    CC25XX_SPI_SCK_OUT &= ~CC25XX_SPI_SCK_PIN;
    CC25XX_SPI_MISO_DIR &= ~CC25XX_SPI_MISO_PIN;
    CC25XX_SPI_MISO_OUT &= ~CC25XX_SPI_MISO_PIN; // disable pullup
    CC25XX_SPI_MOSI_DIR |= CC25XX_SPI_MOSI_PIN;  
#else 
    SPI.begin();
#endif
}

#ifdef USE_DMA
void hal_spi_dma_xfer(uint8_t *buffer, uint8_t len)
{
    // unused
}
#endif

uint8_t hal_spi_tx(uint8_t value) 
{
#ifdef USE_SOFTWARE_SPI
    uint8_t res = 0, i;
    // setup first bit
    if ((value & 0x80) != 0)
        CC25XX_SPI_MOSI_OUT |= CC25XX_SPI_MOSI_PIN;
    else
        CC25XX_SPI_MOSI_OUT &= ~CC25XX_SPI_MOSI_PIN;
    for (i = 0; i < 8; i++)
    {
         // read bit
        res <<= 1;
       if ((CC25XX_SPI_MISO_IN & CC25XX_SPI_MISO_PIN) != 0)
            res |= 1;
        // clock up
         CC25XX_SPI_SCK_OUT |= CC25XX_SPI_SCK_PIN;
        // setup next bit
        value <<= 1;
        if ((value & 0x80) != 0)
            CC25XX_SPI_MOSI_OUT |= CC25XX_SPI_MOSI_PIN;
        else
            CC25XX_SPI_MOSI_OUT &= ~CC25XX_SPI_MOSI_PIN;
        // clk down
        CC25XX_SPI_SCK_OUT &= ~CC25XX_SPI_SCK_PIN;
    }
    return res;
#else 
    return SPI.transfer(value);
#endif
}

//uint8_t hal_spi_read_address(uint8_t address);
