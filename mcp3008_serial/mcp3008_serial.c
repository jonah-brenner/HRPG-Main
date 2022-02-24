#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define PIN_MISO 4
#define PIN_CS   5
#define PIN_SCK  2
#define PIN_MOSI 3

#define SPI_PORT spi0
#define READ_BIT 0x80

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop");
}

uint16_t mcp0Read(uint adc_chan) {
  uint8_t writebuf[] = {0x01, 0b10000000, 0x00};
  writebuf[1] |= adc_chan << 4;
  uint8_t readbuf[] = {0x00, 0x00, 0x00};

  cs_select();
  spi_write_read_blocking(SPI_PORT, writebuf, readbuf, 3);
  cs_deselect();

  readbuf[1] &= 0b00000011;
  return (readbuf[1]<<8) | readbuf[2];
}

int main() {
    stdio_init_all();
    
    // This example will use SPI0 at 0.5MHz.
    spi_init(SPI_PORT, 500 * 1000);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);


    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    uint16_t sample;

    while (1) {
        sample = mcp0Read(0);

        float voltage = (sample / 1024.0) * 3.3;

        printf("Voltage: %f\n", voltage);

        sleep_ms(100);
    }

    return 0;
}