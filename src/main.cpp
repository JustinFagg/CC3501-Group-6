#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" 
#include "drivers/logging/logging.h"
#include "drivers/lis3dh/lis3dh.h"

#include "hardware/i2c.h"
#include "drivers/ssd1306/ssd1306.h"
#include "arm_math.h"

#include "drivers/keypad/keypad.h"
#include "hardware/adc.h"

#include "hardware/uart.h"
#include "hardware/irq.h"
#include "drivers/bluetooth/blue.h"


#define pin_SW1             14
#define pin_SW2             15

//for the other pinpad
// #define pin_R1              10
// #define pin_R2              5
// #define pin_R3              6
// #define pin_R4              8
// #define pin_C1              9
// #define pin_C2              11
// #define pin_C3              7

//for current pinpad
#define pin_R1              11
#define pin_R2              10
#define pin_R3              9
#define pin_R4              8
#define pin_C1              7
#define pin_C2              6
#define pin_C3              5

#define pin_I2C0_SDA        12
#define pin_I2C0_SCL        13

#define pin_fuel            19

#define pin_ADC_Ignition    28

// Define the GPIO connected to keypad rows and columns (ordering important)
const uint rowPins[] = {pin_R1, pin_R2, pin_R3, pin_R4};
const uint colPins[] = {pin_C1, pin_C2, pin_C3};

// Define the I2C address of the SSD1306 OLED display
#define OLED_I2C_ADDRESS 0x3C

uint8_t turnoff[1]={0xAE};
uint8_t turnon[1]={0xAF};
uint8_t DISPLAY_INITT[27] = {
    0xAE,  // Display Off
    0x20, 0x00,  // Set Memory Addressing Mode to Horizontal addressing mode
    0x40,  // Set display start line to 0
    0xA1,  // Set segment remap
    0xA8, 0x3F,  // Set multiplex ratio (1/64 duty)
    0xC8,  // Set COM Output Scan Direction
    0xD3, 0x00,  // Set display offset
    0xDA, 0x12,  // Set COM Pins hardware configuration
    0xD5, 0x80,  // Set display clock divide ratio/oscillator frequency
    0xD9, 0xF1,  // Set pre-charge period
    0xDB, 0x30,  // Set VCOMH Deselect Level
    0x81, 0xFF,  // Set contrast control
    0xA4,  // Entire Display On, Output follows RAM content
    0xA6,  // Normal display (not inverted)
    0x8D, 0x14,  // Enable charge pump regulator
    0xAF,  // Display On
    0x2E,   // Deactivate scrolling

    0xC0    // Invert Something
};



int main()


