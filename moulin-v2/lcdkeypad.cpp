#include "Arduino.h"
#include "lcdkeypad.hpp"

#ifdef ARDUINO_UNO
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_D4 = 4;
const int pin_D5 = 5;
const int pin_D6 = 6;
const int pin_D7 = 7;
const int pin_BL = 10;
const int pin_Buttons = 0;
#endif

#ifdef ARDUINO_MEGA
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_D4 = 4;
const int pin_D5 = 5;
const int pin_D6 = 6;
const int pin_D7 = 7;
const int pin_BL = 10;
const int pin_Buttons = 0;
#endif


Display::LCDKeypadManager::LCDKeypadManager():
  _detection_indicator{false}
{
  _lcd = &LiquidCrystal(pin_RS, pin_EN, pin_D4, pin_D5, pin_D6, pin_D7);
  _lcd->begin(16, 2);  // 16 col, 2 lines
  _lcd->setCursor(0, 0);
  _lcd->print("x.xx [x.xx;x.xx]");
}

void Display::LCDKeypadManager::render()
{
  _lcd->setCursor(15,1);
  _lcd->print(_detection_indicator?"*":" ");
}

void Display::LCDKeypadManager::print(char* msg)
{
  _lcd->setCursor(0, 1);
  _lcd->print(msg);
}

void Display::LCDKeypadManager::_print_ws_at(int pos, float ws)
{
  char buffer[8] = "";
  dtostrf(ws, 4, 2, buffer);
  _lcd->setCursor(pos, 0);
  _lcd->print(buffer);
}

void Display::LCDKeypadManager::print_ws(float ws)
{
  _print_ws_at(0, ws);
}

void Display::LCDKeypadManager::print_min_ws(float ws)
{
  _print_ws_at(6, ws);
}

void Display::LCDKeypadManager::print_max_ws(float ws)
{
  _print_ws_at(11, ws);
}

void Display::LCDKeypadManager::print_signal_strength(int value)
{
  _lcd->setCursor(0, 1);
  _lcd->print("sig:   ");
  char buffer[4];
  itoa(value, buffer, 10); // base 10
  _lcd->setCursor(4, 1);
  _lcd->print(buffer);
}

void Display::LCDKeypadManager::change_detection_indicator()
{
  _detection_indicator = !_detection_indicator;
}

void Display::LCDKeypadManager::check_buttons() {
  _lcd->setCursor(14, 1);
  int button = analogRead(pin_Buttons);
  if (button < 60) {
    _lcd->print('R');
  }
  else if (button < 200) {
    _lcd->print('U');
  }
  else if (button < 400) {
    _lcd->print('D');
  }
  else if (button < 600) {
    _lcd->print('L');
  }
  else if (button < 800) {
    _lcd->print('S');
  }
}
