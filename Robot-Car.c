#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "hardware/i2c.h"       // For more I2C examples see https://github.com/raspberrypi/pico-examples/tree/master/i2c
#include "hardware/adc.h"
#include "hardware/pio.h"       // For more pio examples see https://github.com/raspberrypi/pico-examples/tree/master/pio
#include "hardware/watchdog.h"

// Sensor Libs
#include "Sensor_Libs/ssd1306.h"
#include "Sensor_Libs/AS5600.h"

// True Multithreading
#include "Multithreading.h"

// Subsystem Driver Libs
#include "Subsystems/Drive_Subsystem.h"
#include "Subsystems/Screen_Subsystem.h"
#include "Subsystems/Reflective_Sensor_Subsystem.h"
#include "Subsystems/Distance_Sensor_Subsystem.h"
#include "Subsystems/PID_Subsystem.h"
#include "Subsystems/Remote_Control_Subsystem.h"
#include "Subsystems/Music_Subsystem.h"

// PIO asm Programs
#include "pollReflectance.pio.h"
#include "menuButtons.pio.h"

// Debug Libs
#include "Debug_Scripts/i2c_scan.h"

// I2C0 defines
#define I2C0_PORT i2c0
#define I2C0_SDA 4
#define I2C0_SCL 5

void startI2C0(void){
    gpio_init(I2C0_SDA);
    gpio_init(I2C0_SCL);
    gpio_disable_pulls(I2C0_SDA);
    gpio_disable_pulls(I2C0_SCL);

    // I2C Initialisation. Using it at 400kHz.
    i2c_init(I2C0_PORT, 400*1000);
    
    gpio_set_function(I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA);
    gpio_pull_up(I2C0_SCL);
    
    return;
}

