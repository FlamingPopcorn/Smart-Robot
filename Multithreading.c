#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"
#include "hardware/pio.h"

#include "Multithreading.h"

// Subsystem Driver Libs
#include "Subsystems/Drive_Subsystem.h"
#include "Subsystems/Screen_Subsystem.h"
#include "Subsystems/Reflective_Sensor_Subsystem.h"
#include "Subsystems/Distance_Sensor_Subsystem.h"
#include "Subsystems/PID_Subsystem.h"
#include "Subsystems/Remote_Control_Subsystem.h"

#include "Sensor_Libs/AS5600.h"
// #include "Sensor_Libs/VL6180.h"

static void mode_LineFollowing(struct ReflectiveSystem* reflectiveSystem);
static void mode_MazeSolving(struct DistanceSystem* distanceSystem);
static void mode_RemoteControl(struct IBus* ibus);

volatile enum Core1Mode core1CurrentMode = Idle;
volatile bool core1Booting = true;

volatile float drive_pid_input = 0.0f;
volatile float turn_pid_input = 0.0f;

struct repeating_timer pid_timer;
struct repeating_timer control_timer;
volatile uint32_t last_update_ms = 0;

static absolute_time_t last_time;

void initCore1(void){
    multicore_reset_core1();
    multicore_launch_core1(core1_main_entry_point);
}

void change_core1_mode(enum Core1Mode new_mode) {
    // Send the enum value to Core 1
    core1CurrentMode = new_mode;
}

bool pidUpdate_timer_callback(struct repeating_timer *t) {
    absolute_time_t now = get_absolute_time();
    // Calculate actual time passed in seconds
    float dt = absolute_time_diff_us(last_time, now) / 1000000.0f;
    last_time = now;

    // Ensure we don't divide by zero if the loop runs too fast
    if (dt < 0.0001f) return true;

    // float left_target = drive_pid->setpoint + turn_pid->setpoint;
    // float right_target = drive_pid->setpoint - turn_pid->setpoint;

    // Read Encoders
    if (core1CurrentMode == Idle){
        readMagEncoder(global_drivetrain->motorL->encoder);
        readMagEncoder(global_drivetrain->motorR->encoder);
    } else {
        readMagEncoder_I2C(global_drivetrain->motorL->encoder);
        readMagEncoder_I2C(global_drivetrain->motorR->encoder);
    }

    float current_speed_l = calculateVelocity(global_drivetrain->motorL->encoder, dt);
    float current_speed_r = calculateVelocity(global_drivetrain->motorR->encoder, dt);

    // printf("Hello from core1! Wheel Speeds are: %4.3lf, %4.3f\n", current_speed_l, current_speed_r);

    // Indepentdent Inputs
    left_pid->input = current_speed_l;
    right_pid->input = current_speed_r;

    // Update PID inputs
    // For a simple speed controller, input is the average of both wheels
    // float drive_input = (current_speed_l + current_speed_r) / 2.0f;

    // For turn/heading, can use the difference
    // This helps the robot drive straight if one motor is stronger than the other
    // float turn_input = current_speed_l - current_speed_r;

    // drive_pid->input = drive_input;
    // turn_pid->input = turn_input;

    switch(core1CurrentMode){
            case Idle:
                drive_pid->setpoint = (0.0); 
                turn_pid->setpoint = (0.0);
                break;
            case Line_Following:
                // mode_LineFollowing(global_reflectiveSystem);
                break;
            case Maze_Solving:
                // mode_MazeSolving(global_distanceSystem);
                break;
            case Remote_Control:
                // mode_RemoteControl(global_ibus);
                break;
            case Music:
                drive_pid->setpoint = (0.0); 
                turn_pid->setpoint = (0.0);
                target_drive_rps = (0.0);
                target_turn_rps = (0.0);
                break;
            case Skip: // Use for skip mode check
                break;
            default:
                core1CurrentMode = Idle;
                break;
        }

    // Mix the setpoints
    left_pid->setpoint = target_drive_rps + target_turn_rps;
    right_pid->setpoint = target_drive_rps - target_turn_rps;

    return true; 
}

