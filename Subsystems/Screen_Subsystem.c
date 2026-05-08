#include <stdio.h>
#include <string.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/irq.h"

#include "Multithreading.h"

#include "Sensor_Libs/ssd1306.h"

#include "Subsystems/Flash_Subsystem.h"

#include "Subsystems/Screen_Subsystem.h"

#include "menuButtons.pio.h"

// Static global pointer so the irq handler can find modify menu data
static struct Screen *global_screen_ptr = NULL;

struct Menu *global_menu_ptr = NULL;

struct Screen* initScreen(i2c_inst_t *i2cBus, int addr, PIO pio, int btn_up, int btn_down, int btn_select, int btn_back){
    struct Screen* screen_ptr = (struct Screen*) malloc(sizeof(struct Screen));
    ssd1306_t* disp_ptr = (ssd1306_t*) malloc(sizeof(ssd1306_t));
    if (screen_ptr == NULL) {
        printf("CRITICAL: Screen System Malloc Failed!\n");
        return NULL;
    } else if (disp_ptr == NULL) {
        printf("CRITICAL: Display Malloc Failed!\n");
        return NULL;
    }

    screen_ptr->i2cBus = i2cBus;
    screen_ptr->addr = addr;
    screen_ptr->disp = disp_ptr;
    screen_ptr->pio = pio;
    screen_ptr->btn_up = btn_up;
    screen_ptr->btn_down = btn_down;
    screen_ptr->btn_select = btn_select;
    screen_ptr->btn_back = btn_back;

    // Initialize Display
    ssd1306_init(disp_ptr, SCREEN_WIDTH, SCREEN_HEIGHT, addr, i2cBus);
    ssd1306_contrast(disp_ptr, 0xFF);

    ssd1306_clear(screen_ptr->disp);
    ssd1306_draw_string(screen_ptr->disp, (SCREEN_WIDTH - strlen("Richard Renner") * 5) / 2, SCREEN_HEIGHT * (1 / 5.0), 1, "Richard Renner");
    ssd1306_draw_string(screen_ptr->disp, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2.0 - 8, 1, "&");
    ssd1306_draw_string(screen_ptr->disp, (SCREEN_WIDTH - strlen("Andrea Mrkaja") * 5) / 2, SCREEN_HEIGHT * (4 / 5.0) - 8, 1, "Andrea Mrkaja");
    ssd1306_show(screen_ptr->disp);

    // Initialize Menu
    struct Menu* menu = initMenu();
    screen_ptr->menu = menu;

    for (int i = 0; i < 4; i++) {pio_sm_set_enabled(pio, i, false);};
    pio_clear_instruction_memory(pio);

    screen_ptr->offset = pio_add_program(pio, &menuButtons_program);

    // Set the function that will handle the IRQ
    // PIO0_IRQ_0 is the system-level interrupt for PIO block 0
    irq_set_exclusive_handler(PIO2_IRQ_0, pio_irq_handler_menuButtons);

    // Enable the system-level interrupt
    irq_set_enabled(PIO2_IRQ_0, true);

    for (int i = 0; i < 4; i++){
        screen_ptr->sm[i] = pio_claim_unused_sm(pio, true); // Find a free state machine
        menuButtons_program_init(pio, screen_ptr->sm[i], screen_ptr->offset, MENU_BUTTONS[i]); // Start the State Machine

        // Tell the PIO to route SM interrupts to the system-level IRQ
        // This maps your PIO 'irq 0 rel' to the system interrupt
        pio_set_irq0_source_enabled(pio, (enum pio_interrupt_source)(pis_interrupt0 + screen_ptr->sm[i]), true);
    }

    global_screen_ptr = screen_ptr;

    return screen_ptr;
}

struct Menu* initMenu(void){
    struct Menu* menu_ptr = (struct Menu*) malloc(sizeof(struct Menu));
    if (menu_ptr == NULL) {
        printf("CRITICAL: Menu Malloc Failed!\n");
        return NULL;
    }

    menu_ptr->currentPageID = 0;
    menu_ptr->cursorOption = 0;

   global_menu_ptr = menu_ptr;

