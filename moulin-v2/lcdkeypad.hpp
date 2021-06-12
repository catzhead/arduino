#ifndef __LCDKEYPAD__
#define __LCDKEYPAD__

#include <LiquidCrystal.h>

#include "config.hpp"

namespace Display
{

class LCDKeypadManager
{
  public:
    LCDKeypadManager();
    void init();
    void render();
    void print(char *msg);
    void print_ws(float ws);
    void print_min_ws(float ws);
    void print_max_ws(float ws);
    void print_signal_strength(int value);
    void change_detection_indicator();
    void check_buttons();

  private:
    LiquidCrystal* _lcd;
    bool _detection_indicator;
    void _print_ws_at(int pos, float ws);
};

}

#endif
