#include "StateMachine.h"

#define CURRENCY "GHS "
#define EMPTY_STR ""

#define BLOCK 3

StateMachine::StateMachine(LiquidCrystal_I2C *lcd, MFRC522 *mfrc522, String *inp)
{
    amount = EMPTY_STR;
    input = inp;
    state = State::STARTING_UP;
    previousState = State::INVALID;
    previousAmount = EMPTY_STR;
    screen = lcd;
    reader = mfrc522;
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

void StateMachine::setAmount(String a)
{
    previousAmount = amount;
    amount = a;
}

String StateMachine::getAmount()
{
    return amount;
}

bool StateMachine::stateChanged()
{
    return state != previousState;
}

bool StateMachine::amountChanged()
{
    return amount != previousAmount;
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
    if (stateChanged() || amountChanged())
    {
        screen->clear();
    }

    screen->setCursor(0, 0);
    screen->print("ENTER AMOUNT : ");
    screen->setCursor(0, 1);
    screen->print(CURRENCY);
    screen->blink_on();

    if (amount != EMPTY_STR)
    {
        screen->print(amount);
    }
}

void StateMachine::writeToRFID()
{
    if (stateChanged())
    {
        screen->clear();
    }

    while (!reader->PICC_IsNewCardPresent())
    {
        screen->blink_off();
        screen->setCursor(0, 0);
        screen->print("PLACE CARD NEAR");
        screen->setCursor(0, 1);
        screen->print("THE RFID READER");
    }

    screen->clear();
    screen->setCursor(0, 0);
    screen->print("WRITING TO CARD");
    screen->setCursor(0, 1);
    screen->print("PLEASE WAIT...");

    byte blockcontent[16];

    int inputLen = amount.length();
    int padLen = 16 - inputLen;
    String pad = "";
    for(int i=0; i<padLen; i++){
        pad += "0";
    }
    amount = pad + amount;
    amount.getBytes(blockcontent, 16);

    byte status = reader->MIFARE_Write(BLOCK, blockcontent, 16);
    if (status != MFRC522::STATUS_OK)
    {
        screen->clear();
        screen->setCursor(0, 0);
        screen->print("WRITING DONE");
    }
    
    delay(2000);
    *input = "";
    setAmount("");
    setState(State::READY_FOR_INPUT);
}

void StateMachine::readFromFRID()
{
    if (stateChanged())
    {
        screen->clear();
    }

    while (!reader->PICC_IsNewCardPresent())
    {
        screen->blink_off();
        screen->setCursor(0, 0);
        screen->print("PLACE CARD NEAR");
        screen->setCursor(0, 1);
        screen->print("THE RFID READER");
    }

    byte blockContent[18];
    byte buffersize = 18;
    byte status = reader->MIFARE_Read(BLOCK, blockContent, &buffersize);
    if (status != MFRC522::STATUS_OK)
    {
        String value = String((char *)blockContent);
        Serial.println(value);
        screen->clear();
        screen->setCursor(0, 0);
        screen->print(value);
    }
    delay(2000);
    setAmount("");
    setState(State::READY_FOR_INPUT);
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
    case State::READ_FROM_RFID:
        readFromFRID();
        break;

    default:
        break;
    }
}
