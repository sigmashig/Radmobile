#pragma once

#include "RmProtocol.hpp"
#include <RadioLib.h>

class RmProtocolLora : public RmProtocol
{
public:
    RmProtocolLora();
    void Begin();
    void Reconnect();
    void ReceivedCommand(String command);
    bool SendCommand(String command);

private:
    static void packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

//#if LORA == SX1278
    static LORA *radio;

//#endif
};