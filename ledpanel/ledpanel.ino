#include "LedControl.h"

#define LEDPANEL_DATA 30
#define LEDPANEL_CLK 32
#define LEDPANEL_CS 31

LedControl lc = LedControl(LEDPANEL_DATA, LEDPANEL_CLK, LEDPANEL_CS, 4);

#if 0
// numbers defined as rows
const byte zero[8]  = {B00011100, B00100010, B00100010, B00100010, B00100010, B00100010, B00100010, B00011100};
const byte one[8]   = {B00001000, B00011000, B00101000, B00001000, B00001000, B00001000, B00001000, B00111110};
const byte two[8]   = {B00011100, B00100010, B00000010, B00000100, B00001000, B00010000, B00100000, B00111110};
const byte three[8] = {B00011100, B00100010, B00000010, B00011100, B00000100, B00000010, B00100010, B00011100};
const byte four[8]  = {B00000100, B00001100, B00010100, B00100100, B00111110, B00000100, B00000100, B00000100};
const byte five[8]  = {B00111110, B00100000, B00100000, B00111100, B00000010, B00000010, B00100010, B00011100};
const byte six[8]   = {B00011100, B00100000, B00100000, B00111100, B00100010, B00100010, B00100010, B00011100};
const byte seven[8] = {B00111110, B00000010, B00000010, B00000100, B00000100, B00001000, B00001000, B00001000};
const byte eight[8] = {B00011100, B00100010, B00100010, B00011100, B00100010, B00100010, B00100010, B00011100};
const byte nine[8]  = {B00011100, B00100010, B00100010, B00011110, B00000010, B00000010, B00100010, B00011100};
#endif

const byte led_numbers[10][5] =
{ /* 0 */ {B01111110, B10000001, B10000001, B10000001, B01111110},
  /* 1 */ {B00100001, B01000001, B11111111, B00000001, B00000001},
  /* 2 */ {B01000011, B10000101, B10001001, B10010001, B01100001},
  /* 3 */ {B01000010, B10010001, B10010001, B10011001, B01100110},
  /* 4 */ {B00011000, B00101000, B01001000, B11111111, B00001000},
  /* 5 */ {B11110010, B10010001, B10010001, B10010001, B10001110},
  /* 6 */ {B01111110, B10010001, B10010001, B10010001, B01001110},
  /* 7 */ {B10000000, B10000000, B10000111, B10011000, B11100000},
  /* 8 */ {B01101110, B10010001, B10010001, B10010001, B01101110},
  /* 9 */ {B01100010, B10010001, B10010001, B10010001, B01111110}
};

#if 0
{ {B00011100,
   B00100010,
   B00100010,
   B00100010,
   B00100010,
   B00100010,
   B00100010,
   B00011100},

  {B00001000,
   B00011000,
   B00101000,
   B00001000,
   B00001000,
   B00001000,
   B00001000,
   B00111110},

  {B00011100,
   B00100010,
   B00000010,
   B00000100,
   B00001000,
   B00010000,
   B00100000,
   B00111110},

  {B00011100,
   B00100010,
   B00000010,
   B00011100,
   B00000100,
   B00000010,
   B00100010,
   B00011100},

  {B00000100,
   B00001100,
   B00010100,
   B00100100,
   B00111110,
   B00000100,
   B00000100,
   B00000100},

  {B00111110,
   B00100000,
   B00100000,
   B00111100,
   B00000010,
   B00000010,
   B00100010,
   B00011100},

  {B00011100,
   B00100000,
   B00100000,
   B00111100,
   B00100010,
   B00100010,
   B00100010,
   B00011100},

  {B00111110,
   B00000010,
   B00000010,
   B00000100,
   B00000100,
   B00001000,
   B00001000,
   B00001000},

  {B00011100,
   B00100010,
   B00100010,
   B00011100,
   B00100010,
   B00100010,
   B00100010,
   B00011100},


  {B00011100,
   B00100010,
   B00100010,
   B00011110,
   B00000010,
   B00000010,
   B00100010,
   B00011100}
};
#endif

const byte led_rpm[11] = {/* t */ B00001111, B00000100, B00000000,
                          /* / */ B00000011, B00001100,
                          /* m */ B00000000, B00001111, B00001000, B00000111, B00001000, B00000111};

void setup() {
  Serial.begin(9600);

  for (int dev = 0; dev < 4; dev++)
  {
    lc.shutdown(dev, false);
    lc.setIntensity(dev, 1);
  }

  lc.setLed(3, 7, 7, true);

  for (int i = 0; i < 3; i++)
  {
    lc.setColumn(1, i + 5, led_rpm[i]);
  }
  for (int i = 0; i < 8; i++)
  {
    lc.setColumn(0, i, led_rpm[i+3]);
  }
}

void loop() {
#if 0
  static bool state = true;

  for (int dev = 0; dev < 4; dev++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        lc.setLed(dev, i, j, state);
        delay(100);
      }
    }
  }
  state = !state;
#endif

  static float value = 0.00f;
  led_print_float(value);

  value += 0.01f;

  if (value > 9.99f) value = 0.0f;
  //delay(100);

}

void led_print_number(unsigned pos, unsigned int value)
{
  if (value > 9) return;
  if (pos > 3) return;

  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(pos, i, led_numbers[value][i]);
  }
}

void led_print_float(float value)
{
  unsigned int first_digit = (unsigned int) value;
  unsigned int second_digit = (unsigned int) ((int) (value * 10) % 10);
  unsigned int third_digit = (unsigned int) ((int) (value * 100) % 10);

  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(3, i + 1, led_numbers[first_digit][i]);
  }

  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(2, i + 1, led_numbers[second_digit][i]);
  }

  lc.setColumn(2, 7, led_numbers[third_digit][0]);
  for (int i = 0; i < 4; i++)
  {
    lc.setColumn(1, i, led_numbers[third_digit][i+1]);
  }
}

