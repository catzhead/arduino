#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("yo?");
}

int yo = 0;

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Starting");
  lcd.setCursor(0,1);
  lcd.print(yo);
  yo++;
  delay(10);
}
