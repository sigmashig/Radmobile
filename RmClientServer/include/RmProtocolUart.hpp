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
    int IsReadAvailable() { return uart->available(); };
    ~RmProtocolUart();

private:
    HardwareSerial *uart;
    bool isReady = false;
    TimerHandle_t timer;
    StaticTimer_t uartTimer;
    static void uartTimerCallback(TimerHandle_t xTimer);
};

extern RmProtocolUart *rmProtocolUart;
