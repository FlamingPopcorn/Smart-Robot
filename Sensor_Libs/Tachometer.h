#ifndef _Tachometer_Sensor
#define _Tachometer_Sensor

#define TACHOMETER_PIO pio0
#define TACHOMETER_L 14
#define TACHOMETER_R 15

#define PULSES_PER_REV 40.0f

struct Tachometer{
    int pin;
    PIO pio;
    uint sm;
    uint32_t last_count;
};

float get_signed_wheel_speed(struct Motor*, float delta_t, float setpoint, float current_pwm);
struct Tachometer* init_tachometer(int pin, PIO pio, uint offset);

#endif