#include <Arduino.h>
#include <Key.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H
enum State
{
    INVALID,
    STARTING_UP,
    READY_FOR_INPUT,
    WRITE_TO_RFID
};

class StateMachine
{
private:
    String amount;
    State state;
    State previousState;
    LiquidCrystal_I2C *screen;

public:
    StateMachine(LiquidCrystal_I2C *ldc);
    void setState(State);
    State getState();
    bool stateChanged();
    void setup();
    void getInput();
    void writeToRFID();
    void run();
};
#endif