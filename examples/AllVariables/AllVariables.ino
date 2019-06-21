#include <Arduino.h>
#include "AT24CM01.h"

AT24CM01 EEPROM;

#define TEST_BOOL   (false)
#define TEST_CHAR   ('A')
#define TEST_UCHAR  (254) 
#define TEST_BYTE   (254)
#define TEST_SHORT  (-32767)
#define TEST_WORD   (65535U)
#define TEST_INT    (-32767)
#define TEST_UINT   (65535U)
#define TEST_LONG   (-2147483647L)
#define TEST_ULONG  (4294967295UL)
#define TEST_FLOAT  (-2.12345F)
#define TEST_DOUBLE (-3.27677)

#define TEST_ARRAYCHARINDEX 5
#define TEST_ARRAYBYTEINDEX 5

/* 1 Byte Arrays */
char testCharArray[TEST_ARRAYCHARINDEX] = {'H','E','L','L', 'O'};
byte testByteArray[TEST_ARRAYBYTEINDEX] = {254,253,252,251,250};

void VariableTestWrite();
void VariableTestRead();

void setup() 
{
  Serial.begin(115200); 

  EEPROM.begin(21, 22);   // <--- pass your I2C pins and optional third agument for a TwoWire instance

  VariableTestWrite();
  VariableTestRead();
  
  while(1==1);           // End test
}

void loop(){}

void VariableTestWrite()
{
  printf("\n\n ...........Writing Variables.............");

  printf("\n          bool (%d): %s", sizeof(bool), TEST_BOOL ? "True" : "False"); EEPROM.writeBool(0, TEST_BOOL);
  printf("\n          byte (%d): %d", sizeof(byte), TEST_BYTE); EEPROM.writeByte(2, TEST_BYTE);

  printf("\n          char (%d): %c", sizeof(char), TEST_CHAR); EEPROM.writeChar(4, TEST_CHAR);
  printf("\n unsigned char (%d): %d", sizeof(unsigned char), TEST_UCHAR); EEPROM.writeUnsignedChar(6, TEST_UCHAR);

  printf("\n         short (%d): %hd", sizeof(short), TEST_SHORT); EEPROM.writeShort(8, TEST_SHORT);
  printf("\n          word (%d): %hd", sizeof(word), TEST_WORD); EEPROM.writeShort(12, TEST_WORD);

  printf("\n           int (%d): %d", sizeof(int), TEST_INT); EEPROM.writeInt(16, TEST_INT);
  printf("\n  unsigned int (%d): %u", sizeof(unsigned int), TEST_UINT); EEPROM.writeUnsignedInt(24, TEST_UINT);

  printf("\n          long (%d): %ld", sizeof(long), TEST_LONG); EEPROM.writeLong(32, TEST_LONG);
  printf("\n unsigned long (%d): %lu", sizeof(unsigned long), TEST_ULONG); EEPROM.writeUnsignedLong(40, TEST_ULONG);

  printf("\n         float (%d): %f", sizeof(float), TEST_FLOAT); EEPROM.writeFloat(48, TEST_FLOAT);
  printf("\n        double (%d): %f", sizeof(double), TEST_DOUBLE); EEPROM.writeDouble(56, TEST_DOUBLE);
  
  printf("\n    array char (%d): ", TEST_ARRAYCHARINDEX);
  for(uint8_t x = 0; x < TEST_ARRAYCHARINDEX; x++)
    printf("%c ", testCharArray[x]); 
  EEPROM.writeCharArray(64, testCharArray, TEST_ARRAYCHARINDEX);

  printf("\n    array byte (%d): ", TEST_ARRAYBYTEINDEX);
  for(uint8_t x = 0; x < TEST_ARRAYBYTEINDEX; x++)
    printf("%d ", testByteArray[x]);
  EEPROM.writeByteArray((64 + TEST_ARRAYCHARINDEX), testByteArray, TEST_ARRAYBYTEINDEX);
}

void VariableTestRead()
{
  char buffCharArray[TEST_ARRAYCHARINDEX];
  byte buffByteArray[TEST_ARRAYBYTEINDEX];

  printf("\n\n ..........Reading Variables..............");

  printf("\n          bool (%d): %s", sizeof(bool), EEPROM.readBool(0) ? "True" : "False");
  printf("\n          byte (%d): %d", sizeof(byte), EEPROM.readByte(2));

  printf("\n          char (%d): %c", sizeof(char), EEPROM.readChar(4));
  printf("\n unsigned char (%d): %d", sizeof(unsigned char), EEPROM.readUnsignedChar(6));

  printf("\n         short (%d): %hd", sizeof(short), EEPROM.readShort(8));
  printf("\n          word (%d): %hd", sizeof(word), EEPROM.readword(12));

  printf("\n           int (%d): %d", sizeof(int), EEPROM.readInt(16));
  printf("\n  unsigned int (%d): %d", sizeof(unsigned int), EEPROM.readUnsignedInt(24));

  printf("\n          long (%d): %ld", sizeof(long), EEPROM.readLong(32));
  printf("\n unsigned long (%d): %lu", sizeof(unsigned long), EEPROM.readUnsignedLong(40));

  printf("\n         float (%d): %f", sizeof(float), EEPROM.readFloat(48));
  printf("\n        double (%d): %f", sizeof(double), EEPROM.readDouble(56));
    
  EEPROM.readCharArray(64, buffCharArray, TEST_ARRAYCHARINDEX); 
  printf("\n    array char (%d): ", TEST_ARRAYCHARINDEX);
  for(uint8_t x = 0; x < TEST_ARRAYCHARINDEX; x++)
    printf("%c ", buffCharArray[x]);

  EEPROM.readByteArray((64+TEST_ARRAYCHARINDEX), buffByteArray, TEST_ARRAYBYTEINDEX); 
  printf("\n    array byte (%d): ", TEST_ARRAYBYTEINDEX);
  for(uint8_t x = 0; x < TEST_ARRAYBYTEINDEX; x++)
    printf("%d ", buffByteArray[x]);

  printf("\n ");    // Flush
}