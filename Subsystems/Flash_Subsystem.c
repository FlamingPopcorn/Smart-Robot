#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/flash.h"
#include "hardware/flash.h"
#include "hardware/sync.h"
#include <string.h>

#include "Subsystems/Reflective_Sensor_Subsystem.h"
#include "Subsystems/Screen_Subsystem.h"

#include "Subsystems/Flash_Subsystem.h"

static const uint8_t *flash_calibration_target = (const uint8_t *) (XIP_BASE + FLASH_OFFSET_CALIBRATION);

void runCalibration(void){
    struct CalibrationData* calibrationData_ptr = (struct CalibrationData*) malloc(sizeof(struct CalibrationData));
    if (calibrationData_ptr == NULL) {
        printf("CRITICAL: Calibration Data Malloc Failed!\n");
        return;
    } calibrationData_ptr->magic_word = MAGIC_WORD;

    // Write the sensor values to menu and sampled calibration values to screen while calibratiuon running

    global_menu_ptr->currentPageID = 26;
    // Get White Values
    bool sensors_placed = false;
    // Loop until the flag becomes true
    while (!sensors_placed) {
        sensors_placed = true; // Assume they are placed correctly to start

        // Check every sensor
        for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++) {
            uint32_t reading = readReflectance(global_reflectiveSystem->Sensors[sensor]);
            
            // If ANY sensor is reading above the white threshold (seeing dark/air)
            if (reading > CALIBRATION_WHITE_THRESHOLD) {
                sensors_placed = false; // Disprove our assumption
                break;                  // Break out of the 'for' loop early—no need to check the rest!
            }
        }

        // If they weren't placed correctly, wait 50ms before checking the array again
        if (!sensors_placed) {
            sleep_ms(50);
        }
    }
    uint32_t lightSensorW[NUM_SENSORS] = {0};
    for (int i = 0; i < OVERSAMPLING_CALIBRATION; i++){
        for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++){
            lightSensorW[sensor] += readReflectance(global_reflectiveSystem->Sensors[sensor]);
        }
    }
    for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++){
        calibrationData_ptr->white[sensor] = lightSensorW[sensor] / OVERSAMPLING_CALIBRATION;
    }
    printf("Light level measured for white: %u, %u, %u, %u\n", calibrationData_ptr->white[0], calibrationData_ptr->white[1], calibrationData_ptr->white[2], calibrationData_ptr->white[3]);

    global_menu_ptr->currentPageID = 27;
    // Get Black Values
    sensors_placed = false;
    // Loop until the flag becomes true
    while (!sensors_placed) {
        sensors_placed = true; // Assume they are placed correctly to start

        // Check every sensor
        for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++) {
            uint32_t reading = readReflectance(global_reflectiveSystem->Sensors[sensor]);
            
            // If ANY sensor is reading above the white threshold (seeing dark/air)
            if (reading < CALIBRATION_BLACK_THRESHOLD) {
                sensors_placed = false; // Disprove our assumption
                break;                  // Break out of the 'for' loop early—no need to check the rest!
            }
        }

        // If they weren't placed correctly, wait 50ms before checking the array again
        if (!sensors_placed) {
            sleep_ms(50);
        }
    }
    uint32_t lightSensorB[NUM_SENSORS] = {0};
    for (int i = 0; i < OVERSAMPLING_CALIBRATION; i++){
        for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++){
            lightSensorB[sensor] += readReflectance(global_reflectiveSystem->Sensors[sensor]);
        }
    }
    for (uint8_t sensor = 0; sensor < NUM_SENSORS; sensor++){
        calibrationData_ptr->black[sensor] = lightSensorB[sensor] / OVERSAMPLING_CALIBRATION;
    }
    printf("Light level measured for black: %u, %u, %u, %u\n", calibrationData_ptr->black[0], calibrationData_ptr->black[1], calibrationData_ptr->black[2], calibrationData_ptr->black[3]);

    bool saved = saveCalibration(calibrationData_ptr);
    global_menu_ptr->currentPageID = saved ? 28 : 29;
    
    free(calibrationData_ptr);
}

bool loadCalibration(struct CalibrationData* data){
    if (((struct CalibrationData*)flash_calibration_target)->magic_word == MAGIC_WORD){
        for (int i = 0; i < sizeof(data); i++){
            memcpy(data, flash_calibration_target, sizeof(struct CalibrationData));
        }
        return true;
    } else {
        return false;
    }
}

static void eraseCalibration(void *param){
    uint32_t offset = (uint32_t)param;
    flash_range_erase(offset, FLASH_SECTOR_SIZE);
}

static void writeCalibration(void *param){
    uint32_t offset = ((uintptr_t*)param)[0];
    const uint8_t* buffer = (const uint8_t*)((uintptr_t*)param)[1];
    flash_range_program(offset, buffer, FLASH_PAGE_SIZE);
}

static bool saveCalibration(struct CalibrationData* data){
    // Clear Flash for new calibration to be stored
    int returnCode = flash_safe_execute(eraseCalibration, (void *)FLASH_OFFSET_CALIBRATION , FLASH_TIMEOUT);

    if (returnCode == PICO_OK){
        // Can only write in multiples of 256 bytes
        uint8_t buffer[FLASH_PAGE_SIZE] = {0};
        memcpy(buffer, data, sizeof(struct CalibrationData));

        // pack data and offset into a single parameter poiter to pass
        uintptr_t params[] = { FLASH_OFFSET_CALIBRATION, (uintptr_t)buffer};
        returnCode = flash_safe_execute(writeCalibration, (void *)params , FLASH_TIMEOUT);

        // Handle Error codes how we want from flash_safe_execute()
        switch(returnCode){
            case PICO_OK:                           // on success (the function will have been called)
                return true;
            case PICO_ERROR_TIMEOUT:                // on timeout (the function may have been called)
                return false;
            case PICO_ERROR_NOT_PERMITTED:          // if safe execution is not possible (the function will not have been called)
                return false;
            case PICO_ERROR_INSUFFICIENT_RESOURCES: // if the method fails due to dynamic resource exhaustion (the function will not have been called)
                return false;
            default:
                return false;                       // Always return just incase
        }
    } else return false; // failed to clear flash
}