#include "globals.h"
#include "menus.h"
#include "storage.h"
#include "buttons.h"
#include "gsm.h"

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

unsigned long last_sampling_time, last_sonic_sampling_time, last_alarm_sent_time, GSM_ref_time, last_time_rpm_above_threshold, last_heartbeat_time;
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
bool send_message_request = false;
bool message_alarm = false;
enum {E_GSM_STOP, E_GSM_START, E_GSM_SEND, E_GSM_FINISH} GSM_state = E_GSM_STOP;
const char *tel_numbers[NB_TEL_NUMBERS] = {TEL1, TEL2};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  #ifdef GSM_ENABLED
    GSM_init();
  #endif

  #ifdef LCD_ENABLED
    lcd.begin(16,2);
    lcd.clear();
  #endif

  #ifdef NEW_BOARD
  init_eeprom();
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

  last_sampling_time = last_sonic_sampling_time = last_alarm_sent_time = last_time_rpm_above_threshold = last_heartbeat_time = millis();

  #ifdef LCD_ENABLED
    menu[current_menu]();
  #endif
}

void loop() {
  long duration, distance;

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_sonic_sampling_time) >= DETECTION_MIN_DELAY)
  {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);  // 2ms à LOW pour permettre de détecter HIGH
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10); // 10ms à HIGH
  
    duration = pulseIn(ECHO_PIN, HIGH);
    distance = (float) duration / 2.9f / 2;
    
    if ((distance > detection_min) && (distance < detection_max))
    {
      if ((previous_distance < detection_min) || (previous_distance > detection_max))
      {
        count++;
      }
      detection = true;
      last_sonic_sampling_time = current_time;
    }
    else
    {
      detection = false;
    }
    
    previous_distance = distance;
    
    #ifdef DEBUG_ULTRASONIC
      Serial.print("Distance: ");
      Serial.println(distance);
    #endif    
  }

  if ((unsigned long)(current_time - last_sampling_time) >= PERIOD)
  {
    /* simple règle de 3, et il y a 8 branches à la roue donc on 
     * divise le compteur par 8 */
    rpm = ((float) PERIOD) * (count / 8.0f) / (float)(current_time - last_sampling_time);
    last_sampling_time = current_time;

    if (rpm < rpm_min)
    {
      rpm_min = rpm;
      if (rpm_min <= 0.0f)
        rpm_min = 0.0f;
      store_min();
    }
    if (rpm > rpm_max)
    {
      rpm_max = rpm;
      store_max();
    }
    
    count = 0;
  }

  if (rpm >= alarm_min_threshold)
  {
     last_time_rpm_above_threshold = current_time;
  }
  else
  {
    if ((unsigned long) (current_time - last_time_rpm_above_threshold) > ((unsigned long) alarm_min_duration * 3600 * 1000))
    {
      if (alarm_enabled)
      {
        send_message_request = true;
        message_alarm = true;
      }
      last_time_rpm_above_threshold = current_time;
    }
  }

  if ((unsigned long)(current_time - last_heartbeat_time) > ((unsigned long) HEARTBEAT_MESSAGE_DELAY * 3600 * 1000))
  {
    send_message_request = true;
    last_heartbeat_time = current_time;
  }

  #ifdef GSM_ENABLED
  static int current_tel;
  switch (GSM_state)
  {
    case E_GSM_STOP:
      if (send_message_request)
      {
        current_tel = 0;
        GSM_ref_time = current_time;
        GSM_state = E_GSM_START;
        Serial.println("Powering on GSM module");
        GSM_start();
      }
      break;

    case E_GSM_START:
      Serial.print(".");
      if ((unsigned long)(current_time - GSM_ref_time) >= GSM_START_DELAY)
      {
        Serial.println("");
        GSM_ref_time = current_time;
        GSM_state = E_GSM_SEND;
        Serial.println("Sending SMS");
        GSM_send_SMS(tel_numbers[current_tel++]);
      }
      break;

    case E_GSM_SEND:
      Serial.print(".");
      if ((unsigned long)(current_time - GSM_ref_time) >= GSM_SEND_DELAY)
      {
        Serial.println("");
        GSM_ref_time = current_time;
        if (current_tel == NB_TEL_NUMBERS)
        {
          GSM_state = E_GSM_FINISH;
          Serial.println("Stoping GSM module");
          GSM_stop();
        }
        else
        {
          GSM_state = E_GSM_START;
          Serial.println("Next tel number");
        }
      }
      break;

    case E_GSM_FINISH:
      Serial.print(".");
      if ((unsigned long)(current_time - GSM_ref_time) >= GSM_FINISH_DELAY)
      {
        Serial.println("");
        GSM_state = E_GSM_STOP;
        clear_min_max();
        send_message_request = false;
        last_alarm_sent_time = current_time;
      }
      break;
  }
  #endif 


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
