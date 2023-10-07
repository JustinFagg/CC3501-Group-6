#pragma once

#include <stdint.h>
#include <stdbool.h>



#define I2C_INSTANCE i2c0
#define SDA_PIN 16
#define SCL_PIN 17
#define I2C_ADDRESS 0x19
#define n28 0x28


// Writes to I2c Address
void i2c_write(int place, int data = 0);

// Reads Singlar Data
void i2c_read(int place, uint8_t *data);

// Reads Multiple Data
void i2c_reads(int place, int16_t* data);

//Function to Initialise I2C
void i2c_init();