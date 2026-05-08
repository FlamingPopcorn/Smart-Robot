# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(.pico/pico_sdk_import.cmake)

project(Robot-Car C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(${PROJECT_NAME} 
        Robot-Car.c
        Multithreading.c
        
        # Subsystem Drivers
        Subsystems/Drive_Subsystem.c
        Subsystems/Screen_Subsystem.c
        Subsystems/Reflective_Sensor_Subsystem.c
        Subsystems/Distance_Sensor_Subsystem.c
        Subsystems/PID_Subsystem.c
        Subsystems/Remote_Control_Subsystem.c
        Subsystems/Flash_Subsystem.c
        Subsystems/Music_Subsystem.c

        # Sensor Libraries
        Sensor_Libs/ssd1306.c
        Sensor_Libs/AS5600.c
        # Sensor_Libs/VL6180.c

            Sensor_Libs/VL53L1x/VL53L1X_api.c
            Sensor_Libs/VL53L1x/VL53L1X_calibration.c
            Sensor_Libs/VL53L1x/VL53L1X_platform.c
            
            Sensor_Libs/VL53L0x/vl53l0x_api.c
            Sensor_Libs/VL53L0x/vl53l0x_api_calibration.c
            Sensor_Libs/VL53L0x/vl53l0x_platform.c
            Sensor_Libs/VL53L0x/vl53l0x_api_core.c
            Sensor_Libs/VL53L0x/vl53l0x_api_ranging.c
            Sensor_Libs/VL53L0x/vl53l0x_api_strings.c
            Sensor_Libs/VL53L0x/vl53l0x_platform_log.c

        # Extra Debug stuff
        Debug_Scripts/i2c_scan.c
    )

target_compile_definitions(${PROJECT_NAME} PRIVATE
    SYS_CLK_KHZ=150000
)

pico_set_program_name(${PROJECT_NAME} "Robot-Car")
pico_set_program_version(${PROJECT_NAME} "0.1")

# Generate PIO header
# pico_generate_pio_header(Robot-Car ${CMAKE_CURRENT_LIST_DIR}/blink.pio)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/PIO_Programs/pollReflectance.pio)
pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/PIO_Programs/menuButtons.pio)
# pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/PIO_Programs/tachometerCounter.pio)
# pico_generate_pio_header(${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}/PIO_Programs/magEncoder.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(${PROJECT_NAME} 0)
pico_enable_stdio_usb(${PROJECT_NAME} 1)

# Add the standard library to the build
target_link_libraries(${PROJECT_NAME}
        pico_stdlib
    )

# Add the standard include files to the build
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(${PROJECT_NAME}
        pico_multicore          # Required for Core 1
        pico_cyw43_arch_none    # Required for Led Pin Control bc through WiFi chip
        pico_double             # Fast double-precision math for RP2350
        pico_float              # Fast float-precision math for RP2350
        pico_flash              # Required for thread safe read/write to flash
        hardware_i2c            # Required for I2C Sensos and OLED Screen
        hardware_pwm            # Required for Motors
        hardware_pio            # Required for PIOasm Programs
        hardware_irq            # Required for SIO IRQs
        hardware_watchdog       # Required for the WDT
        hardware_timer          # Required for the Repeating Timer
        hardware_adc            # Required for the ADC
        hardware_flash          # Required for read/write flash
    )

pico_add_extra_outputs(Robot-Car)

