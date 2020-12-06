#include <LiquidCrystal.h>

const int pin_RS = 8;
const int pin_EN = 9;
const int pin_D4 = 4;
const int pin_D5 = 5;
const int pin_D6 = 6;
const int pin_D7 = 7;
const int pin_BL = 10;
const int pin_Buttons = 0;

LiquidCrystal lcd(pin_RS, pin_EN, pin_D4, pin_D5, pin_D6, pin_D7);

void setup() {
  lcd.begin(16, 2);  // 16 col, 2 lines
  lcd.setCursor(0, 0);
  lcd.print("yo");
  lcd.setCursor(0, 1);
  lcd.print("---");
}

void loop() {
  lcd.setCursor(10, 1);
  int button = analogRead(pin_Buttons);
  if (button < 60) {
    lcd.print('R');
  }
  else if (button < 200) {
    lcd.print('U');
  }
  else if (button < 400) {
    lcd.print('D');
  }
  else if (button < 600) {
    lcd.print('L');
  }
  else if (button < 800) {
    lcd.print('S');
  }
}
