#include <TaskScheduler.h>

#include "display.hpp"
#include "gsm.hpp"
#include "sensor.hpp"
#include "ledpanel.hpp"

#define SENSOR_PIN 21

#define RESET_MIN_MAX_VALUE 200

#define TIMEOUT_NO_DETECTION 600000 // timeout in ms

void every_second();
void send_message();
Task task_every_second(1000, TASK_FOREVER, &every_second);
// It seems like it shifts by 11 minutes each day, trying to compensate
Task task_message(24 * 60 * 60000 - 11 * 60000, TASK_FOREVER, &send_message);
Scheduler scheduler;

Display::DisplayManager* display = nullptr;
GSM::GSMManager* gsm = nullptr;

#define WS_BUFFER_SIZE 2
float wheel_speeds[WS_BUFFER_SIZE];
int wheel_speeds_head;
float wheel_speed_average, wheel_speed_average_min, wheel_speed_average_max;
const float wheel_speed_average_factor = 1.0f / (float) (WS_BUFFER_SIZE + 1);
int reset_min_max;
float current_ws;

void init_ws()
{
  for (int i = 0; i < WS_BUFFER_SIZE; i++)
  {
    wheel_speeds[i] = 0.0f;
  }
  wheel_speeds_head = WS_BUFFER_SIZE - 1;
  wheel_speed_average = 0.0f;
  reset_min_max = RESET_MIN_MAX_VALUE;
  wheel_speed_average_min = 10.0f;
  wheel_speed_average_max = 0.0f;
}

void setup(void)
{
  Serial.begin(9600);

  LedPanel::init();

  display = new Display::DisplayManager();
  display->init();
  display->scrollingtextarea->print("GSM?");
  display->render();

  gsm = new GSM::GSMManager(display);
  gsm->init();
  gsm->start();

  std::string gsm_status_str = "GSM nok";
  if (gsm->is_powered)
  {
    gsm_status_str = "GSM ok";
  }
  display->scrollingtextarea->print(gsm_status_str);
  display->render();

  attach_sensor(SENSOR_PIN);

  init_ws();

  scheduler.init();
  scheduler.addTask(task_every_second);
  task_every_second.enable();
  scheduler.addTask(task_message);
  task_message.enable();
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
  static bool message_sent_ws_zero = false;

  if (time_since_last_detection() > TIMEOUT_NO_DETECTION)
  {
    init_ws();
    current_ws = 0.0f;
    wheel_speed_average = 0.0f;
    if (!message_sent_ws_zero)
    {
      send_message();
      message_sent_ws_zero = true;
    }
  }
  else
  {
    message_sent_ws_zero = false;
    
    int index = wheel_speeds_head + 1;
    if (index == WS_BUFFER_SIZE)
    {
      index = 0;
    }
  
    current_ws = get_wheel_speed();
    if (current_ws > 9.9f)
    {
      current_ws = 9.9f;
    }

    wheel_speed_average += (current_ws - wheel_speeds[index]) * wheel_speed_average_factor;
    wheel_speeds[index] = current_ws;
    wheel_speeds_head = index;
  }

  display->oscillo->plot(current_ws);
  display->textarea->print(0, current_ws);

  Serial.print("min: ");
  Serial.println(wheel_speed_average_min);

  if (current_ws < wheel_speed_average_min)
  {
    wheel_speed_average_min = current_ws;
    std::string prefix = "min: ";
    display->textarea->print(1, prefix, wheel_speed_average_min);
  }

  if (current_ws > wheel_speed_average_max)
  {
    wheel_speed_average_max = current_ws;
    std::string prefix = "max: ";
    display->textarea->print(2, prefix, wheel_speed_average_max);
  }

  LedPanel::print(current_ws);

  display->render();
}

void send_message()
{
  Serial.println("sending SMS");
  char buffer[8] = "";
  dtostrf(current_ws, 4, 2, buffer);
  std::string str = "Vitesse de la roue: ";
  str += buffer;
  dtostrf(wheel_speed_average_min, 4, 2, buffer);
  str += " (min: ";
  str += buffer;
  str += " - max: ";
  dtostrf(wheel_speed_average_max, 4, 2, buffer);
  str += buffer;
  str += ")";

  gsm->send_SMS("0633418741", str.c_str());
//  gsm->send_SMS("0630291418", str.c_str());

  reset_wheel_speed_average_min_max();
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

void reset_wheel_speed_average_min_max()
{
  reset_min_max--;
  if (reset_min_max == 0)
  {
    reset_min_max = RESET_MIN_MAX_VALUE;
    wheel_speed_average_min = 10.0f;
    wheel_speed_average_max = 0.0f;
  }
}
