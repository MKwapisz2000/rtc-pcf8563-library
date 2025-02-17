#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include "RTC_PCF8563.h"

int i = 0;
volatile bool alarm_flag = 0;

RTC_PCF8563 rtc1=RTC_PCF8563();





//.......................................................... MAIN ...............................................................
int main()
{
  char string[50];
  char string1[50];
  char string2[50];
  
  ExternalInterupts_init();
  ADC_init();
  USART_init();

  rtc1.setData(27,6,10,24);
  rtc1.setTime(50, 49, 19);
  rtc1.setAlarm(50, 1, 19, 1, 23, 0, 3,0);
 
  while (1) 
  { 
    if(alarm_flag == 1)
    {
      float soil_moisture = checkMoisture();
      dtostrf(soil_moisture, 5,1, string1);
      sprintf(string2, "Pomiar wilgotności gleby: %s%%\n", string1);
      USART_String(string2);
     
      alarm_flag = 0;
    }
    
    sprintf(string, "%d.%d.%d, %d  ->  ", rtc1.getDay(), rtc1.getMonth(), rtc1.getYear(), rtc1.getWeekday());
    USART_String(string);
    sprintf(string, "%d:%d:%d\n", rtc1.getHour(), rtc1.getMinute(), rtc1.getSecond());
    USART_String(string);

    i++;
    _delay_ms(1000);
   
  }
  return 0;
}



//.................................................... EXTERNAL_INT .............................................................
void ExternalInterupts_init()
{
  //failling edge
  EICRA |= (1<<ISC01);
  EIMSK |= (1<<INT0);

  //wlaczenie przerwań 
  sei();
}

ISR(INT0_vect)
{ 
  rtc1.INT();
  alarm_flag = 1;
}



//........................................................ USART ................................................................
void USART_init()
{
  UBRR0H = (unsigned char)(103>>8);
  UBRR0L = (unsigned char)103;
    
  //Adres we/wy rejestrów danych nadawczych USART i rejestry odbierania danych USART
  //UDR0

  //By bufor transmisji mógłbyć zapisany
  UCSR0A |= (1<<UDRE0);

  //Włączenie odbiornika
  UCSR0B |= (1<<RXEN0);

  //Włączenie nadajnika
  UCSR0B |= (1<<TXEN0);

  //Liczba bitów danych w ramce
  UCSR0C |= (1<<UCSZ00);
  UCSR0C |= (1<<UCSZ01);
}


void USART_Transmit( unsigned char data )
 {
 /* Wait for empty transmit buffer */
 while ( !( UCSR0A & (1<<UDRE0)) )
 ;
 /* Put data into buffer, sends the data */
 UDR0 = data;
 }

unsigned char USART_Receive()
 {
 /* Wait for data to be received */
 while ( !(UCSR0A & (1<<RXC0)) )
 ;
 /* Get and return received data from buffer */
 return UDR0;
 }

void USART_String(const char *array)
{
  int i=0;
  while(array[i]!='\0')
  {
    USART_Transmit(array[i]);
    i++;
  }
}



//.......................................................... ADC ..................................................................
void ADC_init()
{
  //zasilanie
  ADMUX |= (1<<REFS0);
  ADMUX &= ~(1<<REFS1);

  //division factor
  ADCSRA |= (1<<ADPS0);
  ADCSRA |= (1<<ADPS1);
  ADCSRA |= (1<<ADPS2);
  
  //włączenie ADC
  ADCSRA |= (1<<ADEN);
}

int channelA0()
{
  //kanał A0
  ADMUX &= ~(1<<MUX0);
  ADMUX &= ~(1<<MUX1);
  ADMUX &= ~(1<<MUX2);
  ADMUX &= ~(1<<MUX3);
  
  ADCSRA |= (1<<ADSC);

  while(ADCSRA & (1<<ADSC))
  {}

  //zwracanie zmierzonego napięcia
  return ADC;
}

float checkMoisture()
{
  int resultA0 = 0;
  float moisture_percent = 0.0;

  resultA0 = channelA0();
  moisture_percent = (1023.0 - resultA0)/1023.0*100.0;

  return moisture_percent;
}