{
    stdio_init_all();

    gpio_set_dir(pin_SW1, 0);
    gpio_set_dir(pin_SW2, 0);
    int code = 1;
    int start = 0;

    blue_init();

    // Setup keyboard
    keypad_init(colPins, rowPins);
    display_init();
    send_cmd(DISPLAY_INITT, sizeof(DISPLAY_INITT) / sizeof(uint8_t));
 


    /////////////////////Extra
    int secretCode[4] = {6,9,3,5};
    int MysecretCode[4] = {6,9,3,5};
    int myNumber[4];
    char myConstChar[9] = "        ";
    char myBlankConstChar[9] = "        ";
    //////////////////////////
    display_text(myBlankConstChar, 1, 1);
    int saved = 5;

    gpio_init(pin_fuel);
    gpio_set_dir(pin_fuel,1);
    gpio_put(pin_fuel,0);

    // Set up the ADC on the Pico
    adc_init();
    adc_gpio_init(pin_ADC_Ignition); // Set the GPIO pin you want to read from (e.g., GPIO 26)
    adc_select_input(2); // Select ADC channel 0

    blue_init();
    
    for (;;) {
    

        if (code != 0){
            //Button grid test
            char key = keypad();
            if (key != '\0') {
                if ((key == '*')&&(code!=1)){
                    code--;
                    myBlankConstChar[2*(code-1)+1] = ' ';
                    display_text(myBlankConstChar, 1, 1);
                    sleep_ms(50);
                    while(keypad() != '\0'){}
                }
                else if (key == '#'){
                    sleep_ms(50);
                    while(keypad() != '\0'){}
                    code = 1;
                        for (int i=0;i<4;i++){
                            myConstChar[2*i]=' ';
                            myConstChar[2*i+1]=' ';
                            myBlankConstChar[2*i]=' ';                            
                            myBlankConstChar[2*i+1]=' ';
                        }
                        display_text("Looking", 1, 1);

                        blue_send("---\r\n");
                        blue_send("$$$");
                        blue_send("d\r\n");
                        blue_send("---\r\n");
                        blue_send("---");

                        if (blue_scan()){
                            code = 0;
                            start = 1;
                            char myString[] = "Start!";
                            display_text(myString, 1, 1);
                            sleep_ms(1000);
                        }
                        else{                        
                            display_text("UnFound", 1, 1);
                        }
                }
                else{
                    if (code!=0){
                        if (code == 1){send_cmd(turnon,sizeof(turnon) / sizeof(uint8_t));}
                        if (code<5){
                            myNumber[code-1] = key-48;
                            myBlankConstChar[2*(code-1)+1] = '*';
                            myConstChar[2*(code-1)+1] = key;
                            code+=1;
                            display_text(myBlankConstChar, 1, 1);
                        }
                        if (code==5){
                            if (MysecretCode[3] == -1){MysecretCode[3] = saved;}
                            if ((myNumber[0]==MysecretCode[0])*(myNumber[1]==MysecretCode[1])*(myNumber[2]==MysecretCode[2])*(myNumber[3]==MysecretCode[3])){
                                code = 0;
                                start = 1;
                                char myString[] = "Start!";
                                display_text(myString, 1, 1);
                                sleep_ms(1000);
                            }
                            else{
                                display_text(myConstChar, 1, 1);
                                code = 1;
                                for (int i=0;i<3;i++){
                                    sleep_ms(500);
                                    send_cmd(turnoff,sizeof(turnoff) / sizeof(uint8_t));
                                    sleep_ms(400);
                                    send_cmd(turnon,sizeof(turnon) / sizeof(uint8_t));
                                }
                                for (int i=0;i<4;i++){
                                    myNumber[i]=0;
                                    myConstChar[2*i]=' ';
                                    myConstChar[2*i+1]=' ';
                                    myBlankConstChar[2*i]=' ';                            
                                    myBlankConstChar[2*i+1]=' ';
                                }
                                display_text(myBlankConstChar, 1, 1);
                                send_cmd(turnoff,sizeof(turnoff) / sizeof(uint8_t));
                            }
                        }
                        sleep_ms(50);
                        while(keypad() != '\0'){}
                    }
                }
            }
            
            if ((gpio_get(pin_SW1))||(gpio_get(pin_SW2))){
                saved = MysecretCode[3];
                for (int i=0;i<4;i++){
                    myConstChar[2*i]=' ';
                    myConstChar[2*i+1]=' ';
                    myBlankConstChar[2*i]=' ';                            
                    myBlankConstChar[2*i+1]=' ';
                }
                display_text("NewCode",0,0);
                blue_send("---\r\n");
                blue_send("$$$");
                blue_send("d\r\n");
                blue_send("---\r\n");
                blue_send("---");
                if (blue_steal()){
                    code = 1;
                    display_text("      ",1,1);
                }
                else {
                    code = 1;
                    while (code != 5){
                        char key = keypad();
                        if (key != '\0'){
                            if ((key == '*')&&(code!=1)){
                            code--;
                            myConstChar[2*(code-1)+1] = ' ';
                            display_text(myConstChar, 1, 1);
                            sleep_ms(50);
                            while(keypad() != '\0'){}
                        }
                            else if (key == '#'){
                            code = 1;
                                for (int i=0;i<4;i++){
                                    myConstChar[2*i]=' ';
                                    myConstChar[2*i+1]=' ';
                                }
                                display_text(myConstChar, 1, 1);
                            sleep_ms(50);
                            while(keypad() != '\0'){}
                        }
                            else if (code != 0){
                                MysecretCode[code-1] = key-48;
                                myBlankConstChar[2*(code-1)+1] = '*';
                                myConstChar[2*(code-1)+1] = key;
                                code+=1;
                                display_text(myConstChar, 1, 1);
                                sleep_ms(50);
                                while(keypad() != '\0'){}
                            }
                        }
                    }
                }
            }
        }
        else if (start == 1){
            gpio_put(pin_fuel,1);
            uint16_t result = adc_read(); // Read the ADC value
            while(result<=500){
                result = adc_read(); // Read the ADC value
            }
            sleep_ms(5000);
            while(result>=500){
                result = adc_read(); // Read the ADC value
            }
            start=0;
            code=1;
            for (int i=0;i<4;i++){
                myNumber[i]=0;
                myConstChar[2*i]=' ';
                myConstChar[2*i+1]=' ';
                myBlankConstChar[2*i]=' ';                            
                myBlankConstChar[2*i+1]=' ';
            }
            display_text(myBlankConstChar, 1, 1);
        }
        if (start == 0){
            gpio_put(pin_fuel,0);
        }
    }
}
