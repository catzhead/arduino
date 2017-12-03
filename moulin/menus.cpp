#include "globals.h"
#include "menus.h"

void menu_rpm()
{
  static int previous_count = 0;
  
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
  if (count != previous_count)
    lcd.print('*');
  else
    lcd.print(' ');
  previous_count = count;
}

void menu_enable_alarm()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("alarme activee:");
  lcd.setCursor(0,1);
  if (alarm_enabled)
    lcd.print("oui");
  else
    lcd.print("non");
}

const fptr menu[LAST_MENU_INDEX+1] = {&menu_rpm, &menu_enable_alarm};
