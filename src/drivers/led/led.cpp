#include "led.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file

uint32_t led_data[LED_COUNT]; // Define led_data

void led_init(){
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

void led_load() {
    // reloads all LEDs from memory
    for (int i = 0; i < LED_COUNT; i++) {
        pio_sm_put_blocking(pio0, 0, led_data[i]);
    }
    sleep_ms(1);
}

void led_set(int led, int r, int g, int b) {
    // sets led in memory
    uint8_t red = r;
    uint8_t green = g;
    uint8_t blue = b;
    led_data[led] = (red << 24) | (green << 16) | (blue << 8);
}

void led_set(int led, uint32_t rgb) {
    int r = (rgb >> 24) & 0xFF;
    int g = (rgb >> 16) & 0xFF;
    int b = (rgb >> 8) & 0xFF;
    led_set(led, r, g, b);
}

void led_all(bool on) {
    // all on/off
    for (int i = 0; i < LED_COUNT; i++) {
        led_set(i, on * 255, on * 255, on * 255);
    }
}

uint32_t led_get(int led) {
    return led_data[led];
}

bool led_check(int led) {
    if (led_data[led]){
        return 1;
    }
    return 0;
}

void led_spin(){
    uint32_t temp = led_get(12);
    for (int i = 12; i > 0; i-- ) {
        led_set(i,led_get(i-1));
    } 
    led_set(0,temp);
}
