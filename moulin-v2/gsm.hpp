#ifndef __GSM__
#define __GSM__

#include <SoftwareSerial.h>

#include "display.hpp"

namespace GSM
{

class GSMManager
{
public:
  GSMManager(Display::DisplayManager* display);
  void init();
  void start();
  void stop();
  void display_signal_strength();
  void send_SMS(const char *);

private:
  bool _is_GSM_board_powered();
  void _clear_incoming_serial();
  void _get_incoming_answer(std::string* buffer);
  SoftwareSerial* _sim900;
  Display::DisplayManager* _display;
  bool _is_powered;
};

}
#endif
