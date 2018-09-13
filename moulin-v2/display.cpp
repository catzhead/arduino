#include "display.hpp"

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320

Display::DisplayManager::DisplayManager()
{
  Display::Menu* statusbar = new Display::StatusBar(&_tft,
                                   0, 0,
                                   SCREEN_WIDTH, 30);
  _menus.push_back(statusbar);

  Display::TextArea* textarea = new Display::TextArea(&_tft,
                                      SCREEN_WIDTH / 2,
                                      SCREEN_HEIGHT / 2 + 30,
                                      SCREEN_WIDTH / 2,
                                      SCREEN_HEIGHT / 2 - 30);
  _menus.push_back(textarea);

  Display::GraphArea* grapharea = new Display::GraphArea(&_tft,
                                        0,
                                        30,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT / 2);
  _menus.push_back(grapharea);

  _tft.reset();

  uint16_t identifier = _tft.readID();
  _tft.begin(identifier);

  // Set to landscape mode
  _tft.setRotation(1);
  _tft.fillScreen(TFT_BLACK);
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
}

/*
 * Menu
 */

Display::Menu::Menu(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    _tft{tft},
    _x{x},
    _y{y},
    _w{w},
    _h{h}
{
}

void Display::Menu::init()
{
#ifdef __AREARECTS_ENABLED__
  _tft->drawRect(_x, _y, _w, _h, TFT_GREEN);
#endif
}

/*
 * StatusBar
 */

#define MAX_CSQ_VALUE 30

void Display::StatusBar::init()
{
  Display::Menu::init();
  _display_static();
}

void Display::StatusBar::render()
{
  _display_signal_strength();
}

void Display::StatusBar::_display_static()
{
  _tft->setCursor(_x + _w / 2 - 30, _y + _h / 2 - 5);
  _tft->setTextColor(TFT_WHITE);
  _tft->setTextSize(2);
  _tft->println("ASAMEC");

  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(_x + 40, _y + 9);
  _tft->print("  ");
  _tft->print("/");
  _tft->print(MAX_CSQ_VALUE);
}

void Display::StatusBar::_display_signal_strength()
{
  const int nb_lines = 3;
  const int space_between_lines = 2;
  const int length_increase = 2;
  const int nb_bars = 5;

  int length = 3;
  int x0 = _x + 10;
  int y0 = _y + _h - length - length_increase * 4;
  int x_offset = 0;
  int y_offset = 0;
  int count = 0;
  int color = TFT_WHITE;
  int nb_bars_to_light = (_signal_strength) / (nb_bars + 1);

  while (count < nb_bars)
  {
    if (_signal_strength == 0 || count > nb_bars_to_light)
    {
      color = TFT_DARKGREY;
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

  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(_x + 40, _y + 9);
  _tft->print("  ");
  _tft->setCursor(_x + 40, _y + 9);
  if (_signal_strength < 10)
    _tft->print("0");
  _tft->print(_signal_strength);

#ifdef __TESTS_ENABLED__
  _signal_strength++;
  if (_signal_strength > MAX_CSQ_VALUE)
    _signal_strength = 0;
#endif
}

void Display::StatusBar::set_signal_strength(int value)
{
  if (value < 0)
    _signal_strength = 0;
  else if (value > MAX_CSQ_VALUE)
    _signal_strength = MAX_CSQ_VALUE;
  else
    _signal_strength = value;
}

/*
 * TextArea
 */

void Display::TextArea::init()
{
  Display::Menu::init();

#ifdef __TESTS_ENABLED__
  char conversion[10];
  for (int i=0; i<10; i++)
  {
    std::string temp = "yo[";
    itoa(i, conversion, 10);
    temp += conversion;
    temp += "]";
    print(temp);
  }
#endif
}

void Display::TextArea::render()
{
  if (!_need_to_render)
    return;

  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  const int line_height = 16; // text size 2
  const int lines_limit = _h / line_height;
  int count = 0;

  for (auto line : _lines)
  {
    _tft->setCursor(_x + 1, _y + 1 + count * line_height);
    _tft->print("     ");
    _tft->setCursor(_x + 1, _y + 1 + count * line_height);
    _tft->print(line.c_str());
    count++;

    if (count == lines_limit)
      break;
  }

  if (_lines.size() > lines_limit)
  {
    _lines.pop_front();
    _need_to_render = true;
  }
  else
  {
    _need_to_render = false;
  }
}

void Display::TextArea::print(int value, int base)
{
  char buffer[20];
  std::string str = itoa(value, buffer, base);
  _lines.push_back(str);
  _need_to_render = true;
}

void Display::TextArea::print(std::string& str)
{
  _lines.push_back(str);
}

/*
 * GraphArea
 */

void Display::GraphArea::init()
{
  Display::Menu::init();

  _tft->drawLine(_x, _y, _x + _w - 1, _y + _h - 1, TFT_RED);
  _tft->drawLine(_x + _w - 1, _y, _x, _y + _h - 1, TFT_RED);
}

void Display::GraphArea::render()
{
}