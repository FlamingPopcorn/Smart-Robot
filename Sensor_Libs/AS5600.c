#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include "Subsystems/PID_Subsystem.h"
#include "Subsystems/Drive_Subsystem.h"

#include "Sensor_Libs/AS5600.h"

// #include "magEncoder.pio.h"

struct MagEncoder* initMagEncoder(uint pin, bool reversed){
    struct MagEncoder* encoder_ptr = (struct MagEncoder*) malloc(sizeof(struct MagEncoder));
    if (encoder_ptr == NULL) {
        printf("CRITICAL: Magnetic Encoder Malloc Failed!\n");
        return NULL;
    }

    adc_gpio_init(pin);

    encoder_ptr->pin = pin;
    encoder_ptr->adcChannel = GPIO_TO_ADC(pin);
    encoder_ptr->reversed = reversed;

    encoder_ptr->theta = 0;
    encoder_ptr->lastTheta = 0;
    encoder_ptr->relEncoder = 0;
    encoder_ptr->relEncoder_goal = 0;
    encoder_ptr->velocity = 0;

    readMagEncoder(encoder_ptr);

    return encoder_ptr;
}

void readMagEncoder(struct MagEncoder* encoder){
    adc_select_input(encoder->adcChannel);
    uint32_t raw = 0;
    for (int i = 0; i < OVERSAMPLE; i++){
        raw += adc_read();
    }
    float sample = (raw / OVERSAMPLE);
    
    encoder->count = (uint16_t)(sample);
    encoder->lastTheta = encoder->theta;
    
    encoder->theta = (sample * 360.0f) / 4095.0f; // the encoder is [0..4095] counts
    // printf("Theta: %f\n", encoder->theta);
}

void readMagEncoder_I2C(struct MagEncoder* encoder) {
    // uint8_t status_reg = 0x0B;
    // uint8_t status_val;
    // i2c_write_blocking(i2c0, 0x36, &status_reg, 1, true);
    // i2c_read_blocking(i2c0, 0x36, &status_val, 1, false);
    // printf("Sensor Status: 0x%02X\n", status_val);
    
    uint8_t reg = 0x0E; // RAW ANGLE Register (High Byte)
    uint8_t data[2];

    // Read 2 bytes from the AS5600
    int ret = i2c_write_blocking(encoder->i2c, 0x36, &reg, 1, true);
    if (ret != PICO_ERROR_GENERIC) {
        i2c_read_blocking(encoder->i2c, 0x36, data, 2, false);
        
        // Combine MSB and LSB (12-bit value: 0-4095)
        uint16_t raw_angle = ((uint16_t)data[0] << 8) | data[1];
        
        encoder->lastTheta = encoder->theta;
        encoder->theta = (raw_angle * 360.0f) / 4095.0f;
    } //else printf("Failed to read sensor\n");
}

float calculateVelocity(struct MagEncoder* encoder, float dt){
    // Calculate the "Shortest Path" Delta
    float delta = encoder->theta - encoder->lastTheta;

    if (delta > 180.0f)  delta -= 360.0f;      // It wrapped around backwards
    else if (delta < -180.0f) delta += 360.0f; // It wrapped around forwards

    // if (fabsf(delta) > 25.0f) {
    //     return encoder->velocity; // Use the previous good velocity
    // }
    float max_degrees = (MAX_RPS * 1.5) * 360.0f * dt; 
    if (fabsf(delta) > max_degrees) {
        return encoder->velocity; 
    }

    if (encoder->reversed) delta = -delta;

    // Convert to Rotations Per Second
    float raw_rps = (delta / 360.0f) / dt;

    if (raw_rps > MAX_RPS * 1.15) raw_rps = MAX_RPS * 1.15;
    if (raw_rps < -MAX_RPS * 1.15) raw_rps = -MAX_RPS * 1.15;

    encoder->relEncoder += delta / 360.0 * WHEEL_CIRCUMFERENCE;

    // Apply Alpha Filter
    // Lower alpha (0.1) = smoother but more lag
    // Higher alpha (0.3) = faster response but more jitter
    const float alpha = 0.1f;
    encoder->velocity = (alpha * raw_rps) + ((1.0f - alpha) * encoder->velocity);
    // printf("delta Theta: %f | dt: %f\n", delta, dt);
    return encoder->velocity;
}

void resetRelEncoder(struct MagEncoder* encoder){
    encoder->relEncoder = 0.0;
}

void setRelEncoderGoal(struct MagEncoder* encoder, float goal){
    encoder->relEncoder_goal = goal;
}