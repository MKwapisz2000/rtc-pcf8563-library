# rtc-pcf8563-library

**Description - English**

The RTC_PCF8563 library is designed to control the PCF8563 Real-Time Clock (RTC) using the I2C bus. It allows easy date and time setting and reading, as well as configuring alarms that trigger external interrupts. It includes built-in functions for BCD conversion and automatic device address detection on the I2C bus.

Features

- PCF8563 I2C communication

- Date and time setting & reading

- Alarm configuration with interrupt support

- BCD conversion

- Device address detection on I2C bus

Example Usages:

    #include "RTC_PCF8563.h"
    
    RTC_PCF8563 rtc;
    
    void setup() {
        Serial.begin(9600);
        
        // Ustawienie daty: dzień, dzień tygodnia, miesiąc, rok
        rtc.setData(15, 3, 2, 24);
    
        // Ustawienie czasu: sekunda, minuta, godzina
        rtc.setTime(0, 30, 12);
    
        // Ustawienie alarmu
        rtc.setAlarm(31, 1, 12, 1, 15, 1, 3, 1);
    }
    
    void loop() {
        Serial.print("Godzina: ");
        Serial.print(rtc.getHour());
        Serial.print(":");
        Serial.print(rtc.getMinute());
        Serial.print(":");
        Serial.println(rtc.getSecond());
        
        delay(1000);
    }


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

**Opis - Polski**

Biblioteka RTC_PCF8563 została stworzona do obsługi zegara czasu rzeczywistego PCF8563 za pomocą magistrali I2C. Umożliwia łatwe ustawianie i odczytywanie daty oraz godziny, a także konfigurację alarmów wywołujących przerwania zewnętrzne. Obsługuje konwersję BCD na format dziesiętny i odwrotnie. Wbudowane funkcje pozwalają na wykrywanie urządzenia na magistrali I2C i automatyczne przypisanie adresu.

Funkcje

- Obsługa PCF8563 przez I2C

- Ustawianie i odczytywanie daty oraz godziny

- Konfiguracja alarmu z przerwaniem

- Konwersja BCD

- Wykrywanie adresu urządzenia na magistrali I2C

Przykład użycia:

    #include "RTC_PCF8563.h"
    
    RTC_PCF8563 rtc;
    
    void setup() {
        Serial.begin(9600);
        
        // Ustawienie daty: dzień, dzień tygodnia, miesiąc, rok
        rtc.setData(15, 3, 2, 24);
    
        // Ustawienie czasu: sekunda, minuta, godzina
        rtc.setTime(0, 30, 12);
    
        // Ustawienie alarmu
        rtc.setAlarm(31, 1, 12, 1, 15, 1, 3, 1);
    }
    
    void loop() {
        Serial.print("Godzina: ");
        Serial.print(rtc.getHour());
        Serial.print(":");
        Serial.print(rtc.getMinute());
        Serial.print(":");
        Serial.println(rtc.getSecond());
        
        delay(1000);
    }
