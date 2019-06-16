#ifndef _AT24CM01_H
#define _AT24CM01_H

#include <Arduino.h>
#include <Wire.h>

#define ATWDelay 5                   // Delay to allow write to be completed (datasheet shows max 5ms)
#define AT2CSPEED 400000             // Clock speed for I2C
#define ATMAXADR 131072              // Size of EEPROM in words
#define ATDEVADR 0x50                // Lowest page address

class AT24CM01
{
    public:
        AT24CM01(uint8_t sda = 21, uint8_t sdl = 22, TwoWire &inWire = Wire);

        void begin();

        uint32_t read(uint32_t address, uint8_t size = 1);
        void read(uint32_t address, uint8_t data[], uint8_t indexCount);

        void write(uint32_t address, uint8_t data[], uint8_t indexCount);
        void write(uint32_t address, uint32_t data);
        void write(uint32_t address, uint32_t data, uint8_t size);

    protected:
    private:
        uint8_t _sda, _sdl;          // I2C pins
        TwoWire* ATWire;             // TwoWire library isntance     
};

#endif