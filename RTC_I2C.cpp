#include "RTC_I2C.h"

#define I2C_course_write    0b00000000
#define I2C_course_read     0b00000001  

RTC_I2C::RTC_I2C()
{
  //dioda wbudowana w płytkę, do informacji o błędzie
  DDRB |= (1<<PB5);
}

void RTC_I2C::I2C_init()
{
  //SCL frequency - 100000Hz
  //scl_freq = 16000000/(16+2(72)*1)= 100000Hz
  //TWBR = (f_cpu/(f_scl)-16)/(2*N)
  TWBR = 0b01001000; //72

  // prescaler = 1
  TWSR &= ~(1<<TWPS1);
  TWSR &= ~(1<<TWPS0);
}

void RTC_I2C::addressScanner()
{
  //iteracja przez wszyskite możliwe wartości zapisu 7-bitowego adresu
  for(int i=1; i<127; i++)
  {
    I2C_start();
    
    PCF8563_address = (i<<1)|(I2C_course_write); 
    TWDR = PCF8563_address;
    TWCR = (1<<TWINT) | (1<<TWEN);
  
    //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
    while (!(TWCR & (1<<TWINT)))
    {}
  
    //SLA+W(adres urządzenia slave + tryb zapisu do slave) została przekazana, ACK został odebrany
    if ((TWSR & 0xF8) == 0x18)
    {
       PCF8563_address = i;
       I2C_stop();    
       return;
    }

    I2C_stop(); 
    _delay_ms(10);
  }  
}

void RTC_I2C::I2C_start()
{
  //Master Transmitter Mode
  //start
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  
  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że warunek START został przesłany
  while (!(TWCR & (1<<TWINT)))
  {}

  //Sprawdzenie czy bajt danych został przesłany, ACK został odebrany
  error(0x08);
}

void RTC_I2C::I2C_firstByte(char direction)
{
  uint8_t address = 0;
  //adres urządzenia i bit kierunku (0, bo zapis danych z master do slave)
  if(direction == 'W')
  {
    address = (PCF8563_address<<1)|(I2C_course_write); 
  }
  else if(direction == 'R')
  {
    address = (PCF8563_address<<1)|(I2C_course_read); 
  }
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);

  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
  while (!(TWCR & (1<<TWINT)))
  {}

  //Sprawdzenie czy bajt danych został przesłany, ACK został odebrany
  if(direction == 'W')
  {
    error(0x18);
  }
  else if(direction == 'R')
  {
    error(0x40);
  }
}

void RTC_I2C::I2C_writeData(uint8_t address, uint8_t DATA)
{
  //Załaduj adres do rejestru TWDR. Wyczyść bit TWINT w TWCR, aby rozpocząć transmisję danych
  TWDR = address;
  TWCR = (1<<TWINT) | (1<<TWEN);

  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
  while (!(TWCR & (1<<TWINT)))
  {}

  //Sprawdzenie czy bajt danych został przesłany, ACK został odebrany
  error(0x28);

  //Załaduj dane do rejestru TWDR. Wyczyść bit TWINT w TWCR, aby rozpocząć transmisję danych
  TWDR = DATA;
  TWCR = (1<<TWINT) | (1<<TWEN);

  //Poczekaj na ustawienie flagi TWINT. Oznacza to, że DANE zostały przesłane i odebrano ACK/NACK
  while (!(TWCR & (1<<TWINT)))
  {}

  //Sprawdzenie czy bajt danych został przesłany, ACK został odebrany
  error(0x28);
}

uint8_t RTC_I2C::I2C_readData(uint8_t registerAddress)
{
  I2C_start();
  I2C_firstByte('W');
  
  //Wysłanie adresu rejestru, z którego chcemy odczytać dane
  TWDR = registerAddress;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)));    
  error(0x28);  
    
  I2C_start();
  I2C_firstByte('R');
  
  //Odbiór danych
  TWCR = (1 << TWINT) | (1 << TWEN); 
  while (!(TWCR & (1 << TWINT)));  
  uint8_t data = TWDR;       
  
  I2C_stop();
  
  return data; 
}

void RTC_I2C::I2C_stop()
{
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
  while(TWCR & (1<<TWSTO)) 
  {}
}

void RTC_I2C::error(uint8_t value)
{
  if ((TWSR & 0xF8) != value)
  {
    PORTB |= (1<<PB5);  
  }
  else
  {
    PORTB &= ~(1<<PB5);
  }
}