    return menu_ptr;
}

void pio_irq_handler_menuButtons(){
    if (global_screen_ptr == NULL){return;}

    // Read the current IRQ status for PIO2
    uint32_t irq_status = pio2_hw->irq;

    for (int i = 0; i < 4; i++) {
        uint sm = global_screen_ptr->sm[i];
        if (irq_status & (1 << sm)) {
            
            switch(MENU_BUTTONS[i]){
                case BTN_UP:
                    if (global_screen_ptr->menu->cursorOption > 0 && global_screen_ptr->menu->currentPageID < 10)
                        global_screen_ptr->menu->cursorOption--;
                    pio_interrupt_clear(PIO_BLOCK_MENU_BUTTONS, sm); 
                    break;
                case BTN_DOWN:
                    if (global_screen_ptr->menu->cursorOption < MAX_PAGES - 1 && global_screen_ptr->menu->currentPageID < 10)
                        global_screen_ptr->menu->cursorOption++;
                    pio_interrupt_clear(PIO_BLOCK_MENU_BUTTONS, sm); 
                    break;
                case BTN_SELECT:
                    if (global_screen_ptr->menu->currentPageID == 0){
                        global_screen_ptr->menu->currentPageID = global_screen_ptr->menu->cursorOption + 1;
                        global_screen_ptr->menu->cursorOption = 0;
                        pio_interrupt_clear(PIO_BLOCK_MENU_BUTTONS, sm); 
                    } else if (global_screen_ptr->menu->currentPageID < 4 && global_screen_ptr->menu->cursorOption == 0){ // Launch screen for specific mode
                        global_screen_ptr->menu->currentPageID += 10;
                        enum Core1Mode core1_mode = global_screen_ptr->menu->currentPageID - 10;
                        // enum Core1Mode core1_mode = 3;
                        change_core1_mode(core1_mode);
                        global_screen_ptr->menu->cursorOption = -1;
                    } else if (global_screen_ptr->menu->currentPageID < 4 && global_screen_ptr->menu->cursorOption == 1){ // Go back
                        global_screen_ptr->menu->currentPageID = 0;
                        global_screen_ptr->menu->cursorOption = 0;
                    } else if (global_screen_ptr->menu->currentPageID == 4 && global_screen_ptr->menu->cursorOption == 0){ // Load Calibration
                        global_screen_ptr->menu->currentPageID = 21;
                        global_screen_ptr->menu->cursorOption = 0;
                        bool loaded = loadCalibration(global_reflectiveSystem->calibration);
                        global_reflectiveSystem->calibrationLoaded = loaded;
                        global_screen_ptr->menu->currentPageID = loaded ? 22 : 23;
                    } else if (global_screen_ptr->menu->currentPageID == 4 && global_screen_ptr->menu->cursorOption == 1){ // Run Calibration
                        global_screen_ptr->menu->currentPageID = 25;
                        global_screen_ptr->menu->cursorOption = 0;
                        runCalibration();
                    } else if (global_screen_ptr->menu->currentPageID == 4 && global_screen_ptr->menu->cursorOption == 2){ // View Debug Values

                    }
                    break;
                case BTN_BACK:
                    if (global_screen_ptr->menu->currentPageID < 10){
                        global_screen_ptr->menu->currentPageID = 0;
                        global_screen_ptr->menu->cursorOption = 0;
                    } else if (global_screen_ptr->menu->currentPageID < 20){ // Exit screen for specific mode
                        enum Core1Mode core1_mode = Idle;
                        change_core1_mode(core1_mode);
                        global_screen_ptr->menu->currentPageID -= 10;
                        global_screen_ptr->menu->cursorOption = 0;
                    } else if (global_screen_ptr->menu->currentPageID == 22 || global_screen_ptr->menu->currentPageID == 23 ||
                                global_screen_ptr->menu->currentPageID == 28 || global_screen_ptr->menu->currentPageID == 29){
                        global_screen_ptr->menu->currentPageID = 4;
                        global_screen_ptr->menu->cursorOption = 0;
                    }
                    break;
            }
            // global_screen_ptr->current_selection = i;
            global_screen_ptr->updated = true;

            // Clear the flag and unblock both the CPU and the PIO SM
            pio_interrupt_clear(PIO_BLOCK_MENU_BUTTONS, sm); 
        }
    }


}

