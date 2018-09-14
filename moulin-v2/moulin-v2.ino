#include "display.hpp"

Display::DisplayManager* display = nullptr;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  display = new Display::DisplayManager();
  display->init();
}

void loop(void) {
  std::string str = "hello";
  display->scrollingtextarea->print(str);

  static int i = 0;
  display->statusbar->set_signal_strength(i);
  if (i < 30) i++;

  std::string str2 = "line0";
  display->textarea->print(0, str2);

  display->render();
  delay(1000);
}
