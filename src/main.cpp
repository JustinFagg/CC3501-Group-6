#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/led/led.h"
#include "drivers/lis3dh/lis3dh.h"
#include "drivers/mic/mic.h"
#include "arm_math.h"



#define SW1 15




int main()
{
    //Init
    stdio_init_all();
    led_init();
    i2c_init();
    int16_t my_result[3] = {};
    mic_init(); 
    #define mic_size 1024
    uint16_t adc_test[mic_size] = {};  


    #define uart_num uart1
    #define tx_pin 8     // GPIO8 (TX)
    #define rx_pin 9     // GPIO9 (RX)
    #define baud_rate 115200 // Baud rate (adjust as needed)
    
    // Initialize UART
    uart_init(uart_num, baud_rate);
    
    // Set UART pins
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);





    arm_rfft_init_q15;
    int32_t DCbias = 0;

    //Lab setup
    gpio_set_dir(SW1, 0);
    int Lab = 5;
   
    //Clear LED
    led_all();
    led_load();


    /////////////////////////////////////////////////////Rainbow
    int red = 255, green = 0, blue = 0;
    int test = 0;
    /////////////////////////////////////////////////////////

    for (;;) {

        if (Lab == 5){
        // Write data to Bluetooth module
        uart_putc(uart_num, 'H'); // Send 'H' (example)
        uart_putc(uart_num, 'i'); // Send 'i' (example)

        // Delay for a moment before sending more data
        sleep_ms(1000); // Delay for 1 second (adjust as needed)
        }


        if (Lab == 6){
            if ((test < 255)&&(test >= 0))
            {
                red--;
                blue++;
            }
            if ((test < 510)&&(test >= 255))
            {
                blue--;
                green++;
            }
            if ((test < 765)&&(test >= 510))
            {
                green--;
                red++;
            }
            test ++;
            if (test >= 766){
                test=0;
            }
            if ((test%50)==0){
                led_spin();
                led_set(0,red/3, green/3, blue/3);
                led_load();
                sleep_ms(10);
            }
        }


        if (Lab == 7){
            led_all();
            led_load();

            sleep_ms(1000);

            led_set(4,128,0,128);
            led_load();

            sleep_ms(1000);

            led_set(7,64,224,208);
            led_load();

            sleep_ms(1000);

            led_set(5,127,10,235);
            led_set(6,235,10,127);
            led_load();

            sleep_ms(1000);
        }   


        if (Lab == 8){
            // Read the "0x28" register
            i2c_reads(n28, my_result);
                led_all();
                if (my_result[0]<130&&my_result[0]>-130&&my_result[1]<130&&my_result[1]>-130&&my_result[2]<130&&my_result[2]>-130){   
                    led_set(int(my_result[0]/62+2),75,0,0);
                    led_set(int(my_result[1]/62+6),0,75,0);
                    led_set(int(my_result[2]/62+10),0,0,75); 
                }
                snprintf(buf, count,"X: %d\nY: %d\nZ: %d", my_result[0], my_result[1], my_result[2])
                uart_puts(uart_num, "test");
            led_load();
        }


        if (Lab == 9){
            mic_read(adc_test,mic_size);
        }


        if (Lab == 10){

            

        }

        if (gpio_get(SW1)){
            Lab++;
            if (Lab == 11){
                Lab = 4;
            }
            led_all(1);
            led_load();
            sleep_ms(3000);
            led_all(0);
            led_load();
        }
    }
}
