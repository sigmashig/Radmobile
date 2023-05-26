#pragma once

#include <Arduino.h>
#include <esp_event.h>

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

typedef enum
{
    RMEVENT_NONE,
    RMEVENT_CMD_RECEIVED,
    RMEVENT_LORA_SOMETHING_HAPPENS
} RmProtocolEvent;

class RmProtocol
{
public:
    RmProtocol();
    virtual void Begin() = 0;
    virtual void Reconnect() = 0;
    virtual void ReceivedCommand(String command) = 0;
    virtual bool SendCommand(String command) = 0;
};

extern RmProtocol *rmProtocol;