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
  uint8_t writebuf[] = {0b00000110, 0b00000000, 0x00};
  writebuf[1] |= (adc_chan << 6) & 0b11000000;
  writebuf[0] |= (adc_chan >> 2) & 0b00000001;
  uint8_t readbuf[] = {0x00, 0x00, 0x00};

  cs_select();
  spi_write_read_blocking(SPI_PORT, writebuf, readbuf, 3);
  cs_deselect();

  readbuf[1] &= 0b00001111;
  return (readbuf[1]<<8) | readbuf[2];
}

int main() {
    stdio_init_all();
    
    // Set up pin 15 as relay output, initialize to off
    gpio_init(15);
    gpio_set_dir(15, 1);
    gpio_put(15, 0);



    stdio_init_all();
    
    // This example will use SPI0 at 0.5MHz.
    spi_init(SPI_PORT, 1500 * 1000);
    spi_set_format(SPI_PORT, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);

    uint16_t sample1;
    uint16_t sample2;

    while (1) {
        sample2 = mcp0Read(1);
        //sample2 = mcp0Read(2);

        // Resolution: 256 (8-bit) 1024 (10-bit) 4096 (12-bit)

        float voltage1 = (sample1 / 4096.0) * 3.3;
        float voltage2 = (sample2 / 4096.0) * 3.3;

        // printf("Voltage 1: %f, Voltage 2: %f\n", voltage1, voltage2);

        float pressure1 = (sample1 / 4096.0) * 500 - 80;
        float pressure2 = (sample2 / 4096.0) * 500 - 80;

        printf("Voltage: %f, Pressure: %f\n", voltage2, pressure2);

        // printf("Pressure 1: %f, Pressure 2: %f\n", pressure1, pressure2);

        sleep_ms(20);
        gpio_put(PICO_DEFAULT_LED_PIN, 0);
        sleep_ms(20);
        gpio_put(PICO_DEFAULT_LED_PIN, 1);
    }
}