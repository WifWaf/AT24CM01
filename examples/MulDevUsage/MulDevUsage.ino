/**
 * MulDevUsage.ino   23rd March 2023
 *
 * @author: Pravin Raghul
 * @description: Multiple EEPROM device read and write example
 */

#include <Arduino.h>
#include "AT24CM01.h"


AT24CM01 eeprom_dev1, eeprom_dev2;


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

  uint8_t buffer[8] {};

  Serial.begin(9600);

  eeprom_dev1.begin();
  eeprom_dev2.begin();

  // setting device address for eeprom device 2
  eeprom_dev2.setDeviceAddress(ATDEVADR1);

  Serial.println("Writing to device 0x50");
  write(eeprom_dev1); // 0x50

  Serial.println("Writing to device 0x52");
  write(eeprom_dev2); // 0x52

  Serial.println("Reading to device 0x50");
  read(eeprom_dev1);  // 0x50

  Serial.println("Reading to device 0x52");
  read(eeprom_dev2);  // 0x52
}

void loop(){}
