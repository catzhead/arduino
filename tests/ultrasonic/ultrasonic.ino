#include <LiquidCrystal.h>

/* Affiche des infos sur la liaison série */
//#define DEBUG_ULTRASONIC
//#define DEBUG_BUTTONS

/* Configuration pour le capteur ultrason */
#define TRIG_PIN 9
#define ECHO_PIN 8

/* Configuration pour le LCD */
#define LCD_RS     12
#define LCD_ENABLE 11
#define LCD_D4     5
#define LCD_D5     4
#define LCD_D6     3
#define LCD_D7     2

/* Entrée pour les boutons */
#define BUTTONS_PIN A0

/* Période d'échantillonage en ms pour le calcul de la vitesse */
#define PERIOD 60000

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

unsigned long last_sampling_time;
int current_menu = 0;
int new_menu = 1;
int count = 0;
float rpm = 0;
int previous_distance = 1000;

void manage_button(int button)
{
  if (button < 0)
    return;

  int old_menu = current_menu;

  if (button == 0)
  {
    if (current_menu > 0)
      current_menu--;
  }
  else if (button == 1)
  {
    if (current_menu < 1)
      current_menu++;
  }

  if (current_menu != old_menu)
    new_menu = 1;
}

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

typedef void (* fptr)(); 
const fptr menus[2] = {&print_rpm, &print_yo};

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  lcd.begin(16,2);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  last_sampling_time = millis();
}

void loop() {
  long duration, distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);  // 2ms à LOW pour permettre de détecter HIGH
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10); // 10ms à HIGH

  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration / 29 / 2; // vitesse du son: 29cm/s, aller-retour

  if ((distance < 15) && (previous_distance > 15))
  {
    count++;
  }
  previous_distance = distance;

  Serial.print("Distance: ");
  Serial.println(distance);

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_sampling_time) >= PERIOD)
  {
    /* simple règle de 3, et il y a 8 branches à la roue donc on 
     * divise le compteur par 8 */
    rpm = 60000.0f * (count / 8.0f) / (float)(current_time - last_sampling_time);
    last_sampling_time = current_time;
    count = 0;
  }

  int value = analogRead(BUTTONS_PIN);
  int button;
  if (value < 300)
    button = -1; 
  else if (value < 600)
    button = 0;
  else if (value < 800)
    button = 1;
  else if (value < 900)
    button = 2;
  else
    button = 3; 

  manage_button(button);
  #ifdef DEBUG_BUTTONS
  Serial.println(value);
  lcd.setCursor(11, 0);
  lcd.print("     ");
  lcd.setCursor(11+button, 0);
  lcd.print('*');
  #endif

  #ifdef DEBUG_ULTRASONIC
  Serial.print("distance:");
  Serial.println(distance);
  Serial.print("count:");
  Serial.println(count);
  Serial.print("next_sampling_time:");
  Serial.println(last_sampling_time);
  #endif

  menus[current_menu]();
  
  delay(250);
}
