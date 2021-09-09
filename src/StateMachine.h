#include <Arduino.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
enum State
{
    INVALID,
    STARTING_UP,
    READY_FOR_INPUT,
    WRITE_TO_RFID,
    READ_FROM_RFID
};

class StateMachine
{
private:
    String amount;
    String *input;
    String previousAmount;
    State state;
    State previousState;
    LiquidCrystal_I2C *screen;
    MFRC522 *reader;

public:
    StateMachine(LiquidCrystal_I2C *ldc, MFRC522 *mfrc522, String *input);
    void setState(State);
    State getState();
    void setAmount(String);
    String getAmount();
    bool stateChanged();
    bool amountChanged();
    void setup();
    void getInput();
    void writeToRFID();
    void readFromFRID();
    void run();
};
#endif