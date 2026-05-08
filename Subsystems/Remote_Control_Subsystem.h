#ifndef _Remote_Control_Subsystem
#define _Remote_Control_Subsystem

#include "pico/stdlib.h"
#include "hardware/uart.h"

#define UART_IBUS uart0
#define UART_IBUS_TX 0  // There is no sending data to the radio supported
#define UART_IBUS_RX 1   
#define UART_IBUS_BAUDRATE 115200 // 8N1
#define IBUS_FRAME_LENGTH 32
#define IBUS_NUM_CHANNELS 6

#define CHANNEL_RSTICK_X 1
#define CHANNEL_RSTICK_Y 2
#define CHANNEL_LSTICK_Y 3
#define CHANNEL_LSTICK_X 4

#define CHANNEL_AUX_VRA 5
#define CHANNEL_AUX_VRB 6

#define VELOCITY_DEADZONE 50

extern struct IBus* global_ibus;

enum ChannelType{
    MOTOR,
    SERVO,
    SWITCH
};

struct Channel{
    uint8_t channelNumber;
    enum ChannelType channelType;
    uint16_t value;
};

struct IBus{
    uart_inst_t *uart;
    int RX_pin;
    struct Channel *channels[IBUS_NUM_CHANNELS];
};

struct IBus* initIBus(uart_inst_t *uart, int pin);
static struct Channel* initChannel(uint8_t channelNumber);

void readIBus(struct IBus* ibus);

#endif