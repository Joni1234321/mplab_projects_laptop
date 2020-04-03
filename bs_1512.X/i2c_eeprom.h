#ifndef __I2C_EEPROM_H__
#define __I2C_EEPROM_H__

/*
 * 
 * MAX 64 BYTES OF DATA
 */
void eeprom_page_write (uint8_t slaveAdd, uint16_t add, uint8_t *eeData, uint32_t eeDataSize);
// Returns an array of 4 bits
uint8_t* eeprom_sequential_4_read (uint8_t slaveAdd, uint16_t add);

// Override
void eeprom_write_16 (uint8_t slaveAdd, uint16_t add, uint8_t eeData);
uint8_t eeprom_read_16 (uint8_t slaveAdd, uint16_t add);

// EEPROM
void eeprom_write (uint8_t slaveAdd, uint8_t addHigh, uint8_t addLow, uint8_t eeData);
uint8_t eeprom_read (uint8_t slaveAdd, uint8_t addHigh, uint8_t addLow);

#endif