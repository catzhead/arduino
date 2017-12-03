#include "globals.h"
#include "storage.h"
#include <EEPROM.h>

#define STORAGE_MIN_ADDR 0
#define STORAGE_MAX_ADDR 4
#define STORAGE_ALARM_ENABLED_ADDR 8 
#define STORAGE_DETECTION_MIN_ADDR 12
#define STORAGE_DETECTION_MAX_ADDR 16

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

void clear_min_max()
{
  rpm_min = 0.0f;
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


