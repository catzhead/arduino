#include <TimerOne.h>

#include "display.hpp"
#include "gsm.hpp"

Display::DisplayManager* display = nullptr;
GSM::GSMManager* gsm = nullptr;

void setup(void)
{
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  display = new Display::DisplayManager();
  display->init();

  gsm = new GSM::GSMManager(display);

  Timer1.initialize(10000000);
  Timer1.attachInterrupt(every);
}

void loop(void)
{
  // test_display();

  display->render();
  delay(100);
}

void every()
{
  unsigned long current_time = millis() / 1000;
  display->scrollingtextarea->print(current_time, 10);
}

void test_display()
{
  static int i = 0;
  display->statusbar->set_signal_strength(i);
  if (i < 30) i++;

  static float x = 0.0f;
  display->oscillo->plot(sin(x / (3.14f * 180.0f)) * 10.0f + 10.0f);
  x += 30.0f;
  display->textarea->print(0, x, 10);

  display->scrollingtextarea->print(x, 10);
}