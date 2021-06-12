#ifndef __GSM__
#define __GSM__

#include "config.hpp"
#include <SoftwareSerial.h>
#include "ArduinoSTL.h"
#include <string>

#include "lcdkeypad.hpp"

namespace GSM
{

class GSMManager
{
public:
  GSMManager(Display::LCDKeypadManager* lcd);
  void init();
  void start();
  void stop();
  void display_signal_strength();
  void send_SMS(const char* number, const char* msg);

  bool is_powered;

private:
  bool _is_GSM_board_powered();
  void _clear_incoming_serial();
  void _get_incoming_answer(std::string* buffer);
  SoftwareSerial* _sim900;
  Display::LCDKeypadManager* _lcd;
};

}
#endif
