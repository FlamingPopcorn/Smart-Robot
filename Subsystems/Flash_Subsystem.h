#ifndef _Flash_Subsystem
#define _Flash_Subsystem

#include "Subsystems/Reflective_Sensor_Subsystem.h"

#define FLASH_OFFSET_CALIBRATION (1536 * 1024) // 1.5MB
#define MAGIC_WORD 0x12345678

#define OVERSAMPLING_CALIBRATION 16

#define FLASH_TIMEOUT 1000

#define CALIBRATION_WHITE_THRESHOLD 25000
#define CALIBRATION_BLACK_THRESHOLD 100000

struct CalibrationData{
    uint32_t magic_word;
    uint32_t white[NUM_SENSORS];
    uint32_t black[NUM_SENSORS];
};

void runCalibration(void);
bool loadCalibration(struct CalibrationData*);
static bool saveCalibration(struct CalibrationData*);

#endif