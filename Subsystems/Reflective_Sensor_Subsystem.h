#ifndef _Reflective_Sensor_Subsystem
#define _Reflective_Sensor_Subsystem

// #include "pico/stdlib.h"
#include "hardware/pio.h"

#define PIO_BLOCK_REFLECTANCE pio1
#define REFLECTIVE_SENSOR_PIN_LEFT_OUTSIDE 18
#define REFLECTIVE_SENSOR_PIN_RIGHT_OUTSIDE 21
#define REFLECTIVE_SENSOR_PIN_LEFT_INSIDE 19
#define REFLECTIVE_SENSOR_PIN_RIGHT_INSIDE 20 // Modded

#define REFLECTIVE_SENSORS ((const int[]){REFLECTIVE_SENSOR_PIN_LEFT_OUTSIDE, REFLECTIVE_SENSOR_PIN_LEFT_INSIDE, REFLECTIVE_SENSOR_PIN_RIGHT_INSIDE, REFLECTIVE_SENSOR_PIN_RIGHT_OUTSIDE})
#define NUM_SENSORS 4

#define LEFT_OUTSIDE    -1.0/3.0
#define LEFT_INSIDE     -1.0/8.0
#define RIGHT_INSIDE     1.0/8.0
#define RIGHT_OUTSIDE    1.0/3.0

// #define SENSOR_WEIGHTS ((const int[]){LEFT_OUTSIDE, LEFT_INSIDE, RIGHT_INSIDE, RIGHT_OUTSIDE})

extern struct ReflectiveSystem* global_reflectiveSystem;

struct ReflectiveSystem{
    struct ReflectiveSensor* Sensors[4];
    PIO pio;
    uint offset;
    struct CalibrationData* calibration;
    bool calibrationLoaded;
    float lineErrorOutput;
};

// https://www.pololu.com/product/959 Link for more information
struct ReflectiveSensor{
    int pin;
    PIO pio;
    uint sm;
    uint32_t output;
    float normalizedOutput;
};

// enum ADC_Channels = {26 = 0, 27 = 1, 28 = 2, 29 = 3};

struct ReflectiveSystem* initReflectiveSystem(PIO pio);
static struct ReflectiveSensor* initReflectiveSensor(int pin, PIO pio, uint offset);
uint32_t readReflectance(struct ReflectiveSensor*);
float normalize_sensor(int index, struct ReflectiveSystem*);
float calculate_line_error(struct ReflectiveSystem*);

#endif