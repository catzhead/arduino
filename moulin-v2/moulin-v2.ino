#include <TaskScheduler.h>

#include "display.hpp"
#include "gsm.hpp"
#include "sensor.hpp"

#define SENSOR_PIN 21

void every_second();
Task task_every_second(1000, TASK_FOREVER, &every_second);
Scheduler scheduler;

Display::DisplayManager* display = nullptr;
GSM::GSMManager* gsm = nullptr;

#define WS_BUFFER_SIZE 10
float wheel_speeds[WS_BUFFER_SIZE];
int wheel_speeds_head;
float wheel_speed_average;
const float wheel_speed_average_factor = 1.0f / (float) (WS_BUFFER_SIZE + 1);

void setup(void)
{
  Serial.begin(9600);

  display = new Display::DisplayManager();
  display->init();
  display->scrollingtextarea->print("GSM?");
  display->render();

  gsm = new GSM::GSMManager(display);
  gsm->init();
  gsm->start();

  display->scrollingtextarea->print("GSM ok");
  display->render();

  attach_sensor(SENSOR_PIN);

  for (int i = 0; i < WS_BUFFER_SIZE; i++)
  {
    wheel_speeds[i] = 0.0f;
  }
  wheel_speeds_head = WS_BUFFER_SIZE - 1;
  wheel_speed_average = 0.0f;

#if 0
  delay(5000);
  gsm->send_SMS("");
#endif

  scheduler.init();
  scheduler.addTask(task_every_second);
  task_every_second.enable();
}

void loop(void)
{
  // test_display();

  scheduler.execute();
}

void every_second()
{
  gsm->display_signal_strength();

  static bool at_least_N_value = false;

  int index = wheel_speeds_head + 1;
  if (index == WS_BUFFER_SIZE)
  {
    index = 0;
  }

  float current_ws = get_wheel_speed();
  if (current_ws > 30.0f)
  {
    current_ws = 30.0f;
  }
  
  wheel_speed_average += (current_ws - wheel_speeds[index]) * wheel_speed_average_factor;
  wheel_speeds[index] = current_ws;
  wheel_speeds_head = index;

  display->oscillo->plot(current_ws);
  display->textarea->print(0, wheel_speed_average);
  Serial.print(current_ws);
  Serial.print(" -- ");
  Serial.println(wheel_speed_average);
  
  display->render();
}

void test_display()
{
  Serial.println(F("TFT LCD test"));

  static int i = 0;
  display->statusbar->set_signal_strength(i);
  if (i < 30) i++;

  static float x = 0.0f;
  display->oscillo->plot(sin(x / (3.14f * 180.0f)) * 10.0f + 10.0f);
  x += 30.0f;
  display->textarea->print(0, x, 10);

  display->scrollingtextarea->print(x, 10);
}
