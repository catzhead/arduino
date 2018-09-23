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
  statusbar = new Display::StatusBar(&_tft,
                                     0, 0,
                                     SCREEN_WIDTH, 30);

  textarea = new Display::TextArea(&_tft,
                                   0,
                                   SCREEN_HEIGHT / 2 + 30,
                                   SCREEN_WIDTH / 2,
                                   SCREEN_HEIGHT / 2 - 30);

  scrollingtextarea = new Display::ScrollingTextArea(&_tft,
                                     SCREEN_WIDTH / 2,
                                     SCREEN_HEIGHT / 2 + 30,
                                     SCREEN_WIDTH / 2,
                                     SCREEN_HEIGHT / 2 - 30);

  oscillo = new Display::Oscilloscope(&_tft,
                                      0,
                                      30,
                                      SCREEN_WIDTH,
                                      SCREEN_HEIGHT / 2,
                                      20);

  _tft.reset();

  uint16_t identifier = _tft.readID();
  _tft.begin(identifier);

  // Set to landscape mode
  _tft.setRotation(1);

  _tft.fillScreen(TFT_BLACK);
}

void Display::DisplayManager::init()
{
  statusbar->init();
  oscillo->init();
  scrollingtextarea->init();
  textarea->init();
}

void Display::DisplayManager::render()
{
  statusbar->render();
  oscillo->render();
  scrollingtextarea->render();
  textarea->render();
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
  static int last_signal_strength = -1;

  if (_signal_strength == last_signal_strength)
    return;

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

  last_signal_strength = _signal_strength;
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
}

void Display::TextArea::render()
{
}

void Display::TextArea::print(int line_number, std::string& str)
{
  const int line_height = 16; // text size 2
  const int lines_limit = _h / line_height;

  if (line_number >= lines_limit)
    return;

  _tft->setTextColor(TFT_WHITE, TFT_BLACK);
  _tft->setTextSize(2);
  _tft->setCursor(_x + 1, _y + 1 + line_number * line_height);
  _tft->print("        "); // TODO improve cleaning the old line
  _tft->setCursor(_x + 1, _y + 1 + line_number * line_height);
  _tft->print(str.c_str());
}

void Display::TextArea::print(int line_number, int value, int base)
{
  char buffer[20];
  std::string str = itoa(value, buffer, base);
  print(line_number, str);
}

/*
 * ScrollingTextArea
 */

void Display::ScrollingTextArea::init()
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

void Display::ScrollingTextArea::render()
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

void Display::ScrollingTextArea::print(int value, int base)
{
  char buffer[20];
  std::string str = itoa(value, buffer, base);
  _lines.push_back(str);
  _need_to_render = true;
}

void Display::ScrollingTextArea::print(std::string& str)
{
  _lines.push_back(str);
}

/*
 * GraphArea
 */

void Display::GraphArea::init()
{
  Display::Menu::init();

  _drawAxes();

#ifdef __TESTS_ENABLED__
  float y = 0;
  for (float x = 0; x < _w - 1; x += 0.1f)
  {
    y = sin(10 * x / 180) * (_h - 1) / 2;
    _tft->drawPixel(x, y + _y + _h / 2, TFT_RED);
  }
#endif
}

void Display::GraphArea::render()
{
  for (auto point : _points)
  {
    _tft->drawPixel(point.x , point.y, TFT_GREEN);
    _points.pop_front();
  }
}

void Display::GraphArea::draw_point(float_coordinates_t coords)
{
  coordinates_t corrected;

  corrected.x = _origin.x + coords.x * _scale_x;
  corrected.y = _origin.y - coords.y * _scale_y;

  _points.push_back(corrected);
}

void Display::GraphArea::_drawAxes()
{
  _tft->drawFastHLine(_x, _origin.y, _w, TFT_WHITE);
  _tft->drawFastVLine(_origin.x, _y, _h, TFT_WHITE);
}

/*
 * Oscilloscope
 */

void Display::Oscilloscope::plot(float y)
{
  if (_current_x == _max_x)
  {
    _current_x = 1;
  }

  float_coordinates_t coords = {(float) _current_x, y};
  static int filling_height = _origin.y - _y;
  if (_current_x == 1)
  {
    _tft->fillRect(_origin.x + _current_x, _y, 10, filling_height, TFT_BLACK);
  }
  else
  {
    _tft->drawFastVLine(_origin.x + _current_x + 9, _y, filling_height, TFT_BLACK);
  }
  draw_point(coords);
  _current_x++;
}