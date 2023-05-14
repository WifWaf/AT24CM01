/*
  Author: Jonathan Dempsey JDWifWaf@gmail.com
  Version: 2.3.0
  License: Apache 2.0
*/

#include <Wire.h>
#include "AT24CM01.h"

#define DEBUG 0

#define GETADR(MGADR, DEVADR) ((MGADR >> 16) | DEVADR)
#define CHCKSIZE(x) ((x) > 8 || (x) == 0) ? Serial.print("\n<AT24CM01> Error - Size given too large or small (1-8)") : 0
#define CHCKADDR(x) ((x) > ATMAXADR) ? Serial.print("\n<AT24CM01> Error - Address given too large (0 - 131072)") : 0

/**
 * This function sets up the AT24CM01 EEPROM for use with the Arduino.
 *
 * @param inWire The TwoWire object you want to use.
 * @param device_adr The address of the device on the I2C bus.
 */
void AT24CM01::begin(TwoWire &inWire, uint8_t device_adr)
{
  this->ATWire = &inWire;
  this->ATWire->begin();
  this->ATWire->setClock(AT2CSPEED);
  this->ATDevAdr = device_adr;
}

/**
 * This function sets the device address of the AT24CM01 EEPROM
 *
 * @param device_adr The address of the device.
 */
void AT24CM01::setDeviceAddress(uint8_t device_adr)
{
  this->ATDevAdr = device_adr;
}

/**
 * Reads a 32-bit value from the specified address
 *
 * @param address The address of the memory location you want to read from.
 * @param size The number of bytes to read.
 *
 * @return The data read from the EEPROM.
 */
uint32_t AT24CM01::read(uint32_t address, uint8_t size)
{
  if (CHCKADDR(address) || CHCKSIZE(size))
    return 0;

  this->ATWire->beginTransmission((uint8_t)GETADR(address, ATDevAdr));
  this->ATWire->write((uint8_t)((address & 0xFFFF) >> 8));
  this->ATWire->write((uint8_t)(address & 0xFF));
  this->ATWire->endTransmission();

  this->ATWire->requestFrom((uint8_t)GETADR(address, ATDevAdr), (uint8_t)size);
  uint32_t buff0 = 0;
  uint32_t buff1 = 0;
  if (this->ATWire->available())
  {
    for (uint8_t x = 0; x < size; x++)
    {
      buff1 = this->ATWire->read();
      if (!buff1)
        continue;
      buff0 |= (buff1 << x * 8);
    }
  }
  return buff0;
}

/**
 * The function reads a specified number of bytes from the specified address and stores them in the
 * specified array
 *
 * @param address The address of the first byte to read.
 * @param data The array of data to be written to the EEPROM.
 * @param indexCount The number of bytes to read.
 *
 * @return The data read from the EEPROM.
 */
void AT24CM01::read(uint32_t address, uint8_t data[], uint8_t indexCount)
{
  if (CHCKADDR(address))
    return;

  memset(data, 0, indexCount);

  this->ATWire->beginTransmission((uint8_t)GETADR(address, ATDevAdr));
  this->ATWire->write((uint8_t)((address & 0xFFFF) >> 8));
  this->ATWire->write((uint8_t)(address & 0xFF));
  this->ATWire->endTransmission();

  this->ATWire->requestFrom((uint8_t)GETADR(address, ATDevAdr), indexCount);
  if (this->ATWire->available())
  {
    for (uint16_t x = 0; x <= indexCount; x++)
      data[x] = this->ATWire->read();
  }
}

/**
 * The function takes an address and data, and writes the data to the address
 *
 * @param address The address of the memory location to write to.
 * @param data The data to be written to the EEPROM.
 *
 * @return a boolean value.
 */
void AT24CM01::write(uint32_t address, uint32_t data)
{
  if (CHCKADDR(address))
    return;

  this->ATWire->beginTransmission((uint8_t)GETADR(address, ATDevAdr)); // Start, and determine segment address
  this->ATWire->write((uint8_t)((address & 0xFF00) >> 8));             // Set curser to address for writing
  this->ATWire->write((uint8_t)(address & 0xFF));

  uint8_t sBuff = 0;
  for (uint8_t x = 0; (data & (0xFFFFFFFF << x * 8)); x++) // Determine data size
  {
    sBuff = (data >> x * 8) & 0xFF; // Value if required into appropriate parts
    this->ATWire->write(sBuff);     // Address rolls over when stop is not called - see datasheet.
  }

  this->ATWire->endTransmission(); // Stop

  delay(ATWDelay); // Delay required to stop write/read failing if carried out abruptly
}

/**
 * The function takes an address, data and size as parameters. It then splits the data into bytes and
 * writes them to the address
 *
 * @param address The address of the memory location to write to.
 * @param data The data to be written to the EEPROM
 * @param size The number of bytes to write.
 *
 * @return a boolean value.
 */

void AT24CM01::write(uint32_t address, uint32_t data, uint8_t size)
{
  if (CHCKADDR(address) || CHCKSIZE(size))
    return;
  this->ATWire->beginTransmission((uint8_t)GETADR(address, ATDevAdr)); // Start, and determine segment address
  this->ATWire->write((uint8_t)((address & 0xFF00) >> 8));             // Set curser to address for writing
  this->ATWire->write((uint8_t)(address & 0xFF));

  uint8_t sBuff = 0;
  for (uint8_t x = 0; x < size; x++)
  {
    sBuff = (data >> x * 8) & 0xFF; // Split into bytes and write
    this->ATWire->write(sBuff);     // Address rolls over when stop is not called - see datasheet;
  }

  this->ATWire->endTransmission(); // Stop

  delay(ATWDelay); // Delay required to stop write/read failing if carried out abruptly
}

/**
 * This function writes a byte array to the specified address
 *
 * @param address The address of the first byte to write to.
 * @param data The data to be written to the EEPROM.
 * @param indexCount The number of bytes to write.
 *
 * @return Nothing.
 */

void AT24CM01::write(uint32_t address, uint8_t data[], uint8_t indexCount)
{
  if (CHCKADDR(address))
    return;

  this->ATWire->beginTransmission((uint8_t)GETADR(address, ATDevAdr));
  this->ATWire->write((uint8_t)((address & 0xFF00) >> 8));
  this->ATWire->write((uint8_t)(address & 0xFF));

  for (uint16_t x = 0; x <= indexCount; x++)
    this->ATWire->write(data[x]);

  this->ATWire->endTransmission();

  delay(ATWDelay);
}
