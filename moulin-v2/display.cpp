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
#define GRAY    0xC618

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

Display::DisplayManager::DisplayManager()
{
  Display::Menu* statusbar = new Display::StatusBar(&_tft,
                                                    0, 0,
                                                    SCREEN_WIDTH - 1, 29);
  _menus.push_back(statusbar);

  _tft.reset();

  uint16_t identifier = _tft.readID();
  _tft.begin(identifier);

  // Set to landscape mode
  _tft.setRotation(1);
  _tft.fillScreen(BLACK);
}

void Display::DisplayManager::init()
{
  for (auto menu : _menus)
  {
    menu->init();
  }
}

void Display::DisplayManager::render()
{
  for (auto menu : _menus)
  {
    menu->render();
  }

#if 0
  static long previous_time = 0;

  _tft.setCursor(0, 30);
  _tft.setTextColor(BLACK);
  _tft.println(previous_time);

  unsigned long current_time = micros();
  _tft.setCursor(0, 30);
  _tft.setTextColor(GREEN);
  _tft.println(current_time);
  previous_time = current_time;
#endif
}

/*
 * StatusBar
 */
void Display::StatusBar::init()
{
  _display_static();
}

void Display::StatusBar::render()
{
  _display_signal_strength();
}

void Display::StatusBar::_display_static()
{
  // _tft->drawRect(_x, _y, _w, _h, GREEN);
  _tft->setCursor(_x + _w / 2 - 30, _y + _h / 2 - 5);
  _tft->setTextColor(WHITE);
  _tft->setTextSize(2);
  _tft->println("ASAMEC");
}

void Display::StatusBar::_display_signal_strength()
{
  const int nb_lines = 3;
  const int space_between_lines = 2;
  const int length_increase = 2;

  int length = 3;
  int x0 = _x + 10;
  int y0 = _y + _h - length - length_increase * 4;
  int x_offset = 0;
  int y_offset = 0;
  int count = 0;
  int color = GRAY;

  while (count < 5)
  {
    if (count <= _signal_strength)
    {
      color = WHITE;
    }
    else
    {
      color = GRAY;
    }

    for (int i = 0; i < nb_lines; i++)
    {
      _tft->drawFastVLine(x0  + x_offset + i,
                         y0 + y_offset,
                         length,
                         color);
    }

    x_offset += nb_lines + space_between_lines;
    y_offset -= length_increase;
    length += length_increase;
    count++;
  }

  _signal_strength++;
  if (_signal_strength == 5) {_signal_strength = 0;};
}