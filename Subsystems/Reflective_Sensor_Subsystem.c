#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "Subsystems/Flash_Subsystem.h"
#include "Subsystems/Reflective_Sensor_Subsystem.h"
#include "pollReflectance.pio.h"

// Mapping weights to sensors: S0=-3, S1=-1, S2=+1, S3=+3
const float SENSOR_WEIGHTS[NUM_SENSORS] = {LEFT_OUTSIDE, LEFT_INSIDE, RIGHT_INSIDE, RIGHT_OUTSIDE};

struct ReflectiveSystem* global_reflectiveSystem;

struct ReflectiveSystem* initReflectiveSystem(PIO pio){
    struct ReflectiveSystem* ReflectiveSystem_ptr = (struct ReflectiveSystem*) malloc(sizeof(struct ReflectiveSystem));
    if (ReflectiveSystem_ptr == NULL) {
        printf("CRITICAL: Reflective System Malloc Failed!\n");
        return NULL;
    }
    struct CalibrationData* CalibrationData_ptr = (struct CalibrationData*) malloc(sizeof(struct CalibrationData));
    if (CalibrationData_ptr == NULL) {
        printf("CRITICAL: Reflective System Calibration Data Malloc Failed!\n");
        return NULL;
    }
    ReflectiveSystem_ptr->calibrationLoaded = false;
    ReflectiveSystem_ptr->calibration = CalibrationData_ptr;
    ReflectiveSystem_ptr->pio = pio;
    ReflectiveSystem_ptr->lineErrorOutput = 0;

    for (int i = 0; i < 4; i++) {pio_sm_set_enabled(pio, i, false);};
    pio_clear_instruction_memory(pio);

    ReflectiveSystem_ptr->offset = pio_add_program(pio, &pollReflectance_program);

    for (int i = 0; i < NUM_SENSORS; i++){
        ReflectiveSystem_ptr->Sensors[i] = initReflectiveSensor(REFLECTIVE_SENSORS[i], pio, ReflectiveSystem_ptr->offset);
    }

    uint32_t whiteArry[4] = {5900, 8500, 8400, 12000};
    uint32_t blackArry[4] = {pollReflectance_TIMEOUT_LOOPS+1, pollReflectance_TIMEOUT_LOOPS+1, pollReflectance_TIMEOUT_LOOPS+1, pollReflectance_TIMEOUT_LOOPS+1};

    ReflectiveSystem_ptr->calibrationLoaded = true;
    

    for (int i = 0; i < 4; i++){
        ReflectiveSystem_ptr->calibration->white[i] = whiteArry[i];
        ReflectiveSystem_ptr->calibration->black[i] = blackArry[i];
    }
    
    
    global_reflectiveSystem = ReflectiveSystem_ptr;

    return ReflectiveSystem_ptr;
}

static struct ReflectiveSensor* initReflectiveSensor(int pin, PIO pio, uint offset){
    struct ReflectiveSensor* sensor_ptr = (struct ReflectiveSensor*) malloc(sizeof(struct ReflectiveSensor)); if (!sensor_ptr) return NULL;
    if (sensor_ptr == NULL) {
        printf("CRITICAL: Reflectance Sensor Malloc Failed!\n");
        return NULL;
    }

    sensor_ptr->pin = pin;
    sensor_ptr->pio = pio;
    sensor_ptr->sm = pio_claim_unused_sm(pio, true); // Find a free state machine

    pollReflectance_program_init(sensor_ptr->pio, sensor_ptr->sm, offset, sensor_ptr->pin); // Start the State Machine

    // Send a timeout value for the loop
    pio_sm_put_blocking(sensor_ptr->pio, sensor_ptr->sm, pollReflectance_TIMEOUT_LOOPS);

    sensor_ptr->output = 0;
    sensor_ptr->normalizedOutput = 0.0f;

    return sensor_ptr;
}

uint32_t readReflectance(struct ReflectiveSensor* sensor){
    // Do not wait! Just return the last completed measurement.
    if (pio_sm_is_rx_fifo_empty(sensor->pio, sensor->sm)) {
        return sensor->output;
    }

    uint32_t raw_val = 0;
    while (!pio_sm_is_rx_fifo_empty(sensor->pio, sensor->sm)) {
        raw_val = pio_sm_get(sensor->pio, sensor->sm);
    }

    if (raw_val == 0 || raw_val == 0xFFFFFFFF) {            // when pio_sm_x gets to 0 and runs jmp x--, it fails to jump but sit decrements x
        sensor->output = pollReflectance_TIMEOUT_LOOPS;     // rolling it over to 0xFFFFFFFF or -1 as signed int
    } else {
        uint32_t elapsed_loops = pollReflectance_TIMEOUT_LOOPS - raw_val;
        
        sensor->output = elapsed_loops;
    }

    // elapsed_loops is the number of 2-cycle loops. 
    // At 150MHz, total time in microseconds = (raw_val * 2) / 150
    return  sensor->output;
}

float normalize_sensor(int index, struct ReflectiveSystem* reflectiveSystem) {
    if (index < NUM_SENSORS){
        uint32_t white_ref;
        uint32_t black_ref;
        
        if (reflectiveSystem->calibrationLoaded){
            white_ref = reflectiveSystem->calibration->white[index];
            black_ref = reflectiveSystem->calibration->black[index];
        } else {
            white_ref = 7000;
            black_ref = 750000;
        }

        uint32_t raw = reflectiveSystem->Sensors[index]->output;
        if (raw <= white_ref) return 0.0f;
        if (raw >= black_ref) return 1.0f;

        reflectiveSystem->Sensors[index]->normalizedOutput = (float)(raw - white_ref) / (float)(black_ref - white_ref);
        return reflectiveSystem->Sensors[index]->normalizedOutput;
    }
    return 0;
}

float calculate_line_error(struct ReflectiveSystem* reflectiveSystem) {
    float weighted_sum = 0.0;
    uint8_t active_sensors = 0;

    for (int i = 0; i < NUM_SENSORS; i++) {
    float val = reflectiveSystem->Sensors[i]->normalizedOutput;
        if (val > 0.05 || true){ // Some Threshold for actually being bright but not overly bright
            weighted_sum += val * SENSOR_WEIGHTS[i];
            active_sensors++;
            // printf("Output: %4.2f, %4.2f, %4.2f\n",val, weighted_sum, SENSOR_WEIGHTS[i]);
        }
    }

    // Handle "No Line Detected"
    if (active_sensors == 0){
        // Strategy: Return a large error based on last direction or 0
        // return 0.0f;
    } else {
        const float alpha = 0.3f;
        reflectiveSystem->lineErrorOutput = (alpha * weighted_sum) + ((1.0f - alpha) * reflectiveSystem->lineErrorOutput);
    }

    // Return the average position of the line relative to center
    return reflectiveSystem->lineErrorOutput;
}