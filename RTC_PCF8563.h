#ifndef RTC_PCF8563_H
#define RTC_PCF8563_H

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "RTC_I2C.h"


class RTC_PCF8563 {
  public:
  RTC_I2C i2c;
  
  RTC_PCF8563();
  RTC_PCF8563(uint8_t PCF8563_address_);
  
  void setData(uint8_t day, uint8_t weekday, uint8_t month, uint8_t year);
  void setTime(uint8_t second, uint8_t minute, uint8_t hour);
  void setAlarm(uint8_t minute, bool m_on, uint8_t hour, bool h_on, uint8_t day, bool d_on, uint8_t weekday, bool wd_on);
  uint8_t getSecond();
  uint8_t getMinute();
  uint8_t getHour();
  uint8_t getDay();
  uint8_t getWeekday();
  uint8_t getMonth();
  uint8_t getYear();
  uint8_t getminuteAlarm();
  uint8_t gethourAlarm();
  void INT();

  private:
  void setDay(uint8_t day);
  void setWeekday(uint8_t weekday);
  void setMonth(uint8_t month);
  void setYear(uint8_t year);
  void setSecond(uint8_t second);
  void setMinute(uint8_t minute);
  void setHour(uint8_t hour);
  void minuteAlarm(uint8_t minute, bool m_on);
  void hourAlarm(uint8_t hour, bool h_on);
  void dayAlarm(uint8_t day, bool d_on);
  void weekdayAlarm(uint8_t weekday, bool wd_on);
  uint8_t BDC_DecToBin(uint8_t value);
  uint8_t BDC_BinToDec(uint8_t value);
  
};


#endif
