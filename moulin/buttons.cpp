#include "globals.h"
#include "buttons.h"

int get_button()
{
  static unsigned long last_read_time = 0;
  static int last_read_button = -1;
  int value;

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_read_time) >= DEBOUNCE_DELAY)
  {
    value = analogRead(BUTTONS_PIN);
    last_read_time = current_time;
  }
  else
  {
    return last_read_button;
  }

  int button;
  if (value < 300)
    button = -1;
  else if (value < 600)
    button = 3;
  else if (value < 800)
    button = 2;
  else if (value < 900)
    button = 1;
  else
    button = 0;

  #ifdef DEBUG_BUTTON
  if ((button >= 0) && (last_read_button != button))
  {
    Serial.print("button pressed: ");
    Serial.println(button);
  }
  #endif

  last_read_button = button;

  return button;
}
