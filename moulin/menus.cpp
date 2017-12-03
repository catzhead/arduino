#include <Arduino.h>
#include <LiquidCrystal.h>
#include "globals.h"
#include "menus.h"

void print_rpm()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
    lcd.print("count:");
    lcd.setCursor(0,1);
    lcd.print("t/min:");
  }
  lcd.setCursor(6,0);
  lcd.print(count);
  lcd.setCursor(6,1);
  lcd.print(rpm);
}

void print_yo()
{
  if (new_menu)
  {
    new_menu = 0;
    lcd.clear();
  }
  lcd.setCursor(0,0);
  lcd.print("yo");
}

const fptr menus[LAST_MENU_INDEX+1] = {&print_rpm, &print_yo};

