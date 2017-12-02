#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define trigPin 9
#define echoPin 8

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("distance:");
  lcd.setCursor(0,1);
  lcd.print("count:");

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int count = 0;
int previous_distance = 1000;

void loop() {
  Serial.println("Starting");

  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  duration = pulseIn(echoPin, HIGH);
  distance = duration / 29 / 2;

  if ((distance < 15) && (previous_distance > 15))
  {
    count++;
  }
  previous_distance = distance;

  lcd.setCursor(9,0);
  lcd.print(distance);
  lcd.print("     ");
  lcd.setCursor(9,1);
  lcd.print(count);
  lcd.print("     ");
  Serial.println(distance);
  delay(100);
}
