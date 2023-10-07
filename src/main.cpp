#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" 
#include "drivers/logging/logging.h"
#include "drivers/lis3dh/lis3dh.h"
#include "arm_math.h"



#define pin_SW1             14
#define pin_SW2             15

#define pin_R1              10
#define pin_R2              5
#define pin_R3              6
#define pin_R4              8
#define pin_C1              9
#define pin_C2              11
#define pin_C3              7

#define pin_I2C0_SDA        12
#define pin_I2C0_SCL        13

#define pin_fuel            19

#define pin_Blue_RST        18
#define pin_Blue_CTS        22
#define pin_Blue_RTS        23
#define pin_Blue_Tx         24
#define pin_Blue_Rx         25

#define pin_ADC_Ignition    28


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Define the GPIO connected to keypad rows and columns (ordering important)
const uint rowPins[] = {pin_R1, pin_R2, pin_R3, pin_R4};
const uint colPins[] = {pin_C1, pin_C2, pin_C3};
const int numRows = 4;
const int numCols = 3;

// Define the symbol associated with each key
char keyList[numRows][numCols] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};


// Scan the keypad to detect keypresses
char keypad(const uint* col, const uint* row) {
    for (int i = 0; i < numRows; i++) {
        gpio_put(rowPins[i], 0);
        int result[numCols] = {gpio_get(colPins[0]), gpio_get(colPins[1]), gpio_get(colPins[2])};
        if (std::min(result[0], std::min(result[1], result[2])) == 0) {
            char key = keyList[i][result[0] ? (result[1] ? 2 : 1) : 0];
            gpio_put(rowPins[i], 1);
            return key;
        }
        gpio_put(rowPins[i], 1);
    }
    return '\0';  // No keypress detected
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int main()
{
    //Init
    stdio_init_all();
    // i2c_init(); 

    //general setup
    gpio_set_dir(pin_fuel, 1);


    //Lab setup
    gpio_set_dir(pin_SW1, 0);
    gpio_set_dir(pin_SW2, 0);
    int code = 1;


    for (;;) {
        
        //Button Grid Setup
        for (int i = 0; i < numRows; i++) {
            gpio_init(rowPins[i]);
            gpio_set_dir(rowPins[i], GPIO_OUT);
            gpio_put(rowPins[i], 1);
        }
        for (int i = 0; i < numCols; i++) {
            gpio_init(colPins[i]);
            gpio_set_dir(colPins[i], GPIO_IN);
            gpio_pull_up(colPins[i]);
        }

        if (code == 1){
            //Button grid test
            char key = keypad(colPins, rowPins);
            if (key != '\0') {
                printf("key: %c\n", key);
                sleep_ms(300);
            }
        }


        if (code == 2){
            
        }


        if (code == 3){
            
        }   


        if (code == 4){
            
        }


        if (code == 5){
            
        }


        if (gpio_get(pin_SW1)){
            code++;
            if (code == 6){
                code = 1;
            }
        }
        if (gpio_get(pin_SW2)){
            code--;
            if (code == 0){
                code = 5;
            }
        }
    }
}
