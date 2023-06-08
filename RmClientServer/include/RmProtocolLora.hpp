#pragma once

#include "RmProtocol.hpp"
#include <RadioLib.h>

class RmProtocolLora : public RmProtocol
{
public:
    RmProtocolLora();
    void Begin();
    void Reconnect();
    //void ReceivedPkg(String state);
    bool SendPkg(String command);
    ~RmProtocolLora();
private:
    static void packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    static LORA *radio;
    static bool isChannelFree;
    static String pkgForTransmit;
    static void loraISR();
    static TimerHandle_t sendTimer;
    static StaticTimer_t sendTimerBuffer;
    static String lastPkg;
};

extern RmProtocolLora *rmProtocolLora;