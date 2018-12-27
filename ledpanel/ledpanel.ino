#include "LedControl.h"

#define LEDPANEL_DATA 12
#define LEDPANEL_CLK 10
#define LEDPANEL_CS 11

LedControl lc = LedControl(LEDPANEL_DATA, LEDPANEL_CLK, LEDPANEL_CS, 4);

void setup() {
  Serial.begin(9600);

  for (int dev = 0; dev < 4; dev++)
  {
    lc.shutdown(dev, false);
    lc.setIntensity(dev, 1);
  }
}

void loop() {
  static bool state = true;

  for (int dev = 0; dev < 4; dev++)
  {
    for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
      {
        lc.setLed(dev, i, j, state);
        delay(10);
      }
    }
  }
  state = !state;
}
