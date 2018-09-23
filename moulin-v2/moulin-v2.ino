#include <TimerOne.h>

#include "display.hpp"

Display::DisplayManager* display = nullptr;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  display = new Display::DisplayManager();
  display->init();

  Timer1.initialize(1000000);
  Timer1.attachInterrupt(every_second);
}

void loop(void) {
  //std::string str = "hello";
  //display->scrollingtextarea->print(str);

  static int i = 0;
  display->statusbar->set_signal_strength(i);
  if (i < 30) i++;

  std::string str2 = "line0";
  //display->textarea->print(0, str2);

  static float x = 0.0f;
  display->oscillo->plot(sin(x / (3.14f * 180.0f)) * 10.0f + 10.0f);
  //display->oscillo->plot(10);
  x += 30.0f;
  // display->textarea->print(0, x, 10);

  //display->scrollingtextarea->print(x, 10);


  display->render();
  delay(100);
}

void every_second()
{
  unsigned long current_time = millis() / 1000;
  display->scrollingtextarea->print(current_time, 10);
}