#include "Arduino.h"
#include "lcdkeypad.hpp"
#include "sensor.hpp"

#define BUFFER_SIZE 3
#define NB_TEETH 4
#define WHEEL_RATIO 1.0f
#define DEBOUNCE_TIME 2000

extern Display::LCDKeypadManager* lcdkeypad;

unsigned long buffer[BUFFER_SIZE];
int head;
float instant_average;
unsigned long previous_time;
const float average_factor = 1.0f / (float) BUFFER_SIZE;
char detection;

/* When detecting each tooth, we need to divide by the number of NB_TEETH
 */
const float wheel_factor = 60000.0f / ((float) NB_TEETH * WHEEL_RATIO);

/* When detecting only one tooth every turn, the ratio is directly the ratio
 * between the wheels
 */
// const float wheel_factor = 60000.0f / WHEEL_RATIO;

void attach_sensor(int pin)
{
  head = BUFFER_SIZE - 1;
  instant_average = 0.0f;
  previous_time = millis();
  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }

  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), detect, FALLING);
  detection = 0;
}

unsigned long time_since_last_detection()
{
  unsigned long current_time = millis();
  return current_time - previous_time;
}

void detect()
{
  unsigned long current_time = millis();
  unsigned long elapsed_time = current_time - previous_time;

  if (elapsed_time < DEBOUNCE_TIME)
  {
    // debounce
    return;
  }

  int index = head + 1;
  if (index == BUFFER_SIZE)
  {
    index = 0;
  }

  float temp = (float) elapsed_time * average_factor - (float) buffer[index] * average_factor;
  instant_average += temp;

  buffer[index] = elapsed_time;
  head = index;
  previous_time = current_time;

  if (lcdkeypad != nullptr)
    lcdkeypad->change_detection_indicator();

  detection = 1;
}

float get_wheel_speed()
{
  return wheel_factor / instant_average;
}
