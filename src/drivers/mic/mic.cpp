#include "pico/stdlib.h"
#include "mic.h"
#include "hardware/adc.h"




void mic_init(){
    adc_init();
        adc_gpio_init( ADC_PIN);
        adc_select_input( ADC_NUM);
        adc_set_clkdiv(1087);
            adc_fifo_setup(
            true,    // Write each completed conversion to the sample FIFO
            false,    // Enable DMA data request (DREQ)
            1,       // DREQ (and IRQ) asserted when at least 1 sample present
            false,    // Set sample error bit on error
            false   // Keep full 12 bits of each sample
        );
}

void mic_read(uint16_t *data, int size){
    adc_run(1);
    for(int i = 0; i < size; i++){
        data[i] = adc_fifo_get_blocking();
    }
    adc_run(0);  
    adc_fifo_drain(); 
}
