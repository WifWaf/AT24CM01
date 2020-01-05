#include <Arduino.h>
#include "AT24CM01.h"

AT24CM01 EEPROM;              // Library 

#define INDEX_SIZE 6

char array[INDEX_SIZE] = "12345";    // arrays values to be stored
int ibuff = 2839;                    // int value to be stored
float fbuff = -245.2049;             // float value to be stored

void setup() 
{
  Serial.begin(9600);

  EEPROM.begin();

  Serial.print("\n\n ...........Writing Variables.............");

  /* int and float values ----------------------------------------------- */
  Serial.print("\n\nRecalling Int\n");
  EEPROM.writeInt(0, ibuff);                            // WriteVartype(eeprom address, value)
  Serial.print(EEPROM.readInt(0));                      // EEPROM.readVarType(eeprom address)
                                                    
  Serial.print("\nRecalling Float\n");
  EEPROM.writeFloat(10, fbuff);
  Serial.print(EEPROM.readFloat(10), 7);                // EEPROM.readVarType(eeprom address), print with 7 decimal place resolution

  /* Array valuess ---------------------------------- */
  Serial.print("\nRecalling Array\n");
  EEPROM.writeCharArray(20, array, INDEX_SIZE-1);        // EEPROM.writeArrayType(address, array, arraysize)  
  EEPROM.readCharArray(20, array, INDEX_SIZE-1);         // ReadArrayType(address, textarraybuffer, textsize)
  Serial.print(array);
}


void loop(){}
