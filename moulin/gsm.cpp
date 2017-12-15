#include "globals.h"
#include "config.h"

// # Steps:
// #        1. Turn the S1 switch to the Prog(right side)
// #        2. Turn the S2 switch to the Arduino side(left side)
// #        3. Set the UART select switch to middle one.
// #        4. Upload the sketch to the Arduino board
// #        5. Turn the S1 switch to the comm(left side) 
// #        6. RST the board 

const byte gsmDriverPin[3] = {
  GSM_PIN1,
  GSM_PIN2,
  GSM_PIN3};
  //The default digital driver pins for the GSM and GPS mode

void init_GSM()
{
  //Init the driver pins for GSM function
  for(int i = 0 ; i < 3; i++){
    pinMode(gsmDriverPin[i],OUTPUT);
  }
  digitalWrite(5,HIGH);//Output GSM Timing 
  delay(1500);
  digitalWrite(5,LOW);  
  digitalWrite(3,LOW);//Enable the GSM mode
  digitalWrite(4,HIGH);//Disable the GPS mode
  delay(2000);
  Serial.begin(9600); //set the baud rate
  delay(15000);//call ready
}

void send_alarm()
{
  static int buf;
   
  Serial.println("AT");
  delay(2000);
  Serial.println("AT");
  delay(2000);

  Serial.println("AT+CPIN=\"1234\"");
  delay(1000);

  //Send message
  Serial.println("AT+CMGF=1");
  delay(1000);
  Serial.println("AT+CMGS=\"" TEL1 "\"");
  delay(1000);
  Serial.print("Attention: vitesse de la roue trop basse");
  delay(1000);
  Serial.write(26);
}

