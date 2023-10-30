#include "pico/stdlib.h"
#include "lis3dh.h"
#include "hardware/i2c.h"
    
//LIS3DH registers
int turn_on = 0x20;
int  turn_on2 = 0x97;
int  mode_set = 0x23;
int  testing = 0x10;

uint8_t WHO_AM_I_REG = 0x0F;
int Mbits = 6;

void i2c_write(int place, int data) {
    if (data){
    uint8_t buf[2];
    buf[0]=place;
    buf[1]= data;
    i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, buf, 2, false);
    }
    else{
    uint8_t uint_data = place;
    i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &uint_data, 1, true);
    }
}

void i2c_read(int place, uint8_t *data){
    i2c_write(place);
    i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, *&data, 1, false);
}

void i2c_reads(int place, int16_t* data) {
    uint8_t uint_data = (place | 0x80);
    uint8_t raw_data[6];
    i2c_write_blocking(I2C_INSTANCE, I2C_ADDRESS, &uint_data, 1, true);
    i2c_read_blocking(I2C_INSTANCE, I2C_ADDRESS, raw_data, 6, false);
    
    int16_t data16[3];
    data16[0] = (int16_t)(raw_data[0] | (raw_data[1] << 8)) >> Mbits;
    data16[1] = (int16_t)(raw_data[2] | (raw_data[3] << 8)) >> Mbits;
    data16[2] = (int16_t)(raw_data[4] | (raw_data[5] << 8)) >> Mbits;
    
    for (int i = 0; i < 3; i ++){
    data[i] = data16[i];
    }
}

void i2c_init(){
// Initialize I2C
    i2c_init(I2C_INSTANCE, 400*1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);

    // Set the I2C address to the LIS3DH's address
    i2c_set_slave_mode(I2C_INSTANCE, false, I2C_ADDRESS);

    i2c_write(turn_on, turn_on2);

    i2c_write(mode_set, testing);
}