void writeMenu(ssd1306_t *disp, struct Menu* menu) {
    ssd1306_clear(disp);
    
    if (menu->currentPageID < 10){
        // Draw Cursor
        ssd1306_draw_string(disp, 0, 15 + 10 * menu->cursorOption, 1, "->");
    }
    
    switch (menu->currentPageID) {
        // Main menu page
        case 0:
            ssd1306_draw_string(disp, 36, 0, 1, "Main Menu");
            ssd1306_draw_string(disp, 12, 15, 1,"Line Following");
            ssd1306_draw_string(disp, 12, 25, 1,"Maze Solving");
            ssd1306_draw_string(disp, 12, 35, 1,"Remote Control");
            ssd1306_draw_string(disp, 12, 45, 1,"Debug");
            break;
        
        // Line Following page
        case 1:
            ssd1306_draw_string(disp, 8, 0, 1, "Line Following Menu");
            ssd1306_draw_string(disp, 12, 15, 1,"Go");
            break;
        // Maze Solving page
        case 2:
            ssd1306_draw_string(disp, 8, 0, 1, "Maze Solving Menu");
            ssd1306_draw_string(disp, 12, 15, 1,"Go");
            break;
        // Radio Control page
        case 3:
            ssd1306_draw_string(disp, 8, 0, 1, "Radio Control Menu");
            ssd1306_draw_string(disp, 12, 15, 1,"Go");
            break;
        // Debug page
        case 4:
            ssd1306_draw_string(disp, 8, 0, 1, "Debug Menu");
            ssd1306_draw_string(disp, 12, 15, 1,"Load Calibration");
            ssd1306_draw_string(disp, 12, 25, 1,"Run Calibration");
            break;
        // Go Line Following
        case 11:
            ssd1306_draw_string(disp, 8, 0, 1, "Line Following :)");
            break;
        // Go Maze Solving
        case 12:
            ssd1306_draw_string(disp, 8, 0, 1, "Maze Solving :)");
            break;
        // Go Remote Control
        case 13:
            ssd1306_draw_string(disp, 8, 0, 1, "Remote Control :)");
            break;

        // Calibration Stuff
            // Go Load Calibration
            case 21:
                ssd1306_draw_string(disp, 8, 25, 1, "Loading Calibration...");
                break;
            // Successful Load
            case 22:
                ssd1306_draw_string(disp, 8, 15, 1, "Calibration Load");
                ssd1306_draw_string(disp, 13, 25, 1, "Succeded");
                break;
            // Failed Load
            case 23:
                ssd1306_draw_string(disp, 8, 15, 1, "Calibration Load");
                ssd1306_draw_string(disp, 13, 25, 1, "Failed");
                break;
            
            // Go Run Calibration
            case 25:
                ssd1306_draw_string(disp, 8, 25, 1, "Running Calibration...");
                break;
            // Calibrate White
            case 26:
                ssd1306_draw_string(disp, 8, 15, 1, "Place Sensors on");
                ssd1306_draw_string(disp, 13, 25, 1, "White");
                break;
            // Calibrate Black
            case 27:
                ssd1306_draw_string(disp, 8, 15, 1, "Place Sensors on");
                ssd1306_draw_string(disp, 13, 25, 1, "Black");
                break;
            // Successful Save
            case 28:
                ssd1306_draw_string(disp, 8, 15, 1, "Calibration Save");
                ssd1306_draw_string(disp, 13, 25, 1, "Succeded");
                break;
            // Failed Load
            case 29:
                ssd1306_draw_string(disp, 8, 15, 1, "Calibration Save");
                ssd1306_draw_string(disp, 13, 25, 1, "Failed");
                break;
    }

    // char buffer[20];
    // snprintf(buffer, sizeof(buffer), "%d, %d", menu->cursorOption, menu->currentPageID);
    // ssd1306_draw_string(disp, 12, 55, 1, buffer);

    ssd1306_show(disp);
}

