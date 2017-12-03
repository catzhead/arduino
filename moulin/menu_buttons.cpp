#include "globals.h"
#include "menus.h"
#include "storage.h"

void change_menu(int button)
{
  int old_menu = current_menu;

  if (button == 0)
  { 
    if (current_menu > 0)
      current_menu--;
  }
  else if (button == 1)
  {
    if (current_menu < LAST_MENU_INDEX)
      current_menu++;
  }
  
  if (current_menu != old_menu)
    new_menu = 1;
}

void menu_rpm_buttons(int button)
{
  if ((button == 0) || (button == 1))
    change_menu(button);

  if (button == 2)
    clear_min_max();
}

void menu_alarm_buttons(int button)
{
  if ((button == 0) || (button == 1))
    change_menu(button);

  if (button == 2)
    if (alarm_enabled)
      alarm_enabled = 0;
    else
      alarm_enabled = 1;

   store_alarm_enabled();
}

const fptr_int menu_buttons[LAST_MENU_INDEX+1] = {&menu_rpm_buttons, &menu_alarm_buttons};
