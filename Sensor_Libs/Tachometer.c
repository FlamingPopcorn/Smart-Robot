#include "Sensors/Tachometer.h"

// Globals to store previous counts for delta calculation
static uint32_t last_count_l = 0;
static uint32_t last_count_r = 0;

struct Tachometer* init_tachometer(int pin, PIO pio, uint offset){
    struct Tachometer* tach_ptr = (struct Tachometer*) malloc(sizeof(struct Tachometer));
    if (tach_ptr == NULL) {
        printf("CRITICAL: Tachometer Malloc Failed!\n");
        return NULL;
    }

    tach_ptr->pin = pin;
    tach_ptr->pio = pio;
    tach_ptr->last_count = 0;

    tach_ptr->sm = pio_claim_unused_sm(pio, true); // Find a free state machine
    
    tachometerCounter_program_init(pio, tach_ptr->sm, offset, pin);

    return tach_ptr;
}

// uint32_t get_raw_count(struct Tachometer* tachometer) {
//     uint32_t latest = tachometer->last_count;
//     // Drain the FIFO to get the absolute newest value
//     while (pio_sm_get_rx_fifo_level(tachometer->pio, tachometer->sm) > 0) {
//         latest = 0xFFFFFFFF - pio_sm_get_blocking(tachometer->pio, tachometer->sm);
//     }
//     tachometer->last_count = latest;
//     return latest;
// }

uint32_t get_raw_count(struct Tachometer* tachometer) {
    // Force the SM to move X into the ISR
    pio_sm_exec(tachometer->pio, tachometer->sm, pio_encode_mov(pio_isr, pio_x));
    // Force the SM to push the ISR into the FIFO
    pio_sm_exec(tachometer->pio, tachometer->sm, pio_encode_push(false, false));
    // Read the value from the FIFO
    uint32_t latest = pio_sm_get(tachometer->pio, tachometer->sm);
    
    // X counts DOWN, so we invert it to get a UP count
    tachometer->last_count = 0xFFFFFFFF - latest;
    return tachometer->last_count;
}

// Function to calculate speed (RPM or m/s)
float get_wheel_speed(struct Motor* motor, float delta_t){
    uint32_t prev_count = motor->tachometer->last_count;
    uint32_t current = get_raw_count(motor->tachometer);
    uint32_t delta_pulses = current - prev_count;
    
    // if (delta_pulses == 0) {
    //     // Drop the speed slowly rather than snapping to zero immediately
    //     // This prevents the PID from "panicking"
    //     motor->filtered_rps *= 0.5f; 
    //     if (motor->filtered_rps < 0.05f) motor->filtered_rps = 0;
    //     return motor->filtered_rps;
    // }

    // Speed in Revolutions per Second
    float raw_rps = (float)delta_pulses / (PULSES_PER_REV * delta_t);

    // Apply Alpha Filter
    // Lower alpha (0.1) = smoother but more lag
    // Higher alpha (0.3) = faster response but more jitter
    const float alpha = 0.1f;
    motor->filtered_rps = (alpha * raw_rps) + ((1.0f - alpha) * motor->filtered_rps);

    return motor->filtered_rps;
}

float get_signed_wheel_speed(struct Motor* motor, float delta_t, float setpoint, float current_pwm){
    float speed = get_wheel_speed(motor, delta_t);
    
    // If PWM is negative, the pulses we just counted happened in reverse
    // Use a small deadzone for sign-flipping to prevent jitter at idle
    // if (setpoint > 0.1f) {
    //     motor->last_dir = 1.0f;
    // } else if (setpoint < -0.1f) {
    //     motor->last_dir = -1.0f;
    // }

    if (current_pwm >= 0){
        motor->last_dir = 1;
    } else {
        motor->last_dir = -1;
    }

    return speed * motor->last_dir;
}