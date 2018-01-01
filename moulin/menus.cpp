#include "globals.h"
#include "menus.h"

void menu_rpm()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("t/min:");
    lcd.setCursor(0,1);
    lcd.print("min:");
    lcd.setCursor(8,1);
    lcd.print("max:");
  }
  
  lcd.setCursor(6,0);
  lcd.print(rpm);
  lcd.setCursor(4,1);
  char buf[4];
  dtostrf(rpm_min, 3, 1, buf);
  lcd.print(buf);
  lcd.setCursor(12,1);
  dtostrf(rpm_max, 3, 1, buf);
  lcd.print(buf);

  lcd.setCursor(15,0);
  if (detection)
    lcd.print('*');
  else
    lcd.print(' ');
}

void menu_enable_alarm()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("alarme activee:");
  }
  lcd.setCursor(0,1);
  if (alarm_enabled)
    lcd.print("oui");
  else
    lcd.print("non");
}

void menu_detection_min()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("detect min (mm):");
  }
  lcd.setCursor(0,1);
  lcd.print(detection_min);
  lcd.print("    ");
}


void menu_detection_max()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("detect max (mm):");
  }
  lcd.setCursor(0,1);
  lcd.print(detection_max);
  lcd.print("    ");
}

void menu_alarm_min_threshold()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarme min:");
    lcd.setCursor(5,1);
    lcd.print("t/min");
  }  
  lcd.setCursor(0,1);
  char buf[4];
  dtostrf(alarm_min_threshold, 3, 1, buf);
  lcd.print(buf);
}

void menu_alarm_min_duration()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Alarme min apres:");
    lcd.setCursor(5,1);
    lcd.print("heure(s)");
  }  
  lcd.setCursor(0,1);
  lcd.print(alarm_min_duration);
}

void menu_test_alarm()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Test SMS: ");
  }
  
  lcd.setCursor(0,1);
  if (send_alarm_request)
  {
    lcd.print("Envoi alarme");
  }
  else
  {
    lcd.print("Pas d'alarme");
  }
}

const fptr menu[LAST_MENU_INDEX+1] = {&menu_rpm,
                                      &menu_enable_alarm,
                                      &menu_detection_min,
                                      &menu_detection_max,
                                      &menu_alarm_min_threshold,
                                      &menu_alarm_min_duration,
                                      &menu_test_alarm};
