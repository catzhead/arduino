#ifndef __GLOBALS__
#define __GLOBALS__

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "config.h"

extern LiquidCrystal lcd;
extern int current_menu;
extern int new_menu;
extern int count;
extern float rpm, rpm_min, rpm_max;
extern bool alarm_enabled;
extern bool detection;
extern int detection_min, detection_max;
extern float alarm_min_threshold;
extern int alarm_min_duration;
extern bool send_alarm_request;

#endif
