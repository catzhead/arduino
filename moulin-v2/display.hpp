#ifndef __DISPLAY__
#define __DISPLAY__

#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>    // Core graphics library

namespace Display
{
#if 0
class StatusBar
{
public:
  void render(int x, int y);
};
#endif

class Display
{
public:
  Display();
  void render();

private:
  MCUFRIEND_kbv _tft;
};

}

#endif