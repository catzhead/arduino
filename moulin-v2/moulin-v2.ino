#include "display.hpp"

Display::DisplayManager* display = nullptr;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  display = new Display::DisplayManager();
  display->init();
}

void loop(void) {
  display->render();
  delay(2000);
}
