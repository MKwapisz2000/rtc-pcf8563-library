#ifndef RTC_I2C_H
#define RTC_I2C_H

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

class RTC_I2C {
  public:
  uint8_t PCF8563_address;
  
  RTC_I2C();

  void I2C_init();
  void addressScanner();
  void I2C_start();
  void I2C_firstByte(char direction);
  void I2C_writeData(uint8_t address, uint8_t DATA);
  uint8_t I2C_readData(uint8_t registerAddress);
  void I2C_stop();
  void error(uint8_t value);

};


#endif
