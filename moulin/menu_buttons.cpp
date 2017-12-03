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

void menu_detection_min_buttons(int button)
{
  switch (button)
  {
    case 0:
    case 1:
      change_menu(button);
      break;
      
    case 2:
      if (detection_min > DETECTION_MIN_LIMIT)
        detection_min--;
      break;
      
    case 3:
      if (detection_min < detection_max - 1)
        detection_min++;
      break;
  }  
  store_detection_min();
}

void menu_detection_max_buttons(int button)
{
  switch (button)
  {
    case 0:
    case 1:
      change_menu(button);
      break;
      
    case 2:
      if (detection_max > detection_min + 1)
        detection_max--;
      break;
      
    case 3:
      if (detection_max < DETECTION_MAX_LIMIT)
        detection_max++;
      break;
  }  
  store_detection_max();
}

void menu_alarm_min_threshold_buttons(int button)
{
  switch (button)
  {
    case 0:
    case 1:
      change_menu(button);
      break;
      
    case 2:
      if (alarm_min_threshold > 0.1f)
      {
        alarm_min_threshold -= 0.1f;
        if (alarm_min_threshold < 0.1f)
          alarm_min_threshold = 0.1f;
      }
      break;
      
    case 3:
      alarm_min_threshold += 0.1f;
      break;
  }  
  store_alarm_min_threshold();  
}

void menu_alarm_min_duration_buttons(int button)
{
  switch (button)
  {
    case 0:
    case 1:
      change_menu(button);
      break;
      
    case 2:
      if (alarm_min_duration > 0)
        alarm_min_duration--;
      break;
      
    case 3:
      if (alarm_min_duration < 150)
        alarm_min_duration++;
      break;
  }  
  store_alarm_min_threshold();    
}

const fptr_int menu_buttons[LAST_MENU_INDEX+1] = {&menu_rpm_buttons,
                                                  &menu_alarm_buttons,
                                                  &menu_detection_min_buttons,
                                                  &menu_detection_max_buttons,
                                                  &menu_alarm_min_threshold_buttons,
                                                  &menu_alarm_min_duration_buttons};
