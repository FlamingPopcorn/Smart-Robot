#include <stdio.h>
#include <math.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "Sensor_Libs/VL53L1x/VL53L1X_api.h"
#include "Sensor_Libs/VL53L0x/vl53l0x_api.h"
// #include "Sensor_Libs/VL6180.h"

#include "Subsystems/Distance_Sensor_Subsystem.h"

struct DistanceSystem* global_distanceSystem;

struct DistanceSystem* initDistanceSystem(i2c_inst_t *i2c){
    struct DistanceSystem* DistanceSystem_ptr = (struct DistanceSystem*) malloc(sizeof(struct DistanceSystem));
    if (DistanceSystem_ptr == NULL) {
        printf("CRITICAL: Distance System Malloc Failed!\n");
        return NULL;
    }

    struct distOutput* distOutput_ptr = (struct distOutput*) malloc(sizeof(struct distOutput));
    if (distOutput_ptr == NULL) {
        printf("CRITICAL: Distance System Output Malloc Failed!\n");
        return NULL;
    }

    DistanceSystem_ptr->i2c = i2c;
    distOutput_ptr->speed = 0.0f;
    distOutput_ptr->turn = 0.0f;
    distOutput_ptr->isInDeadEnd = false;
    DistanceSystem_ptr->Output = distOutput_ptr;

    // init all 3 ToF sensors
    for (int i = 0; i < 3; i++){
        gpio_init(XSHUT_PINS[i]);
        gpio_set_dir(XSHUT_PINS[i], GPIO_OUT);
        gpio_put(XSHUT_PINS[i], 0);
    }
    
    VL53L1X_SetI2CInstance(I2C_VL53L1X_INST);
    DistanceSystem_ptr->Center = initToFSensor_VL53L1x(I2C_CENTER_ADDR, XSHUT_PINS[0]);
    DistanceSystem_ptr->Left = initToFSensor_VL53L0x(i2c, I2C_LEFT_ADDR, XSHUT_PINS[1]);
    DistanceSystem_ptr->Right = initToFSensor_VL53L0x(i2c, I2C_RIGHT_ADDR, XSHUT_PINS[2]);

    global_distanceSystem = DistanceSystem_ptr;

    return DistanceSystem_ptr;
}

struct ToFSensor_VL53L1x* initToFSensor_VL53L1x(uint8_t I2CDevAddr, uint XSHUTpin){
    struct ToFSensor_VL53L1x* ToFSensor_ptr = (struct ToFSensor_VL53L1x*) malloc(sizeof(struct ToFSensor_VL53L1x));
    if (ToFSensor_ptr == NULL) {
        printf("CRITICAL: Time of Flight Sensor Malloc Failed!\n");
        return NULL;
    }

    VL53L1X_Result_t* VL53L1X_Result_t_ptr = (VL53L1X_Result_t*) malloc(sizeof(VL53L1X_Result_t));
    if (VL53L1X_Result_t_ptr == NULL) {
        printf("CRITICAL: Time of Flight Sensor Results Malloc Failed!\n");
        return NULL;
    }

    gpio_put(XSHUTpin, 1);
    sleep_ms(5);

    // Ensure the sensor has booted
    VL53L1X_Status_t status = 0;
    uint8_t bootState;
    do {
		status += VL53L1X_BootState(I2C_VL53L1X_ADDR, &bootState);
		VL53L1X_WaitMs(I2C_VL53L1X_ADDR, 2);
	} while (bootState == 0);

    if (I2CDevAddr != I2C_VL53L1X_ADDR){
        VL53L1X_SetI2CAddress(I2C_VL53L1X_ADDR, I2CDevAddr << 1);
    }

    // Initialize and configure sensor
	status = VL53L1X_SensorInit(I2CDevAddr);
	status += VL53L1X_SetDistanceMode(I2CDevAddr, 1);
	status += VL53L1X_SetTimingBudgetInMs(I2CDevAddr, 100);
	status += VL53L1X_SetInterMeasurementInMs(I2CDevAddr, 100);
	status += VL53L1X_StartRanging(I2CDevAddr);
    VL53L1X_Result_t_ptr->status = status;

    ToFSensor_ptr->I2CDevAddr = I2CDevAddr;
    ToFSensor_ptr->results = VL53L1X_Result_t_ptr;
    ToFSensor_ptr->status = 0;
    ToFSensor_ptr->XSHUTpin = XSHUTpin;

    // VL53L1X_ClearInterrupt(ToFSensor_ptr->I2CDevAddr);
    return ToFSensor_ptr;
}

