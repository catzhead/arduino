#ifndef __DISPLAY__
#define __DISPLAY__

// #define __TESTS_ENABLED__
// #define __AREARECTS_ENABLED__

#include <StandardCplusplus.h>
#include <string>
#include <list>
#include <vector>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library

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
};

class ScrollingTextArea : public Menu
{
public:
  ScrollingTextArea(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    Menu(tft, x, y, w, h),
    _need_to_render{true} {};
  void init();
  void render();
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
    _scale_x{(float) (max_x) / (float) (w - origin_x)},
    _scale_y{(float) (max_y) / (float) (h - origin_y)}
    {};

  void init();
  void render();
  void drawPoint(coordinates_t coords);

private:
  void _drawAxes();
  std::list<coordinates_t> _points;
  coordinates_t _origin;
  int _max_x, _max_y;
  float _scale_x, _scale_y;
};

class DisplayManager
{
public:
  DisplayManager();
  void init();
  void render();

  GraphArea* grapharea;
  TextArea* textarea;
  ScrollingTextArea* scrollingtextarea;
  StatusBar* statusbar;

private:
  MCUFRIEND_kbv _tft;
};

}

#endif