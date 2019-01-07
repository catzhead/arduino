#include <ArduinoSTL.h>
#include <string>

#include "gsm.hpp"

//#define VERBOSE

// Warning: not all pins are supported on Mega:
#define GSM_TX_PIN 50
#define GSM_RX_PIN 51
#define GSM_POWER_PIN 52

GSM::GSMManager::GSMManager(Display::DisplayManager* display) :
  _display{display}
{
  _sim900 = new SoftwareSerial(GSM_TX_PIN, GSM_RX_PIN);
  is_powered = false;
}

void GSM::GSMManager::init() {
  // Arduino communicates with SIM900 GSM shield at a baud rate of 19200
  _sim900->begin(19200);
}

void GSM::GSMManager::start()
{
  if (!_is_GSM_board_powered())
  {
    // Turn on the board
    digitalWrite(GSM_POWER_PIN, HIGH);
  #ifdef VERBOSE
    _display->scrollingtextarea->print("power pin high");
  #endif
    delay(10);

    digitalWrite(GSM_POWER_PIN, LOW);
  #ifdef VERBOSE
    _display->scrollingtextarea->print("power pin low");
  #endif

    int timeout = 10;
    while(!_is_GSM_board_powered() && timeout)
    {
      delay(100);
      timeout--;
    }

    if (!timeout)
    {
      _display->scrollingtextarea->print("GSM pwr timeout");
      return;
    }
  }

  _sim900->println("AT");
  delay(100);

  _sim900->println("AT+CPIN=\"1234\"");
  delay(100);

  _clear_incoming_serial();
}

void GSM::GSMManager::stop()
{
  // Turn off board
  digitalWrite(GSM_POWER_PIN, HIGH);
  delay(1000);
  digitalWrite(GSM_POWER_PIN, LOW);
  delay(2000);
}


void GSM::GSMManager::display_signal_strength()
{
  if (!is_powered) return;

  // Check signal strength
  _sim900->println("AT+CSQ");
  delay(100);

  std::string incoming_string = "";

  _get_incoming_answer(&incoming_string);

  if (incoming_string.length() > 0)
  {
    std::string csq = "CSQ:";
    //_display->scrollingtextarea->print(csq);
    //_display->scrollingtextarea->print(incoming_string);

    std::string tmp = "";
    boolean copy = false;

    for (char c : incoming_string)
    {
      if (c == ':')
        copy = true;

      if (c == ',')
        break;

      if (copy && c != ' ' && c != ':')
        tmp += c;
    }

    int value = atoi(tmp.c_str());

    _display->statusbar->set_signal_strength(value);
  }
  else
  {
    std::string str = "no CSQ ans.";
    _display->scrollingtextarea->print(str);
  }
}

void GSM::GSMManager::send_SMS(const char*)
{
  if (!is_powered) return;

  _sim900->println("AT");
  delay(100);

  // AT+CMFG=1 command to set SIM900 to SMS mode
  _sim900->println("AT+CMGF=1");
  delay(100);

  _sim900->println("AT + CMGS = \"0630291418\"");
  delay(100);

  _sim900->println("Message example from Arduino Uno.");
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  _sim900->write(26);
  delay(100);

}

bool GSM::GSMManager::_is_GSM_board_powered()
{
  is_powered = false;

  // Any request will do, just checking if the board is already powered
  _sim900->println("AT+CSQ");

  std::string incoming_str = "";
  _get_incoming_answer(&incoming_str);

  if (incoming_str.length() > 0)
  {
    is_powered = true;
  }

  return is_powered;
}

void GSM::GSMManager::_clear_incoming_serial()
{
  char incomingByte;

  while (_sim900->available())
  {
    _sim900->read();
  }
}

void GSM::GSMManager::_get_incoming_answer(std::string* buffer)
{
  while (_sim900->available())
  {
    *buffer += _sim900->read();
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
