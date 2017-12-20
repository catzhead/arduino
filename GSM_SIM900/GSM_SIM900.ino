
#include <SoftwareSerial.h>

SoftwareSerial SIM900(7, 8);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);

  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(2000);

  SIM900.println("AT");
  delay(100);
  
  SIM900.println("AT+CPIN=\"1234\"");
  delay(100);
  
  // Give time to your GSM shield log on to network
  for (int i=0; i<10; i++)
  {
    delay(1000);   
    Serial.print(".");
  }
  Serial.println("");
  
  // Send the SMS
  sendSMS();
}

void loop() {
}

void sendSMS() {
  Serial.println("Sending SMS");

  SIM900.println("AT"); 
  delay(100);
  
  // put your main code here, to run repeatedly:
  // AT command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1"); 
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  //SIM900.println("AT + CMGS = \"0781428861\""); 
  SIM900.println("AT + CMGS = \"0630291418\""); 
  delay(100);
  
  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("Message example from Arduino Uno."); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26); 
  delay(100);

  // Give module time to send SMS
  delay(5000); 
}
