#include "StateMachine.h"

#define CURRENCY "GHS "

StateMachine::StateMachine(LiquidCrystal_I2C *lcd)
{
    amount = "";
    state = State::STARTING_UP;
    previousState = State::INVALID;
    screen = lcd;
}

void StateMachine::setState(State s)
{
    previousState = state;
    state = s;
}

State StateMachine::getState()
{
    return state;
}

bool StateMachine::stateChanged()
{
    return state != previousState;
}

void StateMachine::setup()
{
    screen->init();
    screen->backlight();
    screen->setCursor(0, 0);
    screen->print("STARTING UP...");
    screen->setCursor(0, 1);
    screen->print("PLEASE WAIT...");
}

void StateMachine::getInput()
{
    if(stateChanged())
    {
        screen->clear();
    }

    screen->setCursor(0, 0);
    screen->print("ENTER AMOUNT : ");
    screen->setCursor(0, 1);
    screen->print(CURRENCY);
    screen->blink_on();
}

void StateMachine::writeToRFID()
{
    if(stateChanged())
    {
        screen->clear();
    }

    screen->blink_off();
    screen->setCursor(0, 0);
    screen->print("PLACE CARD");
}

void StateMachine::run()
{
    switch (state)
    {
    case State::READY_FOR_INPUT:
        getInput();
        break;
    case State::WRITE_TO_RFID:
        writeToRFID();
        break;

    default:
        break;
    }
}
