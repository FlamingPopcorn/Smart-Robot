#include <stdio.h>
#include <math.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/pwm.h"    // Might want for PID/variable speed control
#include "hardware/pio.h"

#include "Multithreading.h"

#include "Subsystems/Drive_Subsystem.h"
#include "Subsystems/PID_Subsystem.h"

#include "Sensor_Libs/AS5600.h"

// #include "tachometerCounter.pio.h"
// #include "magEncoder.pio.h"

// Global pointers for the ISR to access
struct Drivetrain* global_drivetrain;

struct Drivetrain* initDrivetrain(int pinLFWD, int pinLRVS, int pinRFWD, int pinRRVS){
    struct Drivetrain* drivetrain_ptr = (struct Drivetrain*) malloc(sizeof(struct Drivetrain));
    if (drivetrain_ptr == NULL) {
        printf("CRITICAL: Drivetrain Malloc Failed!\n");
        return NULL;
    }

    // init Motors
    struct Motor* motorL = initMotor(pinLFWD, pinLRVS);
    struct Motor* motorR = initMotor(pinRFWD, pinRRVS);

    drivetrain_ptr->motorL = motorL;
    drivetrain_ptr->motorR = motorR;

    // printf("Starting Left Encoder\n");
    drivetrain_ptr->motorL->encoder = initMagEncoder(ENCODER_LEFT, false);
    drivetrain_ptr->motorL->encoder->i2c = i2c0;
    // printf("Starting Right Encoder\n");
    drivetrain_ptr->motorR->encoder = initMagEncoder(ENCODER_RIGHT, true);
    drivetrain_ptr->motorR->encoder->i2c = i2c1;

    drivetrain_ptr->velocity = 0;
    drivetrain_ptr->heading = 0;

    drivetrain_ptr->mode = Drive_mode;

    global_drivetrain = drivetrain_ptr;

    drivetrain_ptr->motorL->pid = left_pid;
    drivetrain_ptr->motorR->pid = right_pid;

    setDrivetrainVelocity(drivetrain_ptr, 0, 0);

    return drivetrain_ptr;
}

static struct Motor* initMotor(int pinFWD, int pinRVS){
    struct Motor* motor_ptr = (struct Motor*) malloc(sizeof(struct Motor));
    if (motor_ptr == NULL) {
        printf("CRITICAL: Motor Malloc Failed!\n");
        return NULL;
    }

    motor_ptr->velocity = 0;
    motor_ptr->filtered_rps = 0;

    gpio_set_function(pinFWD, GPIO_FUNC_PWM);
    gpio_set_function(pinRVS, GPIO_FUNC_PWM);

    // Get the PWM slice for these pins
    uint sliceFWD = pwm_gpio_to_slice_num(pinFWD);
    uint sliceRVS = pwm_gpio_to_slice_num(pinRVS);

    // Standard 1kHz - 20kHz frequency setup
    // 150,000,000 / (1.0 * 60000) = 2.5kHz
    pwm_set_clkdiv(sliceFWD, 1.0f); 
    pwm_set_wrap(sliceFWD, PWM_MAX); // 0 to 60000 range for speed resolution
    pwm_set_enabled(sliceFWD, true);

    pwm_set_clkdiv(sliceRVS, 1.0f);
    pwm_set_wrap(sliceRVS, PWM_MAX);
    pwm_set_enabled(sliceRVS, true);

    motor_ptr->pinFWD = pinFWD;
    motor_ptr->pinRVS = pinRVS;

    motor_ptr->pwmSliceFWD = sliceFWD;
    motor_ptr->pwmSliceRVS = sliceRVS;

    pwm_set_gpio_level(pinFWD, 0);
    pwm_set_gpio_level(pinRVS, 0);

    pwm_set_phase_correct(sliceFWD, true);
    pwm_set_phase_correct(sliceRVS, true);

    return motor_ptr;
}

void setDrivePWM(struct Motor* motor){
    gpio_set_function(motor->pinFWD, GPIO_FUNC_PWM);
    gpio_set_function(motor->pinRVS, GPIO_FUNC_PWM);

    // Standard 1kHz - 20kHz frequency setup
    // 150,000,000 / (1.0 * 60000) = 2.5kHz
    pwm_set_clkdiv(motor->pwmSliceFWD, 1.0f); 
    pwm_set_wrap(motor->pwmSliceFWD, PWM_MAX); // 0 to 60000 range for speed resolution
    pwm_set_enabled(motor->pwmSliceFWD, true);

    pwm_set_clkdiv(motor->pwmSliceRVS, 1.0f);
    pwm_set_wrap(motor->pwmSliceRVS, PWM_MAX);
    pwm_set_enabled(motor->pwmSliceRVS, true);

    pwm_set_gpio_level(motor->pinFWD, 0);
    pwm_set_gpio_level(motor->pinRVS, 0);
}

