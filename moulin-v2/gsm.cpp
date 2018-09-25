#include <StandardCplusplus.h>
#include <string>

#include "gsm.hpp"

#define VERBOSE

GSM::GSMManager::GSMManager(Display::DisplayManager* display) :
  _display{display}
{
  _sim900 = new SoftwareSerial(23, 25);
}

void GSM::GSMManager::init() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  _sim900->begin(19200);
}

void GSM::GSMManager::start()
{
  // Turn on the board
  digitalWrite(27, HIGH);
#ifdef VERBOSE
  _display->scrollingtextarea->print("pin 27 high");
#endif
  delay(1000);

#ifdef VERBOSE
  digitalWrite(27, LOW);
  _display->scrollingtextarea->print("pin 27 low");
#endif
  delay(2000);

  _sim900->println("AT");
  delay(100);

  _sim900->println("AT+CPIN=\"1234\"");
  delay(100);

  _clear_incoming_serial();
}

void GSM::GSMManager::stop()
{
  // Turn off board
  digitalWrite(27, HIGH);
  delay(1000);
  digitalWrite(27, LOW);
  delay(2000);
}

void GSM::GSMManager::send_SMS(const char*)
{
}

void GSM::GSMManager::_clear_incoming_serial()
{
  String incomingString = "";
  char incomingByte;

  while (_sim900->available())
  {
    incomingByte = _sim900->read();
  }
}

void GSM::GSMManager::_display_signal_strength()
{
  // Check signal strength
  _sim900->println("AT+CSQ");
  delay(100);

  std::string incoming_string = "";

  while (_sim900->available())
  {
    incoming_string += _sim900->read();
  }

  if (incoming_string.length() > 0)
  {
    std::string csq = "CSQ:";
    _display->scrollingtextarea->print(csq);
    _display->scrollingtextarea->print(incoming_string);
  }
}

#if 0

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

void GSM_send_SMS(const char *tel_number) {
  SIM900.println("AT");
  delay(100);

  // AT+CMFG=1 command to set SIM900 to SMS mode
  SIM900.println("AT+CMGF=1");
  delay(100);

  char str[40] = "";
  strcat(str, "AT + CMGS = \"");
  strcat(str, tel_number);
  strcat(str, "\"");

  //SIM900.println("AT + CMGS = \"" + tel_number + "\"");
  SIM900.println(str);
  delay(100);

  if (message_alarm)
  {
    SIM900.print("Attention, vitesse roue trop basse: < ");
    SIM900.print(alarm_min_threshold);
    SIM900.print(" tours/min pendant plus de ");
    SIM900.print(alarm_min_duration);
    SIM900.println(" heure(s)");
  }
  else
  {
    SIM900.print("Nominal, vitesse roue actuelle: ");
    SIM900.print(rpm);
    SIM900.print(" tours/min (min: ");
    SIM900.print(rpm_min);
    SIM900.print(", max: ");
    SIM900.print(rpm_max);
    SIM900.println(")");
  }
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
#endif