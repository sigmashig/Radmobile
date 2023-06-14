#pragma once
#include <Arduino.h>
#include <SigmaIO.hpp>
#include <PSX.h>
#include <RmRemoteControl.hpp>

#define PSXBTN_ACT_RIGHT    0x0400
#define PSXBTN_ACT_LEFT     0x0200

class RmPSX : public PSX
{
    void pinMode(byte pin, byte mode) { sigmaIO->PinMode(pin, mode); };
    void digitalWrite(byte pin, byte value) { sigmaIO->DigitalWrite(pin, value); };
    byte digitalRead(byte pin) { return sigmaIO->DigitalRead(pin); };
};

class RmRcPS2 : public RmRemoteControl
{
public:
    RmRcPS2();
    void Begin();
    //void SendCommand(String command);

private:
    RmPSX psx;
    PSX::PSXDATA lastData;
    //bool isFirst = true;

    static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    int stickToDirection(byte x);
    void cmdProcessing(PSX::PSXDATA psData);
};

extern RmRcPS2 *PS2;