void setDrivetrainVelocity(struct Drivetrain* drivetrain, float left_rps, float right_rps){
    if (drivetrain->mode == Music) {setDrivePWM(drivetrain->motorL);setDrivePWM(drivetrain->motorR);}
    // Normalize and scale directly to PWM (No mixing)
    int32_t left_pwm = (int32_t)((left_rps / MAX_RPS) * PWM_MAX_f);
    int32_t right_pwm = (int32_t)((right_rps / MAX_RPS) * PWM_MAX_f);

    drivetrain->motorL->velocity = left_pwm;
    drivetrain->motorR->velocity = right_pwm;

    setMotorVelocity(drivetrain->motorL, left_pwm);
    setMotorVelocity(drivetrain->motorR, right_pwm);

    // setMotorVelocity(drivetrain->motorL, 60000);
    // setMotorVelocity(drivetrain->motorR, 60000);
    return;
}

void DifferentialDrive(struct Drivetrain* drivetrain, float drive_rps, float turn_rps){

    float left_rps = drive_rps + turn_rps;
    float right_rps = (drive_rps - turn_rps) * 1.20;

    // Normalize and scale to PWM (PWM_MAX)
    int32_t left_pwm = (int32_t)((left_rps / MAX_RPS) * PWM_MAX_f);
    int32_t right_pwm = (int32_t)((right_rps / MAX_RPS) * PWM_MAX_f);

    // Normalize if either exceeds PWM_MAX to maintain turn radius
    // int32_t max_val = abs(left_pwm) > abs(right_pwm) ? abs(left_pwm) : abs(right_pwm);
    // if (max_val > PWM_MAX) {
    //     left_pwm = (left_pwm * PWM_MAX) / max_val;
    //     right_pwm = (right_pwm * PWM_MAX) / max_val;
    // }

    drivetrain->motorL->velocity = left_pwm;
    drivetrain->motorR->velocity = right_pwm;

    setMotorVelocity(drivetrain->motorL, left_pwm);
    setMotorVelocity(drivetrain->motorR, right_pwm);

    return;
}

void setMotorVelocity(struct Motor* motor, int32_t velocity){

    // velocity should be between -PWM_MAX and PWM_MAX
    uint32_t speed = abs(velocity);
    if (speed > PWM_MAX) speed = PWM_MAX; 

    if (velocity > 0) {
        pwm_set_gpio_level(motor->pinFWD, speed);
        pwm_set_gpio_level(motor->pinRVS, 0);
    } else if (velocity < 0) {
        pwm_set_gpio_level(motor->pinFWD, 0);
        pwm_set_gpio_level(motor->pinRVS, speed);
    } else {
        pwm_set_gpio_level(motor->pinFWD, 0);
        pwm_set_gpio_level(motor->pinRVS, 0);
    }    
    return;
}

bool rotateDistance(struct Drivetrain* drivetrain, bool drive){
    enum Core1Mode mode = core1CurrentMode;
    core1CurrentMode = Skip;

    float left_error = drivetrain->motorL->encoder->relEncoder_goal - drivetrain->motorL->encoder->relEncoder;
    float right_error = drivetrain->motorR->encoder->relEncoder_goal - drivetrain->motorR->encoder->relEncoder;

    float diff_error = left_error - right_error;
    float avg_error = (left_error - right_error) / 2.0;

    float timeout = 2.0;
    absolute_time_t now = get_absolute_time();
    absolute_time_t start = now;
    while (((now - start) < timeout) && (fabsf(diff_error) > 0.05 || fabsf(avg_error) > 0.05)){
        
        if(fabsf(diff_error) > 0.25){
            target_turn_rps = (MAX_RPS * 0.75) * (diff_error > 0 ? 1.0 : -1.0);
        } else {
            target_turn_rps = 0.75 * (diff_error > 0 ? 1.0 : -1.0);
        }

        if(drive && (fabsf(avg_error) > 0.25)){
            target_drive_rps = (MAX_RPS * 0.75) * (avg_error > 0 ? 1.0 : -1.0);
        } else {
            target_drive_rps = 0.75 * (avg_error > 0 ? 1.0 : -1.0);
        }

        float left_error = drivetrain->motorL->encoder->relEncoder_goal - drivetrain->motorL->encoder->relEncoder;
        float right_error = drivetrain->motorR->encoder->relEncoder_goal - drivetrain->motorR->encoder->relEncoder;

        float diff_error = left_error - right_error;
        float avg_error = (left_error - right_error) / 2.0;

        absolute_time_t now = get_absolute_time();
    }

    core1CurrentMode = mode;
    return true;
}