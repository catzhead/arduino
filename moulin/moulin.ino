#include "globals.h"
#include "menus.h"
#include "storage.h"
#include "buttons.h"
#include "gsm.h"

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

unsigned long last_sampling_time;
int current_menu = 0;
int new_menu = 1;
int count = 0;
float rpm = 0, rpm_min = 0, rpm_max = 0;
int previous_distance = 0;
bool alarm_enabled = true;
bool detection = false;
int detection_min = 0;
int detection_max = 0;
float alarm_min_threshold = 0.0f;
int alarm_min_duration = 0;

void setup() {

  #ifdef GSM_ENABLED
    init_GSM();
  #else
    Serial.begin(9600);
    Serial.println("Starting");
  #endif

  #ifdef LCD_ENABLED
    lcd.begin(16,2);
    lcd.clear();
  #endif

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  get_stored_min();
  get_stored_max();
  get_stored_alarm_enabled();
  get_stored_detection_min();
  get_stored_detection_max();
  get_stored_alarm_min_threshold();
  get_stored_alarm_min_duration();

  last_sampling_time = millis();

  #ifdef LCD_ENABLED
    menu[current_menu]();
  #endif
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // 2ms à LOW pour permettre de détecter HIGH
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // 10ms à HIGH

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 29 / 2; // vitesse du son: 29cm/s, aller-retour

  if ((distance> detection_min) && (distance < detection_max))
  {
    if ((previous_distance < detection_min) || (previous_distance > detection_max))
    {
      count++;
    }
    detection = true;
  }
  else
  {
    detection = false;
  }
  previous_distance = distance;

  #ifdef DEBUG_ENABLED
    Serial.print("Distance: ");
    Serial.println(distance);
  #endif

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_sampling_time) >= PERIOD)
  {
    /* simple règle de 3, et il y a 8 branches à la roue donc on 
     * divise le compteur par 8 */
    rpm = 60000.0f * (count / 8.0f) / (float)(current_time - last_sampling_time);
    last_sampling_time = current_time;

    if (rpm < rpm_min)
    {
      rpm_min = rpm;
      store_min();
    }
    if (rpm > rpm_max)
    {
      rpm_max = rpm;
      store_max();
    }
    
    count = 0;
  }

  #ifdef LCD_ENABLED
    menu[current_menu]();
  
    static int last_read_button = -1;
    int button = get_button();
    if ((button >= 0) && (button != last_read_button))
      menu_buttons[current_menu](button);
    last_read_button = button;
  #endif
  
  delay(100);
}