// Could do return type bool for non blocking check if function returned true meaning there is new data in struct->results
void readDistance_VL53L1x(struct ToFSensor_VL53L1x* Sensor){
    // Wait until we have new data
    // uint8_t dataReady; uint8_t count = 0;
    // do {
    //     printf("Waiting for sensor to have data\n");
    //     Sensor->status = VL53L1X_CheckForDataReady(Sensor->I2CDevAddr, &dataReady);
    //     count++;
    //     sleep_us(1);
    // } while (dataReady == 0 && count < 50);

    // Read and display result
    Sensor->status += VL53L1X_GetResult(Sensor->I2CDevAddr, Sensor->results);
    if (Sensor->results->status != 0  || Sensor->results->distance > 4000) {
        // Sensor->results->distance = 65535;
    } else if (Sensor->results->distance < 500){
        Sensor->prevOutput = Sensor->results->distance;
    }

    if (Sensor->results->distance > 1300 || Sensor->results->distance == 0) {
        Sensor->results->distance = Sensor->prevOutput;
    }
    // printf("Status = %2d, dist = %5d, Ambient = %2d, Signal = %5d, #ofSpads = %5d\n",
    //     Sensor->results->status, Sensor->results->distance, Sensor->results->ambient, Sensor->results->sigPerSPAD, Sensor->results->numSPADs);

    // Clear the sensor for a new measurement
    Sensor->status += VL53L1X_ClearInterrupt(Sensor->I2CDevAddr);
}

struct ToFSensor_VL53L0x* initToFSensor_VL53L0x(i2c_inst_t *i2c, uint8_t I2CDevAddr, uint XSHUTpin) {
    struct ToFSensor_VL53L0x* ToFSensor_ptr = (struct ToFSensor_VL53L0x*) malloc(sizeof(struct ToFSensor_VL53L0x));
    if (ToFSensor_ptr == NULL) return NULL;

    VL53L0X_RangingMeasurementData_t* results_ptr = (VL53L0X_RangingMeasurementData_t*) malloc(sizeof(VL53L0X_RangingMeasurementData_t));
    if (results_ptr == NULL) return NULL;

    ToFSensor_ptr->results = results_ptr;
    ToFSensor_ptr->I2CDevAddr = I2CDevAddr;
    ToFSensor_ptr->XSHUTpin = XSHUTpin;

    // Device Setup
    ToFSensor_ptr->Device.address = I2C_VL53L0X_ADDR; // Default boot address
    ToFSensor_ptr->I2CDevAddr = I2C_VL53L0X_ADDR;
    ToFSensor_ptr->Device.i2c = I2C_VL53L0X_INST; 

    gpio_put(XSHUTpin, 1);
    sleep_ms(10); // L0X needs slightly longer to stabilize

    // Data Init
    ToFSensor_ptr->status = VL53L0X_DataInit(&ToFSensor_ptr->Device);

    // Set New Address if needed
    if (I2CDevAddr != I2C_VL53L0X_ADDR) {
        ToFSensor_ptr->status += VL53L0X_SetDeviceAddress(&ToFSensor_ptr->Device, I2CDevAddr << 1);
        ToFSensor_ptr->Device.address = I2CDevAddr;
        ToFSensor_ptr->I2CDevAddr = I2CDevAddr;
    }


    // Static Init & Calibration
    ToFSensor_ptr->status += VL53L0X_StaticInit(&ToFSensor_ptr->Device);
    
    uint8_t vhv, phase;
    ToFSensor_ptr->status += VL53L0X_PerformRefCalibration(&ToFSensor_ptr->Device, &vhv, &phase);
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    ToFSensor_ptr->status += VL53L0X_PerformRefSpadManagement(&ToFSensor_ptr->Device, &refSpadCount, &isApertureSpads);

    // Configure for Navigation
    ToFSensor_ptr->status += VL53L0X_SetDeviceMode(&ToFSensor_ptr->Device, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
    
    // Equivalent to your 100ms timing budget
    ToFSensor_ptr->status += VL53L0X_SetLimitCheckValue(&ToFSensor_ptr->Device, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.25*65536));
    ToFSensor_ptr->status += VL53L0X_SetMeasurementTimingBudgetMicroSeconds(&ToFSensor_ptr->Device, 100000);

    // Start
    ToFSensor_ptr->status += VL53L0X_StartMeasurement(&ToFSensor_ptr->Device);

    return ToFSensor_ptr;
}

