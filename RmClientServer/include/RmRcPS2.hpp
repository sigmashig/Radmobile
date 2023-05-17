#pragma once
#include <Arduino.h>
#include <PSX.h>
#include <RmRemoteControl.hpp>

class RmRcPS2 : public RmRemoteControl
{
public:
    RmRcPS2();
    void Begin();
    void SendCommand(String command);

private:
    PSX psx;
    PSX::PSXDATA lastData;
    bool isFirst = true;

    static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    int stickToDirection(byte x);
    void cmdProcessing(PSX::PSXDATA psData);
};

extern RmRcPS2 *PS2;
