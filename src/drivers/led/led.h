#pragma once

#include <stdint.h>
#include <stdbool.h>

// Define the number of LEDs and pin
#define LED_COUNT 12
#define LED_PIN 14

// Function to Initialise LED chain
void led_init();

// Function to update all LEDs
void led_load();

// Function to set the color of a single LED in memory
void led_set(int led, int r, int g, int b);

// Function to set the color of a single LED in memory
void led_set(int led, uint32_t rgb);

// Function to turn all LEDs on or off
void led_all(bool on = 0);

// Function to return if LED on.
bool led_check(int led);

// Function to return current data of LED.
uint32_t led_get(int led);

//Function to rotate LED 1
void led_spin();