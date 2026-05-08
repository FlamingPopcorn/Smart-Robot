#ifndef _Distance_Sensor_Subsystem
#define _Distance_Sensor_Subsystem

#include "Sensor_Libs/VL53L1x/VL53L1X_types.h"
#include "Sensor_Libs/VL53L1x/VL53L1X_api.h"

#include "Sensor_Libs/VL53L0x/vl53l0x_types.h"
#include "Sensor_Libs/VL53L0x/vl53l0x_api.h"

// #include "Sensor_Libs/VL6180.h"

#define I2C_VL53L1X_INST i2c1
#define I2C_VL53L0X_INST i2c1

#define I2C_VL53L1X_ADDR 0x29
#define I2C_VL53L0X_ADDR 0x29

#define I2C_CENTER_ADDR 0x11
#define I2C_LEFT_ADDR   0x12
#define I2C_RIGHT_ADDR  0x13

#define XSHUT_PINS ((const int[]){17, 16, 22})

#define NO_WALL 65535
#define TURN_AWAY_THRESHOLD 100.0f

extern struct DistanceSystem* global_distanceSystem;

struct DistanceSystem{
    struct ToFSensor_VL53L1x* Center;
    struct ToFSensor_VL53L0x* Left;
    struct ToFSensor_VL53L0x* Right;
    i2c_inst_t *i2c;
    struct distOutput* Output;
};

struct ToFSensor_VL53L1x{
    VL53L1X_Status_t status;
    VL53L1X_Result_t* results;
    uint8_t I2CDevAddr;
    uint XSHUTpin;
    uint prevOutput;
};

struct ToFSensor_VL53L0x {
    uint8_t I2CDevAddr;
    uint XSHUTpin;
    VL53L0X_Dev_t Device; // The ST API device instance
    VL53L0X_RangingMeasurementData_t* results;
    VL53L0X_Error status;
    uint prevOutput;
};

struct distOutput{
    float speed;
    float turn;
    bool isInDeadEnd;
};

struct DistanceSystem* initDistanceSystem(i2c_inst_t *i2c);
struct ToFSensor_VL53L1x* initToFSensor_VL53L1x(uint8_t I2CDevAddr, uint XSHUTpin);
void readDistance_VL53L1x(struct ToFSensor_VL53L1x* Sensor);

struct ToFSensor_VL53L0x* initToFSensor_VL53L0x(i2c_inst_t *i2c, uint8_t I2CDevAddr, uint XSHUTpin);
void readDistance_VL53L0x(struct ToFSensor_VL53L0x* Sensor);

void avoidWalls(struct DistanceSystem* distanceSystem);

#endif