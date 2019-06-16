#include <Wire.h>
#include "AT24CM01.h"

#define DEBUG 0
           
#define GETADR(MGADR) (((ATDEVADR<<16) | MGADR)>>16)
#define CHCKSIZE(x) ((x)>4 || (x)==0) ? Serial.println("\r<AT24CM01> Error - Size given too large or small (1-4)") : 0
#define CHCKADDR(x) ((x)>ATMAXADR) ? Serial.print("\r<AT24CM01> Error - Address given too large (0 - 131072)") : 0

/*
#define AT24CM01_ADR_A1H 0x08
#define AT24CM01_ADR_A2H 0x14
#define AT24CM01_ADR_A1L 0x00
#define AT24CM01_ADR_A2L 0x00            // Not needed unless you want to determine address manually and shift the bytes

#define AT24CM01_PAGE_ADR0 0x50
#define AT24CM01_PAGE_ADR1 0x51
#define AT24CM01_WPAGE_ADR0 0x00
#define AT24CM01_WPAGE_ADR1 0x02
#define AT24CM01_WORD_ADR 0xA0
*/

AT24CM01::AT24CM01(uint8_t sda, uint8_t sdl, TwoWire &inWire) : _sda(sda), _sdl(sdl), ATWire(&inWire){}; 

void AT24CM01::begin() 
{
  ATWire->begin(this->_sda, this->_sdl);
  ATWire->setClock(AT2CSPEED);
}

uint32_t AT24CM01::read(uint32_t address, uint8_t size)
{  
  if(CHCKADDR(address) || CHCKSIZE(size))
    return 0;

  ATWire->beginTransmission((uint8_t)GETADR(address));
  ATWire->write((uint8_t)((address & 0xFFFF) >> 8));
  ATWire->write((uint8_t)(address & 0xFF));
  ATWire->endTransmission();

  ATWire->requestFrom((uint8_t)GETADR(address), (uint8_t)size); 
  uint32_t buff0 = 0;
  uint8_t buff1 = 0; 
  if(ATWire->available())
  { 
    for(uint8_t x = 0; x < size; x++)
    {
      buff1 = ATWire->read();
      #if DEBUG
      Serial.print("<AT24CM01> (read_32) Read >> ");
      Serial.println(buff1);
      #endif
      buff0 |= (buff1 << x*8);
    }
  }
  return buff0;
}

void AT24CM01::read(uint32_t address, uint8_t data[], uint8_t indexCount)
{
  if(CHCKADDR(address))
    return;

  memset(data, 0, indexCount);
 
  ATWire->beginTransmission((uint8_t)GETADR(address));
  ATWire->write((uint8_t)((address & 0xFFFF) >> 8));
  ATWire->write((uint8_t)(address & 0xFF));
  ATWire->endTransmission();

  ATWire->requestFrom((uint8_t)GETADR(address), indexCount); 
  if(ATWire->available())
  { 
    for(uint8_t x = 0; x < indexCount; x++)
    {
      data[x] = ATWire->read();
      #if DEBUG
      Serial.print("<AT24CM01> (read_ar) Read >> ");
      Serial.println(data[x]);
      #endif
    }
  }
}

void AT24CM01::write(uint32_t address, uint32_t data)
{
  if(CHCKADDR(address))
    return;

  ATWire->beginTransmission((uint8_t)GETADR(address));                // Start, and determine segment address 
  ATWire->write((uint8_t)((address & 0x00FF) >> 8));                   // Set curser to address for writing
  ATWire->write((uint8_t)(address & 0xFF));
  
  uint8_t sBuff = 0;
  for(uint8_t x = 0; (data & (0xFFFFFFFF << x*8)); x++)                // Determine data size
  {                                                                    
    sBuff = (data >> x*8) & 0xFF;                                      // Value if required into appropriate parts
    ATWire->write(sBuff);                                              // Address rolls over when stop is not called - see datasheet.
    #if DEBUG
    Serial.print("<AT24CM01> (write_32) Write << ");
    Serial.println(sBuff);
    #endif
  }
  
  ATWire->endTransmission();                                           // Stop
  
  delay(ATWDelay);                                                     // Delay required to stop write/read failing if carried out abruptly
}

void AT24CM01::write(uint32_t address, uint32_t data, uint8_t size)
{
  if(CHCKADDR(address) || CHCKSIZE(size))
    return;

  if(size > 4 || size == 0)
  {
    Serial.println("<AT24CM01> (write_s) Error, size too large/small (1-4)");
    return;
  }

  ATWire->beginTransmission((uint8_t)GETADR(address));                 // Start, and determine segment address 
  ATWire->write((uint8_t)((address & 0x00FF) >> 8));                   // Set curser to address for writing
  ATWire->write((uint8_t)(address & 0xFF));
  
  uint8_t sBuff = 0;
  for(uint8_t x = 0; size; x++)                                        
  {                                                                   
    sBuff = (data >> x*8) & 0xFF;                                      // Split into bytes and write
    ATWire->write(sBuff);                                              // Address rolls over when stop is not called - see datasheet;
    #if DEBUG
    Serial.print("<AT24CM01> (write_s) Write << ");
    Serial.println(sBuff);
    #endif
  }
  
  ATWire->endTransmission();                                           // Stop
  
  delay(ATWDelay);                                                     // Delay required to stop write/read failing if carried out abruptly
}

void AT24CM01::write(uint32_t address, uint8_t data[], uint8_t indexCount)
{
  if(CHCKADDR(address))
    return;
  
  ATWire->beginTransmission((uint8_t)GETADR(address));
  ATWire->write((uint8_t)((address & 0x00FF) >> 8));
  ATWire->write((uint8_t)(address & 0xFF));
  
  for(uint8_t x = 0; x < indexCount; x++)
  {
    ATWire->write(data[x]);
    #if DEBUG
    Serial.print("<AT24CM01> (write_ar) Write << ");
    Serial.println(data[x]);
    #endif
  }
  ATWire->endTransmission();
  
  delay(ATWDelay);
}
