#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/pio.h"
#include "clock.pio.h"

#define PIN_MISO   4
#define PIN_CS_MCP 5
#define PIN_SCK    2
#define PIN_MOSI   3
#define PIN_CS_ADS 6
#define PIN_ADS_CLOCK 17

#define SPI_PORT spi0
#define READ_BIT 0x80

#define MODE_REG_ADR 0x2


uint16_t ads_read(uint adc_chan) {
    uint16_t writebuf[] = {0x00, 0x00, 0x00, 0x00};
    uint16_t readbuf[] = {0x00, 0x00, 0x00, 0x00};

    gpio_put(PIN_CS_ADS, 0);
    spi_write16_read16_blocking(SPI_PORT, writebuf, readbuf, 4);
    gpio_put(PIN_CS_ADS, 1);

    return readbuf[1];
}

uint16_t ads_read_reg(uint16_t reg_address) {
    uint16_t writebuf[] = {0b1010000000000000, 0x00, 0x00, 0x00};
    writebuf[0] = writebuf[0] | reg_address << 7;

    uint16_t readbuf[] = {0x00, 0x00, 0x00, 0x00};

    gpio_put(PIN_CS_ADS, 0);
    spi_write16_read16_blocking(SPI_PORT, writebuf, readbuf, 4);
    gpio_put(PIN_CS_ADS, 1);

    gpio_put(PIN_CS_ADS, 0);
    spi_write16_read16_blocking(SPI_PORT, writebuf, readbuf, 4);
    gpio_put(PIN_CS_ADS, 1);

    return readbuf[0];
}

uint16_t ads_write_reg(uint16_t reg_address, uint16_t reg_data) {
    uint16_t writebuf[] = {0b0110000000000000, reg_data, 0x00, 0x00};
    writebuf[0] = writebuf[0] | reg_address << 7;

    uint16_t readbuf[] = {0x00, 0x00, 0x00, 0x00};

    gpio_put(PIN_CS_ADS, 0);
    spi_write16_read16_blocking(SPI_PORT, writebuf, readbuf, 4);
    gpio_put(PIN_CS_ADS, 1);

    return readbuf[0];
}

void ads_reset() {
    uint16_t writebuf[] = {0b0000000000010001, 0x00, 0x00, 0x00};
    uint16_t readbuf[] = {0x00, 0x00, 0x00, 0x00};

    gpio_put(PIN_CS_ADS, 0);
    spi_write16_read16_blocking(SPI_PORT, writebuf, readbuf, 4);
    gpio_put(PIN_CS_ADS, 1);
}

int main() {
    stdio_init_all();
    
    // This example will use SPI0 at 0.5MHz.
    spi_init(SPI_PORT, 1000 * 1000);
    spi_set_format(SPI_PORT, 16, 0, 1, SPI_MSB_FIRST);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(PIN_CS_MCP);
    gpio_set_dir(PIN_CS_MCP, GPIO_OUT);
    gpio_put(PIN_CS_MCP, 1);

    gpio_init(PIN_CS_ADS);
    gpio_set_dir(PIN_CS_ADS, GPIO_OUT);
    gpio_put(PIN_CS_ADS, 1);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &clock_program);

    clock_program_init(pio, sm, offset, PIN_ADS_CLOCK, 8192 * 1000);

    uint16_t sample;
    uint16_t reg;

    sleep_ms(1);

    ads_reset();

    sleep_ms(1);

    ads_write_reg(MODE_REG_ADR, 0b0000010000010000); //0x410

    while (1) {
        //sample = ads_read(0);

        //printf("Voltage: %u\n", sample);

        reg = ads_read_reg(MODE_REG_ADR);

        printf("MODE Register Contents: %x\n", reg);
    }

    return 0;
}