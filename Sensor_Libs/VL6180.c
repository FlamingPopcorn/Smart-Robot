#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "Sensor_Libs/VL6180.h"

// ST AN4545 Mandatory "SR03" Tuning Registers
static const uint8_t vl6180_default_tuning[][3] = {
    {0x02, 0x07, 0x01}, {0x02, 0x08, 0x01}, {0x00, 0x96, 0x00},
    {0x00, 0x97, 0xFD}, {0x00, 0xE3, 0x00}, {0x00, 0xE4, 0x04},
    {0x00, 0xE5, 0x02}, {0x00, 0xE6, 0x01}, {0x00, 0xE7, 0x03},
    {0x00, 0xF5, 0x02}, {0x00, 0xD9, 0x05}, {0x00, 0xDB, 0xCE},
    {0x00, 0xDC, 0x03}, {0x00, 0xDD, 0xF8}, {0x00, 0x9F, 0x00},
    {0x00, 0xA3, 0x3C}, {0x00, 0xB7, 0x00}, {0x00, 0xBB, 0x3C},
    {0x00, 0xB2, 0x09}, {0x00, 0xCA, 0x09}, {0x01, 0x98, 0x01},
    {0x01, 0xB0, 0x17}, {0x01, 0xAD, 0x00}, {0x00, 0xFF, 0x05},
    {0x01, 0x00, 0x05}, {0x01, 0x99, 0x05}, {0x01, 0xA6, 0x1B},
    {0x01, 0xAC, 0x3E}, {0x01, 0xA7, 0x1F}, {0x00, 0x30, 0x00}
};

struct ToFSensor_VL6180* initToFSensor_VL6180(i2c_inst_t *i2c, uint8_t addr, uint XSHUTpin){
    struct ToFSensor_VL6180* ToFSensor_ptr = (struct ToFSensor_VL6180*) malloc(sizeof(struct ToFSensor_VL6180));
    if (ToFSensor_ptr == NULL) {
        printf("CRITICAL: Time of Flight Sensor Malloc Failed!\n");
        return NULL;
    }

    gpio_put(XSHUTpin, 1); 
    sleep_ms(5);

    // Ensure the sensor has booted
    uint8_t bootStatus;
    do {
        i2c_write_blocking (i2c, I2C_VL6810_ADDR, (uint8_t[]){0xFF & (FIRMWARE__BOOTUP >> 8), 0xFF & FIRMWARE__BOOTUP}, 2, true);    
		i2c_read_blocking(i2c, I2C_VL6810_ADDR, &bootStatus, 1, false);
		sleep_ms(2);
	} while (bootStatus == 0);

    if (addr != I2C_VL6810_ADDR){
        i2c_write_blocking (i2c, I2C_VL6810_ADDR, (uint8_t[]){0xFF & (I2C_SLAVE__DEVICE_ADDRESS >> 8), 0xFF & I2C_SLAVE__DEVICE_ADDRESS, addr}, 3, false); // Set User defined I2C address
    }

    // Load the mandatory SR03 Tuning Settings
    int num_tuning_regs = sizeof(vl6180_default_tuning) / sizeof(vl6180_default_tuning[0]);
    for (int i = 0; i < num_tuning_regs; i++) {
        // We pass 'false' for nostop here because we are writing independent sequences
        i2c_write_blocking(i2c, addr, vl6180_default_tuning[i], 3, false);
    }

    // Register 0x0016 must be cleared to 0x00 to stabilize the sensor state machine
    i2c_write_blocking(i2c, addr, (uint8_t[]){0xFF & (SYSTEM__FRESH_OUT_OF_RESET >> 8), 0xFF & SYSTEM__FRESH_OUT_OF_RESET, 0x00}, 3, false);