void startI2C1(void){
    gpio_init(I2C1_SDA);
    gpio_init(I2C1_SCL);
    gpio_disable_pulls(I2C1_SDA);
    gpio_disable_pulls(I2C1_SCL);

    // I2C Initialisation. Using it at 400kHz.
    i2c_init(I2C1_PORT, 400*1000);
    
    gpio_set_function(I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA);
    gpio_pull_up(I2C1_SCL);
    
    return;
}

#include "Debug_Scripts/i2c_scan.h"
static void core1_main_entry_point(void){
    // init I2C1
    startI2C1();

    // Debug Print all devices found of the i2c1 bus 
    // scan_i2c(I2C1_PORT);
    // scan_i2c(i2c0);

    // init Reflective Sensor Subsystem
    struct ReflectiveSystem* reflectiveSystem = initReflectiveSystem(pio1);
    // printf("Hello from core1!\n");
    // init Distance Sensor Subsystem
    struct DistanceSystem* distanceSystem = initDistanceSystem(I2C1_PORT);
    // printf("Hello from core1!\n");
    // init IBus Subsystem for Remote Control
    struct IBus* ibus = initIBus(UART_IBUS, UART_IBUS_RX);
    
    last_time = get_absolute_time();
    
    // Launch timer on Core 1
    add_repeating_timer_ms(-1000*LEFT_PERIOD, pidUpdate_timer_callback, NULL, &pid_timer);

    printf("Hello from core1!\n"); core1Booting = false;
    while(1){
        switch(core1CurrentMode){
            case Idle:
                // printf("Mode: %i - Starting core1 Idle Mode\n", core1CurrentMode);
                sleep_ms(300);
                break;
            case Line_Following:
                // printf("Mode: %i - Starting core1 Line Following Mode\n", core1CurrentMode);
                mode_LineFollowing(reflectiveSystem);
                // sleep_ms(1);
                break;
            case Maze_Solving:
                // printf("Mode: %i - Starting core1 Maze Solving Mode\n", core1CurrentMode);
                // printf("Status = %2d, dist = %5d, Ambient = %2d, Signal = %5d, #ofSpads = %5d\n",
                //         distanceSystem->Center->results->status, distanceSystem->Center->results->distance, distanceSystem->Center->results->ambient, distanceSystem->Center->results->sigPerSPAD, distanceSystem->Center->results->numSPADs);
                // printf("dists: %5d | %5d | %5d\n", distanceSystem->Left->results, distanceSystem->Center->results->distance, distanceSystem->Right->results);
                mode_MazeSolving(distanceSystem);
                sleep_ms(10);
                break;
            case Remote_Control:
                // printf("Mode: %i - Starting core1 Remote Control Mode\n", core1CurrentMode);
                mode_RemoteControl(ibus);
                sleep_ms(1);
                break;
            case Music:
                sleep_ms(300);
                break;
            case Skip:
                break;
            default:
                // printf("Mode: %i - Does Not exist. Returning core1 to Idle Mode\n", core1CurrentMode);
                core1CurrentMode = Idle;
                break;
        }
    }
}

static void mode_LineFollowing(struct ReflectiveSystem* reflectiveSystem){
    for (int i = 0; i < NUM_SENSORS; i++){
        uint32_t raw_val = readReflectance(reflectiveSystem->Sensors[i]);
        float normalizedOutput = normalize_sensor(i, reflectiveSystem);
        // printf("Output: %u, %4.2f\n",raw_val, normalizedOutput);
    }

    float line_error = calculate_line_error(reflectiveSystem);

    // need to figure out line_error to RPS
        // maybe the closer to 0.0 error the faster we go?
    
    float target_speed = 0.35 - (fabsf(line_error) / 2.0);
    float target_turn = (fabsf(line_error)) * (line_error > 0 ? 1.0 : -1.0);

    // printf("Line Error: %4.2f\n",line_error);
    // Normalize set point to +/- MAX_RPS
    target_drive_rps = (target_speed) * MAX_RPS; 
    target_turn_rps = (target_turn) * MAX_RPS;

    return;
}

