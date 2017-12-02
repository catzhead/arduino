#include <LiquidCrystal.h>

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

/* Période d'échantillonage en ms pour le calcul de la vitesse */
#define PERIOD 60000

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

unsigned long last_sampling_time;

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("count:");
  lcd.setCursor(0,1);
  lcd.print("t/min:");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  last_sampling_time = millis();
}

int count = 0;
int previous_distance = 1000;
float rpm = 0;

void loop() {
  Serial.println("Starting");

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

  unsigned long current_time = millis();
  if ((unsigned long)(current_time - last_sampling_time) >= PERIOD)
  {
    /* simple règle de 3, et il y a 8 branches à la roue donc on 
     * divise le compteur par 8 */
    rpm = 60000.0f * (count / 8.0f) / (float)(current_time - last_sampling_time);
    last_sampling_time = current_time;
    count = 0;
  }

  /*
  lcd.setCursor(9,0);
  lcd.print(distance);
  lcd.print("     ");
  */
  lcd.setCursor(6,0);
  lcd.print(count);
  lcd.print("     ");
  lcd.setCursor(5,1);
  lcd.print(rpm);
  lcd.print("     ");
  Serial.print("distance:");
  Serial.println(distance);
  Serial.print("count:");
  Serial.println(count);
  Serial.print("next_sampling_time:");
  Serial.println(last_sampling_time);
  delay(200);
}
