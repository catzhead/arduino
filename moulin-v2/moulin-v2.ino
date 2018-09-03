#include "display.hpp"

Display::Display* display = nullptr;

void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

  display = new Display::Display();
}

void loop(void) {
  display->render();
  delay(2000);
}
