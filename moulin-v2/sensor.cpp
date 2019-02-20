#include "Arduino.h"
#include "sensor.hpp"

#define BUFFER_SIZE 10
#define NB_TEETH 120
#define WHEEL_RATIO 1.0f
#define DEBOUNCE_TIME 50

unsigned long buffer[BUFFER_SIZE];
int head;
float instant_average;
const float average_factor = 1.0f / (float) (BUFFER_SIZE + 1);

/* When detecting each tooth, we need to divide by the number of NB_TEETH
 */
// const float wheel_factor = 60000.0f / ((float) NB_TEETH * WHEEL_RATIO);

/* When detecting only one tooth every turn, the ratio is directly the ratio
 * between the wheels
 */
const float wheel_factor = 60000.0f / WHEEL_RATIO;

void attach_sensor(int pin)
{
  head = BUFFER_SIZE - 1;
  instant_average = 0.0f;
  for (int i = 0; i < BUFFER_SIZE; i++)
  {
    buffer[i] = 0;
  }

  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), detect, FALLING);
}

void detect()
{
  static unsigned long previous_time = 0;

  int index = head + 1;
  if (index == BUFFER_SIZE)
  {
    index = 0;
  }

  unsigned long current_time = millis();
  unsigned long elapsed_time = current_time - previous_time;

  if (elapsed_time < DEBOUNCE_TIME)
  {
    // debounce
    return;
  }

  float temp = (float) elapsed_time * average_factor - (float) buffer[index] * average_factor;
  instant_average += temp;

  buffer[index] = elapsed_time;
  head = index;
  previous_time = current_time;
}

float get_wheel_speed()
{
  return wheel_factor / instant_average;
}

