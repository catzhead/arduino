#include "display.hpp"

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Display::Display::Display()
{
  _tft.reset();

  uint16_t identifier = _tft.readID();
  _tft.begin(identifier);

  // Set to landscape mode
  _tft.setRotation(1);
  _tft.fillScreen(BLACK);
}

void Display::Display::render()
{
  _tft.setCursor(0, 0);
  _tft.setTextColor(GREEN);
  _tft.setTextSize(2);
  _tft.println("yo");

  static long previous_time = 0;

  _tft.setCursor(0, 30);
  _tft.setTextColor(BLACK);
  _tft.println(previous_time);

  unsigned long current_time = micros();
  _tft.setCursor(0, 30);
  _tft.setTextColor(GREEN);
  _tft.println(current_time);
  previous_time = current_time;
}