static void mode_MazeSolving(struct DistanceSystem* distanceSystem){
    readDistance_VL53L1x(distanceSystem->Center);
    readDistance_VL53L0x(distanceSystem->Left);
    readDistance_VL53L0x(distanceSystem->Right);

    avoidWalls(distanceSystem);
    
    float target_speed;
    float target_turn;

    target_speed = distanceSystem->Output->speed;
    target_turn = distanceSystem->Output->turn;

    // Normalize set point to +/- MAX_RPS
    target_drive_rps = target_speed * MAX_RPS; 
    target_turn_rps = target_turn * MAX_RPS;

    return;
}

static void mode_RemoteControl(struct IBus* ibus){
    readIBus(ibus);
    // printf("IBus: Ch1: %u, Ch2: %u, Ch3: %u, Ch4: %u, Ch5: %u, Ch6: %u\n", ibus->channels[0]->value, ibus->channels[1]->value, ibus->channels[2]->value, ibus->channels[3]->value, ibus->channels[4]->value, ibus->channels[5]->value);

    // Map 1000-2000 iBus to +/- 1.0 percent duty cycle
    float target_speed = (ibus->channels[CHANNEL_LSTICK_Y - 1]->value - 1500) / 500.0f;
    float target_turn = (ibus->channels[CHANNEL_RSTICK_X - 1]->value - 1500) / 500.0f;

    // Deadzones
    if (fabsf(target_speed) < 0.05f) target_speed = 0.0f;
    if (fabsf(target_turn) < 0.05f) target_turn = 0.0f;

    // if (target_speed == 0.0) drive_pid->integrator = 0;

    // // printf("Speed: %f, %f | Turn: %f, %f \n", target_speed, abs(target_speed), target_turn, abs(target_turn));
    // // Normalize set point to +/- MAX_RPS
    // drive_pid->setpoint = (target_speed * MAX_RPS); 
    // turn_pid->setpoint = (target_turn * MAX_RPS);

    target_drive_rps = target_speed * MAX_RPS;
    target_turn_rps = target_turn * MAX_RPS;

    // Mix the Speed and Turn setpoints into individual wheel targets
    // float left_target  = (target_speed + target_turn) * MAX_RPS;
    // float right_target = (target_speed - target_turn) * MAX_RPS;

    // // Update the actual PIDs being used in the motor control loop
    // left_wheel_pid->setpoint  = left_target;
    // right_wheel_pid->setpoint = right_target;

    // Reset integrators if stopped to prevent "jump" when starting
    if (target_speed == 0.0f && target_turn == 0.0f) {
        left_pid->integrator = 0;
        right_pid->integrator = 0;
    }
    return;
}

void start_control_loop() {
    // Negative delay (-1000) means "start to start" timing
    // This ensures exactly 1kHz even if the PID math takes 100us
    add_repeating_timer_ms(-1000*LEFT_PERIOD, control_motors_timer_callback, NULL, &control_timer);
}

// This function runs every 1000 microseconds (1ms) for pid_timer
// bool control_motors_timer_callback(struct repeating_timer *t){
//     watchdog_update();

//     float linear_out = 0;
//     float angular_out = 0;

//     //  Run PID
//     if (core1CurrentMode != Idle){
//         // (Assuming Setpoints were set by Core 1)
//         linear_out = pidController(drive_pid);
//         angular_out = pidController(turn_pid);
//     }

//     DifferentialDrive(global_drivetrain, linear_out, angular_out);
//      return true;
// }

bool control_motors_timer_callback(struct repeating_timer *t){
    watchdog_update();

    float left_out = 0;
    float right_out = 0;

    // Run Independent PIDs
    if (core1CurrentMode != Idle){
        left_out = pidController(left_pid);
        right_out = pidController(right_pid);
    }

    // Call the drive function
    if (global_drivetrain->mode == Drive_mode)    
        setDrivetrainVelocity(global_drivetrain, left_out, right_out);
    return true;
}