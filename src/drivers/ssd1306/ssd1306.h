#pragma once

#include <stdint.h>
#include <stdbool.h>
// #ifndef SSD1306_COMMANDS
// #define SSD1306_COMMANDS

// uint8_t DISPLAY_INIT[25];
// void send_cmd(uint8_t commands[], size_t len);
// void send_data(uint8_t buffer[8][128]);
// #endif


//
void send_cmd(uint8_t data[], size_t len);
// 

//
void send_data(uint8_t data[8][128]);
//

//
void display_text(char text[], uint8_t row, uint8_t column);
//

//
void display_init();
//


