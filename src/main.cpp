#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Key.h>
#include <Keypad.h>

#include <SPI.h>
#include <MFRC522.h>

#include "StateMachine.h"

#define RST_PIN 14 // Configurable, see typical pin layout above
#define SS_PIN 10

void keypadListener(KeypadEvent key);

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad *keypad = new Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
LiquidCrystal_I2C *lcd = new LiquidCrystal_I2C(0x27, 16, 2);
MFRC522 *reader = new MFRC522(SS_PIN, RST_PIN);

String amount = "";
StateMachine machine = StateMachine(lcd, reader, &amount);


void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  SPI.begin();
  reader->PCD_Init();
  delay(4);

  machine.setup();
  delay(2000);
  keypad->addEventListener(keypadListener);
  machine.setState(State::READY_FOR_INPUT);
}

void loop()
{
  machine.run();
  keypad->getKey();
  delay(100);
}

void keypadListener(KeypadEvent key)
{
  Serial.println(key);
  switch (keypad->getState())
  {
  case PRESSED:
    if (key == 'D' && amount != "")
    {
      machine.setAmount(amount);
      machine.setState(State::WRITE_TO_RFID);
    }
    else if (key == 'C')
    {
      amount = "";
      machine.setAmount(amount);
      machine.setState(State::READY_FOR_INPUT);
    }
    else if (key == 'B')
    {
      machine.setState(State::READ_FROM_RFID);
    }
    else if (key == 'A' || key == '*' || key == '#')
    {
      // Ignore these keys
    }
    else
    {
      amount = amount + key;
      machine.setAmount(amount);
      Serial.println(amount);
    }
    break;
  case IDLE:
    break;
  case HOLD:
    break;
  case RELEASED:
    break;
  }
}
