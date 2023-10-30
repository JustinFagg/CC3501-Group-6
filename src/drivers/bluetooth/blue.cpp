#include "blue.h"
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <algorithm>
#include "hardware/uart.h"
#include "hardware/irq.h"

#define pin_Blue_RST        18
#define pin_Blue_CTS        22
#define pin_Blue_RTS        23
#define pin_Blue_Tx         24
#define pin_Blue_Rx         25

#define uart_num uart1
#define baud_rate 115200 // Baud rate (adjust as needed)

char bt_buffer[1000] = {0};
uint bt_buffer_idx = 0;

char mac[] = {'5','4','F','8','5','7','A','B','E','5','F','8'};


// RX interrupt handler
void on_uart_rx() {
    while (uart_is_readable(uart_num)) {
        bt_buffer[bt_buffer_idx] = uart_getc(uart_num);
        bt_buffer_idx++;
        if (bt_buffer_idx > 1000-1) {
          bt_buffer_idx = 0;
        }
    }
}

void blue_init(){
    // Initialize UART
    uart_init(uart_num, baud_rate);
    
    // Set UART pins
    gpio_set_function(pin_Blue_Tx, GPIO_FUNC_UART);
    gpio_set_function(pin_Blue_Rx, GPIO_FUNC_UART);
    uart_set_fifo_enabled(uart_num, true);

    // set up interrupt handlers
    irq_set_exclusive_handler(UART1_IRQ, on_uart_rx);
    irq_set_enabled(UART1_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(uart_num, true, false);
}

void blue_send(char massage[]){
    uart_puts(uart_num, massage);
    sleep_ms(100);
}



bool blue_scan(){
    bool wrong = 0;
    for (int j = 0;(bt_buffer[j]!='\000')&&(j<(1000-12));j = j + 1){
        for(int o = 0;o<12;o++){
            if (bt_buffer[j + o] != mac[o]){
                wrong = 1;
            }
        }
        if (!wrong){
            return 1;
        }
        else{
            wrong = 0;
        }
    }
    return 0;
}


//
bool blue_steal(){
if ((bt_buffer[52] != 'n')&&(bt_buffer[53] != 'o')){
    for (int i = 0; i<12; i++){
        mac[i]=bt_buffer[52 + i];
    }
    return 1;
}
return 0;
}

