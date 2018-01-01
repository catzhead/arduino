#include "globals.h"
#include "config.h"

#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);

void GSM_init() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  SIM900.begin(19200);
}

void GSM_start() {
  // Turn on the board
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(2000);

  SIM900.println("AT");
  delay(100);
  
  SIM900.println("AT+CPIN=\"1234\"");
  delay(100);
}

void GSM_send_SMS() {
  SIM900.println("AT"); 
  delay(100);
  
  // AT+CMFG=1 command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1"); 
  delay(100);

  SIM900.println("AT + CMGS = \"" TEL1 "\""); 
  delay(100);
  
  SIM900.println("Message example from Arduino Uno."); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26); 
  delay(100);
}

void GSM_stop() {
  // Turn off board
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(2000);
}

