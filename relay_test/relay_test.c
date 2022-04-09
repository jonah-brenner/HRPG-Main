#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"

int main() {
    stdio_init_all();
    
    gpio_init(15);
    gpio_set_dir(15, 1);
    gpio_put(15, 0);

    while (1) {
        gpio_put(15, 0);
        sleep_ms(500);
        gpio_put(15, 1);
        sleep_ms(500);
    }
}