int main(){
    stdio_init_all();

    adc_init();

    multicore_reset_core1();

    pio_clear_instruction_memory(pio0);
    pio_clear_instruction_memory(pio1);
    pio_clear_instruction_memory(pio2);

    sleep_ms(3000); 
    
    // Check if we rebooted because of a watchdog timeout
    if (watchdog_caused_reboot()) {
        printf("Rebooted from Watchdog!\n");
    } else printf("Rebooting System...\n");

    if (cyw43_arch_init()) return -1;

    printf("Starting I2C...\n");
    startI2C0();
    // adc_init();

    printf("Initializing Drivetrain System...\n");
    // init Drivetrain Subsystem
    struct Drivetrain* drivetrain = initDrivetrain(MOTORL_FWD, MOTORL_RVS,
                                                   MOTORR_FWD, MOTORR_RVS);

    // playMusic(11);
    uint replayCount = 2;
    for (int i = 0; i < replayCount; i++){
        for(int j = 1; j < 12; j++){
            playMusic(j);
        }
    }

    printf("Initializing PID Controllers...\n");
    // init PID Controllers
    // drive_pid = initPID_Controller(DRIVE_P, DRIVE_I, DRIVE_D, DRIVE_FF,
    //                                DRIVE_OUT_MIN, DRIVE_OUT_MAX, DRIVE_PERIOD);
    // turn_pid = initPID_Controller(TURN_P, TURN_I, TURN_D, TURN_FF, 
    //                               TURN_OUT_MIN, TURN_OUT_MAX, TURN_PERIOD);

    // Initialize Left Wheel PID
    left_pid = initPID_Controller(LEFT_P, LEFT_I, LEFT_D, LEFT_FF,
                                  -MAX_RPS, MAX_RPS, LEFT_PERIOD); 
                                  
    // Initialize Right Wheel PID
    right_pid = initPID_Controller(RIGHT_P, RIGHT_I, RIGHT_D, RIGHT_FF, 
                                   -MAX_RPS, MAX_RPS, RIGHT_PERIOD);
    
    // Debug Print all devices found of the i2c bus 
    // scan_i2c(i2c0);

    printf("Starting Core 1...\n");
    enum Core1Mode core1_mode = Idle;
    // enum Core1Mode core1_mode = Remote_Control;
    // enum Core1Mode core1_mode = Maze_Solving;
    change_core1_mode(core1_mode);
    initCore1();

    // Wait for core1 to boot for the PID Controller to start
    while(core1Booting){
        // printf("Waiting on core1 to boot\n");
        sleep_ms(50);
    }

    printf("Initializing Screen System...\n");
    // init Screen Subsystem
    struct Screen* screen = initScreen(I2C0_PORT, 0x3C, PIO_BLOCK_MENU_BUTTONS,
                                       BTN_UP, BTN_DOWN, BTN_SELECT, BTN_BACK);

    printf("Starting Control Loop...\n");
    start_control_loop();

    printf("Starting Watchdog Timer...\n");
    // Enable watchdog for 500ms
    // The second parameter is 'pause_on_debug' (true means it won't trigger while you're debugging)
    watchdog_enable(500, true);
    
    printf("Starting Main Loop\n");
    while (true) {
        if (screen->updated){
            writeMenu(screen->disp, screen->menu);
            screen->updated = false;
        }

        // Print status every loop so we don't flood the terminal
        // printf("Mode: %d | Drive SP: %5.2f | Drive In: %5.2f | Turn SP: %5.2f | Turn In: %5.2f | Wheel Speed: Left - %5.2f %5.2f - Right |  Wheel PWM: Left - %4i %4i - Right\n", 
        //         core1CurrentMode, drive_pid->setpoint, drive_pid->input, turn_pid->setpoint, turn_pid->input, global_drivetrain->motorL->encoder->velocity, global_drivetrain->motorR->encoder->velocity, global_drivetrain->motorL->velocity, global_drivetrain->motorR->velocity);
        printf("Mode: %d | Left SP: %5.2f | Left In: %5.2f | Right SP: %5.2f | Right In: %5.2f | Wheel Speed: Left - %5.2f %5.2f - Right |  Wheel PWM: Left - %4i %4i - Right\n", 
                core1CurrentMode, left_pid->setpoint, left_pid->input, right_pid->setpoint, right_pid->input, global_drivetrain->motorL->encoder->velocity, global_drivetrain->motorR->encoder->velocity, global_drivetrain->motorL->velocity, global_drivetrain->motorR->velocity);
        sleep_ms(100);

        // uint32_t lightL_O = readReflectance(global_reflectiveSystem->Sensors[0]);
        // uint32_t lightL_I = readReflectance(global_reflectiveSystem->Sensors[1]);
        // uint32_t lightR_I = readReflectance(global_reflectiveSystem->Sensors[2]);
        // uint32_t lightR_O = readReflectance(global_reflectiveSystem->Sensors[3]);
        // for (int i = 0; i < NUM_SENSORS; i++){
        //     float normalizedOutput = normalize_sensor(i, global_reflectiveSystem);
        //     // printf("Output: %u, %4.2f\n",raw_val, normalizedOutput);
        // }
        // printf("Light level measured: %u, %u, %u, %u, | %4.2f, %4.2f, %4.2f, %4.2f\n", lightL_O, lightL_I, lightR_I, lightR_O, global_reflectiveSystem->Sensors[0]->normalizedOutput, global_reflectiveSystem->Sensors[1]->normalizedOutput, global_reflectiveSystem->Sensors[2]->normalizedOutput, global_reflectiveSystem->Sensors[3]->normalizedOutput);
        // sleep_ms(100);

        printf("dists: %5d | %5d | %5d\n", global_distanceSystem->Left->results->RangeMilliMeter, global_distanceSystem->Center->results->distance, global_distanceSystem->Right->results->RangeMilliMeter);
        sleep_ms(100);

        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        // sleep_ms(500);
        // cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        // sleep_ms(500);

        // sleep_ms(300);
    }
}
