#ifndef _Screen_Subsystem
#define _Screen_Subsystem

#include "Sensor_Libs/ssd1306.h"

// Screen Dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// PIO Instance
#define PIO_BLOCK_MENU_BUTTONS pio2

// Screen Button defines
#define BTN_UP 9
#define BTN_DOWN 8
#define BTN_SELECT 7   // '#' Button
#define BTN_BACK 6     // '*' Button
#define MENU_BUTTONS ((const int[]){BTN_UP, BTN_DOWN, BTN_SELECT, BTN_BACK})

#define MAX_PAGES 4

extern struct Menu *global_menu_ptr;

struct Screen{
    i2c_inst_t *i2cBus;
    int addr;
    ssd1306_t* disp;
    PIO pio;
    uint offset;
    int btn_up;
    int btn_down;
    int btn_select;
    int btn_back;
    struct Menu* menu;
    uint32_t sm[4];
    bool updated;
};

struct Screen* initScreen(i2c_inst_t *i2cBus, int addr, PIO pio, int btn_up, int btn_down, int btn_select, int btn_back);

// // IRQ Callbacks
// void btn_up_callback(void);
// void btn_down_callback(void);
// void btn_select_callback(void);
// void btn_back_callback(void);
void pio_irq_handler_menuButtons();

// Menu Section //
struct Menu{
    volatile int currentPageID;
    volatile int cursorOption;
};

struct Menu* initMenu(void);
void writeMenu(ssd1306_t *, struct Menu*);

#endif