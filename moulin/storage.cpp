#include "globals.h"
#include "storage.h"
#include <EEPROM.h>

#define STORAGE_MIN_ADDR 0
#define STORAGE_MAX_ADDR 4
#define STORAGE_ALARM_ENABLED_ADDR 8 

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

