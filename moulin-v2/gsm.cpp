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

    // warning: the following delay must be long enough
    // (it seems like 1s is enough)
    delay(2500);

    digitalWrite(GSM_POWER_PIN, LOW);
  #ifdef VERBOSE
    _display->scrollingtextarea->print("power pin low");
  #endif

    delay(3000);

    if(!_is_GSM_board_powered())
    {
      _display->scrollingtextarea->print("GSM not responding");
      return;
    }
  }

  _sim900->println("AT");
  delay(100);

  _sim900->println("AT+CPIN=\"1234\"");
  delay(100);

  // waiting for the connection to the network
  delay(10000);

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
#if 0
  // not very useful
  else
  {
    std::string str = "no CSQ ans.";
    _display->scrollingtextarea->print(str);
  }
#endif
}

void GSM::GSMManager::send_SMS(const char* number, const char* msg)
{
  if (!is_powered) return;

  _sim900->println("AT");
  delay(100);

  // AT+CMFG=1 command to set SIM900 to SMS mode
  _sim900->println("AT+CMGF=1");
  delay(100);

  _sim900->print("AT + CMGS = \"");
  _sim900->print(number);
  _sim900->println("\"");
  delay(100);

  _sim900->println(msg);
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  _sim900->write(26);
  delay(100);
}

bool GSM::GSMManager::_is_GSM_board_powered()
{
  _clear_incoming_serial();

  is_powered = false;

  // Any request will do, just checking if the board is already powered
  _sim900->println("AT+CSQ");
  delay(100);

  std::string incoming_str = "";
  _get_incoming_answer(&incoming_str);

  if (incoming_str.length() > 0)
  {
    Serial.println(incoming_str.c_str());
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