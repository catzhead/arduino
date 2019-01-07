#ifndef __DISPLAY__
#define __DISPLAY__

// #define __TESTS_ENABLED__
// #define __AREARECTS_ENABLED__

#include <ArduinoSTL.h>
#include <string>
#include <list>
#include <vector>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library

typedef struct { float x; float y; } float_coordinates_t;
typedef struct { int x; int y; } coordinates_t;

namespace Display
{
class Menu
{
public:
  Menu(MCUFRIEND_kbv* tft, int x, int y, int w, int h);
  virtual void init();
  virtual void render() = 0;

protected:
  MCUFRIEND_kbv* _tft;
  int _x, _y, _w, _h;
};

class StatusBar : public Menu
{
public:
  StatusBar(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    Menu(tft, x, y, w, h),
    _signal_strength{0}
    {};
  void init();
  void render();
  void set_signal_strength(int value);

private:
  void _display_static();
  void _display_signal_strength();

  int _signal_strength;
};

class TextArea : public Menu
{
public:
  TextArea(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    Menu(tft, x, y, w, h) {};
  void init();
  void render();
  void print(int line_number, std::string& str);
  void print(int line_number, int value, int base);
  void print(int line_number, float value);
};

class ScrollingTextArea : public Menu
{
public:
  ScrollingTextArea(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    Menu(tft, x, y, w, h),
    _need_to_render{true} {};
  void init();
  void render();
  void print(const char* str);
  void print(std::string& str);
  void print(int value, int base);

private:
  std::list<std::string> _lines;
  bool _need_to_render;
};

class GraphArea : public Menu
{
public:

  /*
   * GraphArea parameters:
   * tft: MCUFRIEND_kbv instance to write to screen
   * x, y: top-left point of the area to draw to
   * w, h: width and length relative to (x, y) for the area
   * origin_x, origin_y: origin of the axes in the area, relative to (x, y)
   * max_x, max_y: max value that will be drawable on each axis, the class
   *               will scale everything so that it fits in the area
   */

  GraphArea(MCUFRIEND_kbv* tft,
            int x, int y, int w, int h,
            int origin_x, int origin_y, int max_x, int max_y) :
    Menu(tft, x, y, w, h),
    _origin{x + origin_x, y + origin_y},
    _max_x{max_x},
    _max_y{max_y},
    _scale_x{(float) (w - origin_x) / (float) (max_x)},
    _scale_y{(float) (origin_y) / (float) (max_y)}
    {};

  void init();
  void render();
  void draw_point(float_coordinates_t coords);

protected:
  coordinates_t _origin;
  int _max_x, _max_y;

private:
  void _drawAxes();
  std::list<coordinates_t> _points;
  float _scale_x, _scale_y;
};

class Oscilloscope : public GraphArea
{
public:
  Oscilloscope(MCUFRIEND_kbv* tft,
               int x, int y, int w, int h, int max_y) :
    GraphArea(tft,
               x, y,
               w, h,
               10, h - 10,
               w - 10, max_y),
    _current_x{0} {};

  void plot(float y);

private:
  int _current_x;
};

class DisplayManager
{
public:
  DisplayManager();
  void init();
  void render();

  Oscilloscope* oscillo;
  TextArea* textarea;
  ScrollingTextArea* scrollingtextarea;
  StatusBar* statusbar;

private:
  MCUFRIEND_kbv _tft;
};

}

#endif
