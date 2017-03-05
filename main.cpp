#include <Arduino.h>

const uint8_t hc595_data  = 6; /* SER */
const uint8_t hc595_latch = 7; /* RCLK */
const uint8_t hc595_clock = 8; /* SRCLK */

const uint8_t hc138_a = 5;
const uint8_t hc138_b = 4;
const uint8_t hc138_c = 3;
const uint8_t hc138_enable = 9;

const uint8_t hc165_data = A5; /* QH */
const uint8_t hc165_latch = A4; /* SH */
const uint8_t hc165_clock = A3; /* CLK */

uint8_t hc595_out = 0;
uint8_t hc138_out = 0;

void setup()
{
  pinMode(hc595_data, OUTPUT);
  pinMode(hc595_latch, OUTPUT);
  pinMode(hc595_clock, OUTPUT);

  pinMode(hc138_a, OUTPUT);
  pinMode(hc138_b, OUTPUT);
  pinMode(hc138_c, OUTPUT);
  pinMode(hc138_enable, OUTPUT);

  pinMode(hc165_data, INPUT);
  pinMode(hc165_clock, OUTPUT);
  pinMode(hc165_latch, OUTPUT);
}

/*
 * Here we define an alternate shiftIn function. The only difference 
 * between this shift in function and the built-in shiftIn function 
 * is that the clock must got LOW before shifting and HIGH after 
 * shifting.  This is the opposite of the built in Arduino shiftIn.
 *
 * https://github.com/SAIC-ATS/Tutorials/tree/master/Arduino/ICs/74HC165/Examples/ExampleBasic
 */
uint8_t shiftIn165(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i)
  {
    digitalWrite(clockPin, LOW);
    if (bitOrder == LSBFIRST)
      value |= digitalRead(dataPin) << i;
    else
      value |= digitalRead(dataPin) << (7 - i);
    digitalWrite(clockPin, HIGH);
  }

  return value;
}

void loop()
{
  digitalWrite(hc165_latch, LOW);
  delayMicroseconds(5);
  digitalWrite(hc165_latch, HIGH);
  delayMicroseconds(5);

  hc595_out = shiftIn165(hc165_data, hc165_clock, MSBFIRST);

  digitalWrite(hc595_latch, LOW);
  shiftOut(hc595_data, hc595_clock, MSBFIRST, hc595_out);
  digitalWrite(hc595_latch, HIGH);

  digitalWrite(hc138_enable, LOW);
  digitalWrite(hc138_a, hc138_out & (1 << 0));
  digitalWrite(hc138_b, hc138_out & (1 << 1));
  digitalWrite(hc138_c, hc138_out & (1 << 2));
  digitalWrite(hc138_enable, HIGH);
  hc138_out = (hc138_out + 1) & B00000111;

  delay(100);
}
