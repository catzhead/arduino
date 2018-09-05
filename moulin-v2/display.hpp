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
  Menu(int x, int y, int w, int h) :
    _x{x},
    _y{y},
    _w{w},
    _h{h}
    {};
  virtual void render(MCUFRIEND_kbv* tft) = 0;

protected:
  int _x, _y, _w, _h;
};

class StatusBar : public Menu
{
public:
  StatusBar(int x, int y, int w, int h) :
    Menu(x, y, w, h),
    _signal_strength{0}
    {};
  void render(MCUFRIEND_kbv* tft);

private:
  int _signal_strength;
};

class DisplayManager
{
public:
  DisplayManager();
  void render();

private:
  MCUFRIEND_kbv _tft;
  std::vector<Menu*> _menus;
};

}

#endif