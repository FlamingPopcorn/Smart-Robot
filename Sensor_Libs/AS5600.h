#ifndef _AS5600_Sensor
#define _AS5600_Sensor

#include "hardware/i2c.h"

// Constants for AS5600
#define AS5600_ADDR 0x36
#define CONF_REG    0x07 // Register for Configuration

#define ENCODER_LEFT 26
#define ENCODER_RIGHT 27

#define OVERSAMPLE 8

#define GPIO_TO_ADC(pin) (uint8_t)((pin) - 26)

struct MagEncoder{
    uint pin;
    uint8_t adcChannel;
    i2c_inst_t *i2c;
    bool reversed;
    uint16_t count;
    float lastTheta;
    float theta;
    float relEncoder;
    float relEncoder_goal;
    float velocity;
};

struct MagEncoder* initMagEncoder(uint pin, bool reversed);
void readMagEncoder(struct MagEncoder* encoder);
void readMagEncoder_I2C(struct MagEncoder* encoder);
float calculateVelocity(struct MagEncoder* encoder, float dt);
void resetRelEncoder(struct MagEncoder* encoder);
void setRelEncoderGoal(struct MagEncoder* encoder, float goal);


#endif