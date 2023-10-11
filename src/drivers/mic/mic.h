#pragma once

#include <stdint.h>
#include <stdbool.h>


#define ADC_NUM 0
#define ADC_PIN (26 + ADC_NUM)



// Function to Initialise Mic
void mic_init();

// Function to Read Mic
void mic_read(uint16_t *data, int size = 1024);

