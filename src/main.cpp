#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Key.h>
#include <Keypad.h>

#include "StateMachine.h"

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

StateMachine machine = StateMachine(lcd);

void setup()
{
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
  switch (keypad->getState())
  {
  case PRESSED:
    if (key == 'D')
    {
      machine.setState(State::WRITE_TO_RFID);
    }
    if(key == 'C')
    {
      machine.setState(State::READY_FOR_INPUT);
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
