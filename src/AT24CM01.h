#ifndef _AT24CM01_H
#define _AT24CM01_H

#include <stdint.h>
#include <stdbool.h>

#define AT24CM01_ADR_A1H 0x08
#define AT24CM01_ADR_A2H 0x14
#define AT24CM01_ADR_A1L 0x00
#define AT24CM01_ADR_A2L 0x00

#define AT24CM01_PAGE_ADR0 0x50
#define AT24CM01_PAGE_ADR1 0x51
#define AT24CM01_WPAGE_ADR0 0x00
#define AT24CM01_WPAGE_ADR1 0x02
#define AT24CM01_WORD_ADR 0xA0

bool AT24CM01begin();  
uint8_t AT24CM01I2CSetup(); 
uint8_t AT24CM01read(uint32_t address);

#endif