void readDistance_VL53L0x(struct ToFSensor_VL53L0x* Sensor) {
    // Read the result from the ST API
    Sensor->status = VL53L0X_GetRangingMeasurementData(&Sensor->Device, Sensor->results);

    // RangeStatus 0 is "Range Valid"
    // If status is not 0 (e.g., Sigma Fail, Signal Fail, Out of Bounds), 
    // set it to 65535 to match L1X behavior.
    if (Sensor->results->RangeStatus != 0 || Sensor->results->RangeMilliMeter > 2000) {
        // Sensor->results->RangeMilliMeter = 65535;
    } else if (Sensor->results->RangeMilliMeter < 300){
        Sensor->prevOutput = Sensor->results->RangeMilliMeter;
    }

    if (Sensor->results->RangeMilliMeter > 1300 || Sensor->results->RangeMilliMeter == 0) {
        Sensor->results->RangeMilliMeter = Sensor->prevOutput;
    }

    // Clear interrupt to allow the sensor to trigger the next measurement
    VL53L0X_ClearInterruptMask(&Sensor->Device, VL53L0X_REG_SYSTEM_INTERRUPT_GPIO_NEW_SAMPLE_READY);
}

static float lockedTurnDir = 0.0f;
void avoidWalls(struct DistanceSystem* distanceSystem){
    uint8_t active_sensors = 0;

    // Get Distances from the Various Sensors
    uint16_t centerDist = distanceSystem->Center->results->distance;
    uint16_t leftDist = distanceSystem->Left->results->RangeMilliMeter - 20;
    uint16_t rightDist = distanceSystem->Right->results->RangeMilliMeter - 25;

    // Check which sensors see walls
    if (centerDist != NO_WALL) active_sensors++;
    if (leftDist != NO_WALL) active_sensors++;
    if (rightDist != NO_WALL) active_sensors++;

    // Handle "No walls Detected"
    if (active_sensors == 0) {
        // Strategy: Return (1.0, 0.0) to say just proceed forward
        distanceSystem->Output->speed = 0.20;
        distanceSystem->Output->turn = 0.0;
        return; 
    }

    // Drive Speed proportional to open space in front of us
    if (centerDist > 100){ // was 1000
        distanceSystem->Output->speed = 0.35;
    } else if (centerDist == NO_WALL) {
        distanceSystem->Output->speed = 0.20f; // Cautionary speed when "blind" ahead
    } // else if (centerDist < 15){
        //distanceSystem->Output->speed = -0.40;
    //} 
    else {
        // distanceSystem->Output->speed = sqrtf(sqrtf(centerDist / 1000.0));  // 4th root: 50% power at ~6.25cm | favor farther distances way more than y = x
        // distanceSystem->Output->speed = cbrtf(centerDist / 1000.0);         // 3rd root: 50% power at ~12.5cm
        distanceSystem->Output->speed = 0.35 * sqrt(centerDist / 100.0);         // 2nd root: 50% power at ~25cm
        // distanceSystem->Output->speed = 0.35 * (centerDist / 100.0);
        // distanceSystem->Output->speed = 0.35 * (centerDist / 150.0) * (centerDist / 150.0);
    }

    // printf("input dists: %5d | %5d | %5d\n", leftDist, centerDist, rightDist);

    float l = fminf((float)leftDist, TURN_AWAY_THRESHOLD);
    float r = fminf((float)rightDist, TURN_AWAY_THRESHOLD);
    // printf("%4.2f | %4.2f\n", l, r);

    float turnAway = 0.20 * (fabsf((r - l) / TURN_AWAY_THRESHOLD)) * ((r - l) > 0 ? 1.0 : -1.0);      
    if (fabsf(turnAway) > 0.02f){
        distanceSystem->Output->turn = turnAway;  // Push away from closer wall within (TURN_AWAY_THRESHOLD)cm
        distanceSystem->Output->speed *= (0.75 - (fabsf(distanceSystem->Output->turn)));  // Slow down based on how much we are turning
    } else {
        distanceSystem->Output->turn = 0.0f;
    }
 
    // printf("%4.2f | %4.2f\n", distanceSystem->Output->speed, distanceSystem->Output->turn);
    
    // Dead End Hysteresis
    if (centerDist < 20) {
        distanceSystem->Output->isInDeadEnd = true;
        distanceSystem->Output->speed = -0.25f; // Gentle reverse
        // Pivot away from whichever side is closer while backing up one time decision
        if (lockedTurnDir == 0.0f) lockedTurnDir = (leftDist > rightDist) ? -0.45f : 0.45f;
        distanceSystem->Output->turn = lockedTurnDir;
    } else if (centerDist > 70) { 
        // Only exit dead end mode once we've backed up significantly
        distanceSystem->Output->isInDeadEnd = false;
        lockedTurnDir = 0.0f;
    }

    // Return the distOutput variable
    return;
}