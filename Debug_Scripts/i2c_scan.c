#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"


void scan_i2c(i2c_inst_t *i2c){
  // Scan for devices on the I2C Bus
    printf("\n--- I2C Bus Scan ---\n");
    uint8_t rxdata; // This defines the missing variable
    int found_devices = 0;

    for (int addr = 0; addr < (1 << 7); ++addr) {
        // Perform a 1-byte read from the address
        int ret = i2c_read_blocking(i2c, addr, &rxdata, 1, false);
        
        if (ret >= 0) {
            printf("Found device at 0x%02x\n", addr);
            found_devices++;
        }
    }

    if (found_devices == 0) {
        printf("No I2C devices found. Check your wiring and power!\n");
    }
    printf("--------------------\n\n");
}