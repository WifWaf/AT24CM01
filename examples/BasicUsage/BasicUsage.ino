#include <Arduino.h>
#include "AT24CM01.h"

#define MSGINDEX 11                                                    // Used below for example

AT24CM01 EEPROM(21, 22);                                               // Pass your I2C pins and optional third argument
                                                                       // of a TwoWire instance.
void setup() 
{
  Serial.begin(115200); 

  EEPROM.begin();                                                      // Essential Begin

  /*SendRecieve Value.............................................................*/
  uint32_t toSend = 517891;                                            // Library takes a single value of up to 4 bytes
  Serial.print("\nStoring value << "); Serial.println(toSend);         // !Note - keep track of the addresses
  EEPROM.write(15, toSend);         // Specify address and value       // I.e. 517891 uses 3 bytes, so occupies addresses; 15,16 and 17
                                                                       // Optional third argument for size definition

  uint32_t recVal = EEPROM.read(15, 3);                                // Specify address and word size
  Serial.print("Retrieving value >> "); Serial.println(recVal);        // Library will read up to 4 bytes and return a single values
                                                                       // Use EEPROM.read(address) to read a single address value
                                                                       // Optional third argument for size defintion
  /*Send & Recieve Array.......................................................... */
  uint8_t sendMSG[MSGINDEX] = {'H','E','L','L','O',' ','W','O','R','L','D'};   // Array of bytes to be stored
  uint8_t recMSG[MSGINDEX];                                                    // Buffer for retrieved bytes
                            
  Serial.print("Storing Array << ");    
  for(uint8_t x = 0; x < MSGINDEX; x++)
  {
    Serial.print((char)sendMSG[x]);                                             // Print retrieved array
  }
  Serial.println("");

  EEPROM.write(20, sendMSG, MSGINDEX);                                          // Write - specify address, array and array index size
  EEPROM.read(20, recMSG, MSGINDEX);                                            // Read  - Specify address, array and array index size
    
  Serial.print("Retrieving Array >> ");
  for(uint8_t x = 0; x < MSGINDEX; x++)
  {
    Serial.print((char)recMSG[x]);                                               // Print retrieved array
  }
  Serial.println("");
}

void loop() {}
