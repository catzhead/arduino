#include "ledpanel.hpp"

#include "LedControl.h"

#define LEDPANEL_DATA 30
#define LEDPANEL_CLK 32
#define LEDPANEL_CS 31

LedControl lc = LedControl(LEDPANEL_DATA, LEDPANEL_CLK, LEDPANEL_CS, 4);

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

const byte led_rpm[11] = {/* t */ B00001111, B00000100, B00000000,
                          /* / */ B00000011, B00001100,
                          /* m */ B00000000, B00001111, B00001000, B00000111, B00001000, B00000111};

void LedPanel::init()
{
  for (int dev = 0; dev < 4; dev++)
  {
    lc.shutdown(dev, false);
    lc.setIntensity(dev, 1);
  }

  // the dot is always at the same place
  lc.setLed(3, 7, 7, true);

  // same for t/m at the end
  for (int i = 0; i < 3; i++)
  {
    lc.setColumn(1, i + 5, led_rpm[i]);
  }
  for (int i = 0; i < 8; i++)
  {
    lc.setColumn(0, i, led_rpm[i+3]);
  }
}

void LedPanel::print_at_pos(unsigned pos, unsigned int value)
{
  if (value > 9) return;
  if (pos > 3) return;

  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(pos, i, led_numbers[value][i]);
  }
}

void LedPanel::print(float value)
{
  unsigned int first_digit = (unsigned int) value;
  unsigned int second_digit = (unsigned int) ((int) (value * 10) % 10);
  unsigned int third_digit = (unsigned int) ((int) (value * 100) % 10);

  // first digit on panel 3
  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(3, i + 1, led_numbers[first_digit][i]);
  }

  // second digit on panel 2
  for (int i = 0; i < 5 ; i++)
  {
    lc.setColumn(2, i + 1, led_numbers[second_digit][i]);
  }

  // third digit, on two different panels (2 and 1)
  lc.setColumn(2, 7, led_numbers[third_digit][0]);
  for (int i = 0; i < 4; i++)
  {
    lc.setColumn(1, i, led_numbers[third_digit][i+1]);
  }
}

