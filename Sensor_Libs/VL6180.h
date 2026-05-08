#ifndef _VL6180_Sensor
#define _VL6180_Sensor

// Datasheet:
// https://www.st.com/resource/en/datasheet/vl6180.pdf

// Register group | Address range
// IDENTIFICATION   0x000 - 0x00F
// SYSTEM SETUP     0x010 - 0x017
// RANGE SETUP      0x018 - 0x037
// RESULTS          0x04D - 0x080

// Register address | Byte
// Address            MSB
// Address + 1        ..
// Address + 2        ..
// Address + 3        LSB

#define IDENTIFICATION__MODEL_ID                0x000
#define IDENTIFICATION__MODEL_REV_MAJOR         0x001
#define IDENTIFICATION__MODEL_REV_MINOR         0x002
#define IDENTIFICATION__MODULE_REV_MAJOR        0x003
#define IDENTIFICATION__MODULE_REV_MINOR        0x004
#define IDENTIFICATION__DATE_HI                 0x006
#define IDENTIFICATION__DATE_LO                 0x007
#define IDENTIFICATION__TIME                    0x008 // 0x008:0x009
#define SYSTEM__MODE_GPIO0                      0x010
#define SYSTEM__MODE_GPIO1                      0x011
#define SYSTEM__HISTORY_CTRL                    0x012
// #define SYSTEM__INTERRUPT_CONFIG_GPIO           0x014
#define SYSTEM__INTERRUPT_CLEAR_VL6180          0x015
#define SYSTEM__FRESH_OUT_OF_RESET              0x016
#define SYSTEM__GROUPED_PARAMETER_HOLD          0x017
#define SYSRANGE__START                         0x018
#define SYSRANGE__THRESH_HIGH                   0x019
#define SYSRANGE__THRESH_LOW                    0x01A
#define SYSRANGE__INTERMEASUREMENT_PERIOD       0x01B
#define SYSRANGE__MAX_CONVERGENCE_TIME          0x01C
#define SYSRANGE__CROSSTALK_COMPENSATION_RATE   0x01E
#define SYSRANGE__CROSSTALK_VALID_HEIGHT        0x021
#define SYSRANGE__EARLY_CONVERGENCE_ESTIMATE    0x022
#define SYSRANGE__PART_TO_PART_RANGE_OFFSET     0x024
#define SYSRANGE__RANGE_IGNORE_VALID_HEIGHT     0x025
#define SYSRANGE__RANGE_IGNORE_THRESHOLD        0x026
#define SYSRANGE__MAX_AMBIENT_LEVEL_MULT        0x02C
#define SYSRANGE__RANGE_CHECK_ENABLES           0x02D
#define SYSRANGE__VHV_RECALIBRATE               0x02E
#define SYSRANGE__VHV_REPEAT_RATE               0x031
#define RESULT__RANGE_STATUS                    0x04D
#define RESULT__INTERRUPT_STATUS_GPIO           0x04F
#define RESULT__HISTORY_BUFFER_x                0x052 // 0x052:0x060 (0x2)
#define RESULT__RANGE_VAL                       0x062
#define RESULT__RANGE_RAW                       0x064
#define RESULT__RANGE_RETURN_RATE               0x066
#define RESULT__RANGE_REFERENCE_RATE            0x068
#define RESULT__RANGE_RETURN_SIGNAL_COUNT       0x06C
#define RESULT__RANGE_REFERENCE_SIGNAL_COUNT    0x070
#define RESULT__RANGE_RETURN_AMB_COUNT          0x074
#define RESULT__RANGE_REFERENCE_AMB_COUNT       0x078
#define RESULT__RANGE_RETURN_CONV_TIME          0x07C
#define RESULT__RANGE_REFERENCE_CONV_TIME       0x080
#define READOUT__AVERAGING_SAMPLE_PERIOD        0x10A
#define FIRMWARE__BOOTUP                        0x119
#define I2C_SLAVE__DEVICE_ADDRESS               0x212

// Undocumented Registers
#define RANGE_SCALER    0x0096

#define I2C_VL6810_ADDR 0x29

#define VL6180_MAX_RANGE_LIMIT 350

struct ToFSensor_VL6180{
    uint8_t status;
    int16_t results;
    i2c_inst_t *i2c;
    uint16_t addr;
    uint XSHUTpin;
};

struct ToFSensor_VL6180* initToFSensor_VL6180(i2c_inst_t *i2c, uint8_t I2CDevAddr, uint XSHUTpin);
void readDistance_VL6180(struct ToFSensor_VL6180* Sensor);

#endif

