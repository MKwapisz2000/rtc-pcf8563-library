#include "RTC_PCF8563.h"

#define CONTROL1            0x00
#define CONTROL2            0x01
#define SECOND              0x02
#define MINUTE              0x03
#define HOUR                0x04
#define DAY                 0x05
#define WEEKDAY             0x06
#define MONTH               0x07
#define YEAR                0x08
#define MINUTE_ALARM        0x09
#define HOUR_ALARM          0x0A
#define DAY_ALARM           0x0B
#define WEEKDAY_ALARM       0x0C

RTC_PCF8563::RTC_PCF8563()
{
  //pull-up INT0
  DDRD &= ~(1 << PD2); 
  PORTD |= (1 << PD2); 
  
  i2c.I2C_init();
  i2c.addressScanner(); 
}

RTC_PCF8563::RTC_PCF8563(uint8_t PCF8563_address_)
{
  //pull-up INT0
  DDRD &= ~(1 << PD2); 
  PORTD |= (1 << PD2); 
  
  i2c.I2C_init();
  i2c.PCF8563_address = PCF8563_address_;
}

void RTC_PCF8563::setData(uint8_t day, uint8_t weekday, uint8_t month, uint8_t year)
{
  setDay(day);
  setWeekday(weekday);
  setMonth(month);
  setYear(year);
}

void RTC_PCF8563::setTime(uint8_t second, uint8_t minute, uint8_t hour)
{
  setSecond(second);
  setMinute(minute);
  setHour(hour);
}

void RTC_PCF8563::setAlarm(uint8_t minute, bool m_on, uint8_t hour, bool h_on, uint8_t day, bool d_on, uint8_t weekday, bool wd_on)
{
  minuteAlarm(minute, m_on);
  hourAlarm(hour, h_on);
  dayAlarm(day, d_on);
  weekdayAlarm(weekday, wd_on);
 
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  //ustawienie bitu AIE (flaga przerwania)
  i2c.I2C_writeData(CONTROL2, 0b00000010);  
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setDay(uint8_t day)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(DAY, BDC_DecToBin(day));
 
  i2c.I2C_stop();
}

void RTC_PCF8563::setWeekday(uint8_t weekday)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(WEEKDAY, weekday);
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setMonth(uint8_t month)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(MONTH, BDC_DecToBin(month));
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setYear(uint8_t year)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(YEAR, BDC_DecToBin(year));
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setSecond(uint8_t second)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(SECOND, BDC_DecToBin(second));
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setMinute(uint8_t minute)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');

  i2c.I2C_writeData(MINUTE, BDC_DecToBin(minute));
  
  i2c.I2C_stop();
}

void RTC_PCF8563::setHour(uint8_t hour)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  i2c.I2C_writeData(HOUR, BDC_DecToBin(hour));
  
  i2c.I2C_stop();
}

void RTC_PCF8563::minuteAlarm(uint8_t minute, bool m_on)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  if(m_on == 0)
  {
    i2c.I2C_writeData(MINUTE_ALARM, (0b10000000));
  }
  else
  {
    i2c.I2C_writeData(MINUTE_ALARM, (BDC_DecToBin(minute) & (0b01111111)));
  }

  i2c.I2C_stop();
}

void RTC_PCF8563::hourAlarm(uint8_t hour, bool h_on)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  if(h_on == 0)
  {
    i2c.I2C_writeData(HOUR_ALARM, (0b10000000));
  }
  else
  {
    i2c.I2C_writeData(HOUR_ALARM, (BDC_DecToBin(hour) & (0b00111111)));
  }

  i2c.I2C_stop();
}

void RTC_PCF8563::dayAlarm(uint8_t day, bool d_on)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  if(d_on == 0)
  {
    i2c.I2C_writeData(DAY_ALARM, (0b10000000));
  }
  else
  {
    i2c.I2C_writeData(DAY_ALARM, (BDC_DecToBin(day) & (0b00111111)));
  }

  i2c.I2C_stop();
}

void RTC_PCF8563::weekdayAlarm(uint8_t weekday, bool wd_on)
{
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  
  if(wd_on == 0)
  {
    i2c.I2C_writeData(WEEKDAY_ALARM, (0b10000000));
  }
  else
  {
     i2c.I2C_writeData(WEEKDAY_ALARM, (weekday & (0b00000111)));
  }

  i2c.I2C_stop();
}

uint8_t RTC_PCF8563::getSecond()
{
  uint8_t sec = i2c.I2C_readData(SECOND);
  return BDC_BinToDec(sec);  
}

uint8_t RTC_PCF8563::getMinute()
{
  uint8_t min = i2c.I2C_readData(MINUTE);
  min = min & 0b01111111;
  return BDC_BinToDec(min);  
}

uint8_t RTC_PCF8563::getHour()
{
  uint8_t h = i2c.I2C_readData(HOUR);
  h = h & 0b00111111;
  return BDC_BinToDec(h);  
}

uint8_t RTC_PCF8563::getDay()
{
  uint8_t d = i2c.I2C_readData(DAY);
  d = d & 0b00111111;
  return BDC_BinToDec(d);  
}

uint8_t RTC_PCF8563::getWeekday()
{
  uint8_t wd = i2c.I2C_readData(WEEKDAY);
  wd = wd & 0b00000111;
  return BDC_BinToDec(wd);  
}

uint8_t RTC_PCF8563::getMonth()
{
  uint8_t m = i2c.I2C_readData(MONTH);
  m = m & 0b00011111;
  return BDC_BinToDec(m);  
}

uint8_t RTC_PCF8563::getYear()
{
  uint8_t y = i2c.I2C_readData(YEAR);
  return BDC_BinToDec(y); 
}

uint8_t RTC_PCF8563::getminuteAlarm()
{
  uint8_t mA = i2c.I2C_readData(MINUTE_ALARM);
  mA = mA & 0b01111111;
  return BDC_BinToDec(mA);  
}

uint8_t RTC_PCF8563::gethourAlarm()
{
  uint8_t hA = i2c.I2C_readData(HOUR_ALARM);
  hA = hA & 0b00111111;
  return BDC_BinToDec(hA);  
}

uint8_t RTC_PCF8563::BDC_DecToBin(uint8_t value)
{
  uint8_t BDC = ((value/10)<<4) | (value%10);
  return BDC; 
}

uint8_t RTC_PCF8563::BDC_BinToDec(uint8_t value)
{
  uint8_t BDC1 = (value>>4) * 10;
  uint8_t BDC2 = (value & 0b00001111);
  uint8_t BDC = BDC1 + BDC2;
  return BDC;
}

void RTC_PCF8563::INT()
{
  uint8_t value = i2c.I2C_readData(CONTROL2);
  //wyczyszczenie bitu AF (flaga alarmu)
  value = value & 0b11110111;
  
  i2c.I2C_start();
  i2c.I2C_firstByte('W');
  i2c.I2C_writeData(CONTROL2, value);
  i2c.I2C_stop();
}