    // Initialize and configure sensor
	i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__INTERMEASUREMENT_PERIOD >> 8), 0xFF & SYSRANGE__INTERMEASUREMENT_PERIOD, 9}, 3, false); // Set Measurement period to 100ms
	i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__MAX_CONVERGENCE_TIME >> 8), 0xFF & SYSRANGE__MAX_CONVERGENCE_TIME, 100}, 3, false); // Set Max Convergence Time to 100ms
	i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (READOUT__AVERAGING_SAMPLE_PERIOD >> 8), 0xFF & READOUT__AVERAGING_SAMPLE_PERIOD, 48}, 3, false); // Set Averaging sampling period to datasheet suggested 48
    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (RANGE_SCALER >> 8), 0xFF & RANGE_SCALER, 0x00, 0x54}, 4, false); // Set Range Scale register to 3x

    // Scale Part Offset to account for range scale change
    uint8_t ptp_offset = 0;
    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__PART_TO_PART_RANGE_OFFSET >> 8), 0xFF & SYSRANGE__PART_TO_PART_RANGE_OFFSET}, 2, true);
    i2c_read_blocking(i2c, addr, &ptp_offset, 1, false);
    if (XSHUTpin == 22) {i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__PART_TO_PART_RANGE_OFFSET >> 8), 0xFF & SYSRANGE__PART_TO_PART_RANGE_OFFSET, (ptp_offset - 250) / 3}, 3, true);}
    else {i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__PART_TO_PART_RANGE_OFFSET >> 8), 0xFF & SYSRANGE__PART_TO_PART_RANGE_OFFSET, ptp_offset / 3}, 3, true);}
    
    uint8_t rce_reg = 0;
    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__RANGE_CHECK_ENABLES >> 8), 0xFF & SYSRANGE__RANGE_CHECK_ENABLES}, 2, true);
    i2c_read_blocking(i2c, addr, &rce_reg, 1, false);
    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__RANGE_CHECK_ENABLES >> 8), 0xFF & SYSRANGE__RANGE_CHECK_ENABLES, (rce_reg & (~0x01))}, 3, true);

    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (SYSRANGE__START >> 8), 0xFF & SYSRANGE__START, 0b11}, 3, false); // Start in continuous mode and start

    // Read Status Register
    i2c_write_blocking (i2c, addr, (uint8_t[]){0xFF & (RESULT__RANGE_STATUS >> 8), 0xFF & RESULT__RANGE_STATUS}, 2, true);
    i2c_read_blocking(i2c, addr, &ToFSensor_ptr->status, 1, false);

    ToFSensor_ptr->addr = addr;
    ToFSensor_ptr->i2c = i2c;
    ToFSensor_ptr->results = 0;
    ToFSensor_ptr->XSHUTpin = XSHUTpin;

    return ToFSensor_ptr;
}

void readDistance_VL6180(struct ToFSensor_VL6180* Sensor){

    // Read Status Register
    i2c_write_blocking (Sensor->i2c, Sensor->addr, (uint8_t[]){0xFF & (RESULT__RANGE_STATUS >> 8), 0xFF & RESULT__RANGE_STATUS}, 2, true);
    i2c_read_blocking(Sensor->i2c, Sensor->addr, &Sensor->status, 1, false);

    if (Sensor->status >> 4 != 0){
        Sensor->results = -1;
        return;
    }

    uint8_t rawRangeVal = 0;
    i2c_write_blocking(Sensor->i2c, Sensor->addr, (uint8_t[]){0xFF & (RESULT__RANGE_VAL >> 8), 0xFF & RESULT__RANGE_VAL}, 2, true);
    i2c_read_blocking(Sensor->i2c, Sensor->addr, &rawRangeVal, 1, false);

    Sensor->results = rawRangeVal * 3;

    i2c_write_blocking(Sensor->i2c, Sensor->addr, (uint8_t[]){0xff & (SYSTEM__INTERRUPT_CLEAR_VL6180 >> 8), 0xFF & SYSTEM__INTERRUPT_CLEAR_VL6180, 0x07}, 3, false);
}