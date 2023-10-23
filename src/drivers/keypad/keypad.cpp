#include "pico/stdlib.h"
#include "keypad.h"
#include "arm_math.h"

const int numRows = 4;
const int numCols = 3;
// Define the GPIO connected to keypad rows and columns (ordering important)
uint rowPins[numRows];
uint colPins[numCols];


// Define the symbol associated with each key
char keyList[numRows][numCols] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Scan the keypad to detect keypresses
char keypad() {
    for (int i = 0; i < numRows; i++) {
        gpio_put(rowPins[i], 0);
        int result[numCols+1] = {1,gpio_get(colPins[0]), gpio_get(colPins[1]), gpio_get(colPins[2])};
        if ((result[1]*result[2]*result[3]) == 0) {
            char key = keyList[i][result[1] ? (result[2] ? 2 : 1) : 0];
            gpio_put(rowPins[i], 1);
            return key;
        }
        gpio_put(rowPins[i], 1);
    }
    return '\0';  // No keypress detected
}

void keypad_init(const uint* col, const uint* row){
//Button Grid Setup
        for (int i = 0; i < numRows; i++) {
            rowPins[i]=row[i];
            gpio_init(rowPins[i]);
            gpio_set_dir(rowPins[i], GPIO_OUT);
            gpio_put(rowPins[i], 1);
        }
        for (int i = 0; i < numCols; i++) {
            colPins[i] = col[i];
            gpio_init(colPins[i]);
            gpio_set_dir(colPins[i], GPIO_IN);
            gpio_pull_up(colPins[i]);
        }
}
