/**
 * MulDevUsage.ino   23rd March 2023
 *
 * @author: Pravin Raghul
 * @description: Multiple EEPROM device read and write example
 */

#include <Arduino.h>
#include "AT24CM01.h"


AT24CM01 EEPROM;


void write(AT24CM01 &EEPROM)
{
  uint8_t send_data[8] {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  for (int i = 0; i < 8; i++)
  {
    Serial.print(send_data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  EEPROM.writeByteArray(0, send_data, 8);
}


void read(AT24CM01 &EEPROM)
{
  uint8_t read_data[8] {0, 0, 0, 0, 0, 0, 0, 0};

  EEPROM.readByteArray(0, read_data, 8);

  for (int i = 0; i < 8; i++)
  {
    Serial.print(read_data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void setup()
{
  Serial.begin(9600);

  EEPROM.begin();

  Serial.println("Writing to device 0x50");
  EEPROM.setDeviceAddress(ATDEVADR0); // 0x50
  write(EEPROM);

  Serial.println("Writing to device 0x52");
  EEPROM.setDeviceAddress(ATDEVADR1); // 0x52
  write(EEPROM);

  Serial.println("Reading from device 0x50");
  EEPROM.setDeviceAddress(ATDEVADR0); // 0x50
  read(EEPROM);

  Serial.println("Reading from device 0x52");
  EEPROM.setDeviceAddress(ATDEVADR1);  // 0x52
  read(EEPROM);
}

void loop(){}