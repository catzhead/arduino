#include "globals.h"
#include "storage.h"
#include <EEPROM.h>

#define STORAGE_MIN_ADDR 0
#define STORAGE_MAX_ADDR 4
#define STORAGE_ALARM_ENABLED_ADDR 8 
#define STORAGE_DETECTION_MIN_ADDR 12
#define STORAGE_DETECTION_MAX_ADDR 16
#define STORAGE_ALARM_MIN_THRESHOLD_ADDR 20
#define STORAGE_ALARM_MIN_DURATION_ADDR 24

void init_eeprom()
{
  EEPROM.put(STORAGE_MIN_ADDR, 9.0f);
  EEPROM.put(STORAGE_MAX_ADDR, 0.0f);
  EEPROM.put(STORAGE_ALARM_ENABLED_ADDR, true);
  EEPROM.put(STORAGE_DETECTION_MIN_ADDR, 50);
  EEPROM.put(STORAGE_DETECTION_MAX_ADDR, 150);
  EEPROM.put(STORAGE_ALARM_MIN_THRESHOLD_ADDR, 0.3);  
  EEPROM.put(STORAGE_ALARM_MIN_DURATION_ADDR, 24);  
}

void get_stored_min()
{
  EEPROM.get(STORAGE_MIN_ADDR, rpm_min);
}

void get_stored_max()
{
  EEPROM.get(STORAGE_MAX_ADDR, rpm_max);  
}

void get_stored_alarm_enabled()
{
  EEPROM.get(STORAGE_ALARM_ENABLED_ADDR, alarm_enabled);
}

void get_stored_detection_min()
{
  EEPROM.get(STORAGE_DETECTION_MIN_ADDR, detection_min);
}

void get_stored_detection_max()
{
  EEPROM.get(STORAGE_DETECTION_MAX_ADDR, detection_max);
}

void get_stored_alarm_min_threshold()
{
  EEPROM.get(STORAGE_ALARM_MIN_THRESHOLD_ADDR, alarm_min_threshold);  
}

void get_stored_alarm_min_duration()
{
  EEPROM.get(STORAGE_ALARM_MIN_DURATION_ADDR, alarm_min_duration);  
}

void clear_min_max()
{
  rpm_min = 9.0f;
  rpm_max = 0.0f;
  store_min();
  store_max();
}

void store_min()
{
  EEPROM.put(STORAGE_MIN_ADDR, rpm_min);
}

void store_max()
{
  EEPROM.put(STORAGE_MAX_ADDR, rpm_max);  
}

void store_alarm_enabled()
{
  EEPROM.put(STORAGE_ALARM_ENABLED_ADDR, alarm_enabled);
}

void store_detection_min()
{
  EEPROM.put(STORAGE_DETECTION_MIN_ADDR, detection_min);
}

void store_detection_max()
{
  EEPROM.put(STORAGE_DETECTION_MAX_ADDR, detection_max);
}

void store_alarm_min_threshold()
{
  EEPROM.put(STORAGE_ALARM_MIN_THRESHOLD_ADDR, alarm_min_threshold);  
}

void store_alarm_min_duration()
{
  EEPROM.put(STORAGE_ALARM_MIN_DURATION_ADDR, alarm_min_duration);  
}

