
#include <SoftwareSerial.h>

#define POWER_CMD

#if 0
// works on mega:
int tx = 10;
int rx = 11;
int power = 12;
#endif

/*
WARNING:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
*/

// works on mega:
int tx = 50;
int rx = 51;
int power = 52;

SoftwareSerial SIM900(tx, rx);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  // Make sure that corresponds to the baud rate of your module
  SIM900.begin(19200);

#ifdef POWER_CMD
  digitalWrite(power, HIGH);
  delay(1000);
  digitalWrite(power, LOW);
  delay(2000);
#endif

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

  // Turn off board
#ifdef POWER_CMD
  Serial.println("Stopping board");

  digitalWrite(power, HIGH);
  delay(1000);
  digitalWrite(power, LOW);
  delay(2000);
#endif

  Serial.println("End");
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

  String incomingString = "";
  char incomingByte;

  while (SIM900.available())
  {
    incomingByte = SIM900.read();
    incomingString += incomingByte;
  }
  
  // Check signal strength
  SIM900.println("AT+CSQ");
  delay(100);

  incomingString = "";

  while (SIM900.available())
  {
    incomingByte = SIM900.read();
    incomingString += incomingByte;
  }

  Serial.print("Answer to CSQ: ");
  Serial.print(incomingString.length());
  Serial.println(" bytes");
  Serial.println(incomingString);
  Serial.println("End of answer");

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"0781428861\"");
  //SIM900.println("AT + CMGS = \"0630291418\"");
  delay(100);

  // REPLACE WITH YOUR OWN SMS MESSAGE CONTENT
  SIM900.println("Message example from Arduino Uno.");
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.write(26);
  delay(100);

  Serial.println("Delay for SMS");

  // Give module time to send SMS
  delay(5000);

}
