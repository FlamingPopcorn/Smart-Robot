#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"

#include "Subsystems/PID_Subsystem.h"
struct PID_Controller* drive_pid = NULL;
struct PID_Controller* turn_pid = NULL;

struct PID_Controller* left_pid = NULL;
struct PID_Controller* right_pid = NULL;

// Initialize the global navigation targets
volatile float target_drive_rps = 0.0f;
volatile float target_turn_rps = 0.0f;

struct PID_Controller* initPID_Controller(float Kp, float Ki, float Kd, float Kf,
                                            float out_min, float out_max, float T){
    struct PID_Controller* pid_ptr = (struct PID_Controller*) malloc(sizeof(struct PID_Controller));
    if (pid_ptr == NULL) {
        printf("CRITICAL: PID Controller Malloc Failed!\n");
        return NULL;
    }

    pid_ptr->Kp = Kp;
    pid_ptr->Ki = Ki;
    pid_ptr->Kd = Kd;
    pid_ptr->Kf = Kf;

    pid_ptr->integrator = 0;
    pid_ptr->prev_error = 0;
    pid_ptr->prev_input = 0;

    pid_ptr->setpoint = 0;
    pid_ptr->input = 0;

    pid_ptr->out_min = out_min;
    pid_ptr->out_max = out_max;

    pid_ptr->T = T;

    return pid_ptr;
}

float pidController(struct PID_Controller* controller){
    float input = controller->input;
    float setpoint = controller->setpoint;

    // Find the error
    float error = setpoint - input;

    // Proportional
    float proportional = controller->Kp * error;
    
    // Integral (Trapezoidal integration)
    controller->integrator += ((controller->prev_error + error) / 2.0) * controller->T;
    float integral = controller->Ki * controller->integrator;

    // Integral Clamping (Anti-Windup)
    // Limit the integrator so it can't exceed the max/min output on its own
    if (integral > controller->out_max) controller->integrator = controller->out_max / controller->Ki;
    else if (integral < controller->out_min) controller->integrator = controller->out_min / controller->Ki;
    
    // Derivative (Change in error / time)
    float derivative = -controller->Kd * (input - controller->prev_input) / controller->T;

    // Feedforward (Directly proportional to setpoint)
    float feedforward = controller->Kf * setpoint;
    
    // Sum the terms
    float total_output = proportional + integral + derivative + feedforward;

    // Save for next loop
    controller->prev_error = error;
    controller->prev_input = input;
    
    // Clamping / Anti-Windup Based on controller max and min
    if (total_output > controller->out_max) return controller->out_max;
    if (total_output < controller->out_min) return controller->out_min;

    return total_output;
}