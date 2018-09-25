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
  void send_SMS(const char *);

private:
  void _clear_incoming_serial();
  void _display_signal_strength();
  SoftwareSerial* _sim900;
  Display::DisplayManager* _display;
};

}
#endif
