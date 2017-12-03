#include "globals.h"
#include "buttons.h"

int get_button()
{
  static unsigned long last_read_time = 0;
  int value;

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_read_time) >= DEBOUNCE_DELAY)  
  {
    value = analogRead(BUTTONS_PIN);
    last_read_time = current_time;
  }
  else
  {
    return -1; 
  }
    
  int button;
  if (value < 300)
    button = -1; 
  else if (value < 600)
    button = 0;
  else if (value < 800)
    button = 1;
  else if (value < 900)
    button = 2;
  else
    button = 3; 

  #ifdef DEBUG_BUTTON
  {
    static int old_button = -2;
    if ((button > 0) && (old_button != button))
    {
      Serial.print("button pressed: ");
      Serial.println(button);
    }
    old_button = button;
  }
  #endif

  return button;
}
