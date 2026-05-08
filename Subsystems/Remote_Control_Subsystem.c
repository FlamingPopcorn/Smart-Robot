#include <stdio.h>
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "Remote_Control_Subsystem.h"

struct IBus* global_ibus;

struct IBus* initIBus(uart_inst_t *uart, int RX_pin){
    struct IBus* ibus_ptr = (struct IBus*) malloc(sizeof(struct IBus));
    if (ibus_ptr == NULL) {
        printf("CRITICAL: IBus Malloc Failed!\n");
        return NULL;
    }

    ibus_ptr->uart = uart;
    ibus_ptr->RX_pin = RX_pin;

    uint baudrate_negotiated = uart_init(uart, UART_IBUS_BAUDRATE);
    // gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_IBUS_RX, GPIO_FUNC_UART);

    // if (baudrate_negotiated != UART_IBUS_BAUDRATE){printf("ERROR: UART Baudrate: %u\n", baudrate_negotiated);}
    // else printf("UART Baudrate: %u\n", baudrate_negotiated);

    for (uint8_t i = 0; i < IBUS_NUM_CHANNELS; i++){
        ibus_ptr->channels[i] = initChannel(i);
    }

    global_ibus = ibus_ptr;

    return ibus_ptr;
}

static struct Channel* initChannel(uint8_t channelNumber){
    struct Channel* channel_ptr = (struct Channel*) malloc(sizeof(struct Channel));
    if (channel_ptr == NULL) {
        printf("CRITICAL: IBus Channel Malloc Failed!\n");
        return NULL;
    }

    channel_ptr->channelNumber = channelNumber; // 0..5 Base so we can add 1 to match the remote setup 1..6
    channel_ptr->value = -1;

    switch(channelNumber + 1){ // Set Channel types
        case CHANNEL_RSTICK_X:
            channel_ptr->channelType = MOTOR;
            break;
        case CHANNEL_RSTICK_Y:
            channel_ptr->channelType = MOTOR;
            break;
        case CHANNEL_LSTICK_Y:
            channel_ptr->channelType = MOTOR;
            break;
        case CHANNEL_LSTICK_X:
            channel_ptr->channelType = MOTOR;
            break;
        case CHANNEL_AUX_VRA:
            channel_ptr->channelType = SERVO;
            break;
        case CHANNEL_AUX_VRB:
            channel_ptr->channelType = SERVO;
            break;
    }

    switch(channel_ptr->channelType){
        case MOTOR:
            channel_ptr->value = 1500; // 1500 is neutral
            break;
        case SERVO:
            channel_ptr->value = 1500; // 1500 is neutral
            break;
        case SWITCH:
            channel_ptr->value = 1000; // 1000 is off
            break;
        
    }

    return channel_ptr;
}

void readIBus(struct IBus* ibus){
    uint8_t byte = 0;
    uint8_t prevByte = 0;

    while (byte != 0x40 && prevByte != 0x20){ // Look for IBus header 0x20 0x40
        if (uart_is_readable(ibus->uart)){
            prevByte = byte;
            byte = uart_getc(ibus->uart);
        }
    }
    
    uint8_t buffer[IBUS_FRAME_LENGTH - 2] = {0};
    uart_read_blocking(ibus->uart, buffer, IBUS_FRAME_LENGTH - 2);

    uint16_t calc_checksum = 0xFFFF - 0x20 - 0x40;
    for (int i = 0; i < 28; i++) {
        calc_checksum -= buffer[i];
    }

    // The received checksum is in the last 2 bytes of our 30-byte buffer
    uint16_t rx_checksum = buffer[28] | (buffer[29] << 8);

    if (rx_checksum == calc_checksum){
        for (int i = 0; i < IBUS_NUM_CHANNELS; i++){
            // ibus->channels[i]->value = 0;
            uint16_t value = (buffer[2*i+1] << 8) | buffer[2*i];

            if (value > 2000){
                value = 2000;
            } else if (value < 1000){
                value = 1000;
            }

            ibus->channels[i]->value = value;
        }
    }
    return;
}

