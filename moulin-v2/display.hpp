#ifndef __DISPLAY__
#define __DISPLAY__

#include <ArduinoSTL.h>

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library

namespace Display
{
class Menu
{
public:
  Menu(MCUFRIEND_kbv* tft, int x, int y, int w, int h) :
    _tft{tft},
    _x{x},
    _y{y},
    _w{w},
    _h{h}
    {};
  virtual void init() = 0;
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

private:
  void _display_static();
  void _display_signal_strength();

  int _signal_strength;
};

class DisplayManager
{
public:
  DisplayManager();
  void init();
  void render();

private:
  MCUFRIEND_kbv _tft;
  std::vector<Menu*> _menus;
};

}

#endif