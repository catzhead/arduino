#include <TaskScheduler.h>

#include "display.hpp"
#include "gsm.hpp"

void every_second();
Task task_every_second(1000, TASK_FOREVER, &every_second);
Scheduler scheduler;

Display::DisplayManager* display = nullptr;
GSM::GSMManager* gsm = nullptr;

void setup(void)
{
  Serial.begin(9600);

  display = new Display::DisplayManager();
  display->init();

  gsm = new GSM::GSMManager(display);
  gsm->init();
  gsm->start();

  delay(5000);
  gsm->send_SMS("");

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
