/**
 * MulDevUsage.ino   23rd March 2023
 *
 * @author: Pravin Raghul
 * @description: Multiple EEPROM device read and write example
 */

#include <Arduino.h>
#include "AT24CM01.h"


AT24CM01 eeprom;


void write(AT24CM01 &eeprom)
{
  uint8_t data[] {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};

  for (int i = 0; i < 8; i++) {
    Serial.printf("0x%x, ", data[i]);
  }
  Serial.println();

  eeprom.writeByteArray(0, data, sizeof(data));
}


void read(AT24CM01 &eeprom)
{
  uint8_t buff[8] {};

  eeprom.readByteArray(0, buff, sizeof(buff));

  for (int i = 0; i < 8; i++) {
    Serial.printf("0x%x, ", buff[i]);
  }
  Serial.println();
}


void setup()
{
  Serial.begin(9600);

  eeprom.begin();

  Serial.println("Writing to device 0x50");
  eeprom.setDeviceAddress(ATDEVADR0);
  write(eeprom); // 0x50

  Serial.println("Writing to device 0x52");
  eeprom.setDeviceAddress(ATDEVADR1);
  write(eeprom); // 0x52

  Serial.println("Reading to device 0x50");
  eeprom.setDeviceAddress(ATDEVADR0);
  read(eeprom);  // 0x50

  Serial.println("Reading to device 0x52");
  eeprom.setDeviceAddress(ATDEVADR1);
  read(eeprom);  // 0x52
}

void loop(){}
