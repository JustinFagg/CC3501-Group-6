#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// RX interrupt handler
void on_uart_rx();



//
void blue_init();

//
void blue_send(char massage[]);

//
bool blue_scan();

//
bool blue_steal();
