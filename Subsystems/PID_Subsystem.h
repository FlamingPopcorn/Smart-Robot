#ifndef _PID_Subsystem
#define _PID_Subsystem

#define MAX_RPS             1.50f  // Adjust based on motor's actual top speed
                                  // was 1.50, 2.90, 1.60, 2.0, and 2.65f
// Drive PID
#define DRIVE_PID_INPUT_ID  0
#define DRIVE_PID_SET_ID    1

#define DRIVE_P             0.20f // 0.5
#define DRIVE_I             0.00f
#define DRIVE_D             0.00f
#define DRIVE_FF            1.15f // 1.15

#define DRIVE_OUT_MIN      -MAX_RPS
#define DRIVE_OUT_MAX       MAX_RPS
#define DRIVE_PERIOD        0.020f

// Turn PID
#define TURN_PID_INPUT_ID   2
#define TURN_PID_SET_ID     3

#define TURN_P              0.40f // 0.8
#define TURN_I              0.05f
#define TURN_D              0.0f
#define TURN_FF             1.1f

#define TURN_OUT_MIN       -MAX_RPS
#define TURN_OUT_MAX        MAX_RPS
#define TURN_PERIOD         0.020f

// Decoupled Independent Velocity PIDs | 20ms Constnts
#define LEFT_P          0.05f //
#define LEFT_I          0.05f
#define LEFT_D          0.02f
#define LEFT_FF         1.38f 
#define LEFT_PERIOD     0.010f

#define RIGHT_P         0.05f // Start identical to left
#define RIGHT_I         0.05f
#define RIGHT_D         0.02f // 0.05f
#define RIGHT_FF        1.30f
#define RIGHT_PERIOD    0.010f

// External pointers for your Left and Right controllers
extern struct PID_Controller* left_pid;
extern struct PID_Controller* right_pid;

// Global Navigation Setpoints (Your modes will write to these)
extern volatile float target_drive_rps;
extern volatile float target_turn_rps;

struct PID_Controller{
    // Tuning Parameters
    float Kp;  // P
    float Ki;  // I
    float Kd;  // D
    float Kf;  // Feed forward

    // Dynamics
    volatile float integrator;          // Accumulated error for Integral term
    float prev_error;          // Previous error for Derivative
    float prev_input;          // Previous input to avoid derivative kick
                                // when setpoint changes
    // Desired Value
    volatile float setpoint;
    volatile float input;

    // Controller Limits
    float out_min;
    float out_max;

    // Timing
    float T;      // Sampling Period
};

struct PID_Controller* initPID_Controller(float Kp, float Ki, float Kd, float Kf, float out_min, float out_max, float T);
float pidController(struct PID_Controller* controller);

#endif