// Address  |  Register name                    |    Reference
// 0x000       IDENTIFICATION__MODEL_ID              Section 6.2.1 on page 42
// 0x001       IDENTIFICATION__MODEL_REV_MAJOR       Section 6.2.2 on page 42
// 0x002       IDENTIFICATION__MODEL_REV_MINOR       Section 6.2.3 on page 42
// 0x003       IDENTIFICATION__MODULE_REV_MAJOR      Section 6.2.4 on page 43
// 0x004       IDENTIFICATION__MODULE_REV_MINOR      Section 6.2.5 on page 43
// 0x006       IDENTIFICATION__DATE_HI               Section 6.2.6 on page 43
// 0x007       IDENTIFICATION__DATE_LO               Section 6.2.7 on page 44
// 0x008:0x009 IDENTIFICATION__TIME                  Section 6.2.8 on page 44
// 0x010       SYSTEM__MODE_GPIO0                    Section 6.2.9 on page 45
// 0x011       SYSTEM__MODE_GPIO1                    Section 6.2.10 on page 46
// 0x012       SYSTEM__HISTORY_CTRL                  Section 6.2.11 on page 47
// 0x014       SYSTEM__INTERRUPT_CONFIG_GPIO         Section 6.2.12 on page 48
// 0x015       SYSTEM__INTERRUPT_CLEAR               Section 6.2.13 on page 48
// 0x016       SYSTEM__FRESH_OUT_OF_RESET            Section 6.2.14 on page 48
// 0x017       SYSTEM__GROUPED_PARAMETER_HOLD        Section 6.2.15 on page 49
// 0x018       SYSRANGE__START                       Section 6.2.16 on page 49 *
// 0x019       SYSRANGE__THRESH_HIGH                 Section 6.2.17 on page 50
// 0x01A       SYSRANGE__THRESH_LOW                  Section 6.2.18 on page 51
// 0x01B       SYSRANGE__INTERMEASUREMENT_PERIOD     Section 6.2.19 on page 51 *
// 0x01C       SYSRANGE__MAX_CONVERGENCE_TIME        Section 6.2.20 on page 51 *
// 0x01E       SYSRANGE__CROSSTALK_COMPENSATION_RATE Section 6.2.21 on page 52
// 0x021       SYSRANGE__CROSSTALK_VALID_HEIGHT      Section 6.2.22 on page 52
// 0x022       SYSRANGE__EARLY_CONVERGENCE_ESTIMATE  Section 6.2.23 on page 52
// 0x024       SYSRANGE__PART_TO_PART_RANGE_OFFSET   Section 6.2.24 on page 53
// 0x025       SYSRANGE__RANGE_IGNORE_VALID_HEIGHT   Section 6.2.25 on page 53
// 0x026       SYSRANGE__RANGE_IGNORE_THRESHOLD      Section 6.2.26 on page 53
// 0x02C       SYSRANGE__MAX_AMBIENT_LEVEL_MULT      Section 6.2.27 on page 54
// 0x02D       SYSRANGE__RANGE_CHECK_ENABLES         Section 6.2.27 on page 54
// 0x02E       SYSRANGE__VHV_RECALIBRATE             Section 6.2.29 on page 55
// 0x031       SYSRANGE__VHV_REPEAT_RATE             Section 6.2.30 on page 55
// 0x04D       RESULT__RANGE_STATUS                  Section 6.2.31 on page 56 *
// 0x04F       RESULT__INTERRUPT_STATUS_GPIO         Section 6.2.32 on page 57
// 0x052:0x060 (0x2) RESULT__HISTORY_BUFFER_x        Section 6.2.33 on page 57
// 0x062       RESULT__RANGE_VAL                     Section 6.2.34 on page 58 *
// 0x064       RESULT__RANGE_RAW                     Section 6.2.35 on page 58
// 0x066       RESULT__RANGE_RETURN_RATE             Section 6.2.36 on page 59
// 0x068       RESULT__RANGE_REFERENCE_RATE          Section 6.2.37 on page 60
// 0x06C       RESULT__RANGE_RETURN_SIGNAL_COUNT     Section 6.2.38 on page 60
// 0x070       RESULT__RANGE_REFERENCE_SIGNAL_COUNT  Section 6.2.39 on page 61
// 0x074       RESULT__RANGE_RETURN_AMB_COUNT        Section 6.2.40 on page 61
// 0x078       RESULT__RANGE_REFERENCE_AMB_COUNT     Section 6.2.41 on page 61
// 0x07C       RESULT__RANGE_RETURN_CONV_TIME        Section 6.2.42 on page 62
// 0x080       RESULT__RANGE_REFERENCE_CONV_TIME     Section 6.2.43 on page 62
// 0x10A       READOUT__AVERAGING_SAMPLE_PERIOD      Section 6.2.44 on page 62 *
// 0x119       FIRMWARE__BOOTUP                      Section 6.2.45 on page 62
// 0x212       I2C_SLAVE__DEVICE_ADDRESS             Section 6.2.46 on page 63 *

// Undocumented Registers
// 0x0096      RANGE_SCALER                          1x = 0x00FD, 2x = 0x007F, 3x = 0x0054