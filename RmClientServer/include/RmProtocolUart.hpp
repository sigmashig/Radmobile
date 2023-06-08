#pragma once
#include "RmProtocol.hpp"


class RmProtocolUart : public RmProtocol
{
public:
    RmProtocolUart(HardwareSerial *uart);
    void Begin();
    void Reconnect();
    bool SendPkg(String command);
    bool IsReady() { return isReady; };
    ~RmProtocolUart();

private:
    HardwareSerial *uart;
    bool isReady = false;
    static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

extern RmProtocolUart *rmProtocolUart;
