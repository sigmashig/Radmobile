#pragma once

#include <Arduino.h>
#include <esp_event.h>

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

class RmProtocol
{
public:
    RmProtocol();
    virtual void Begin() = 0;
    virtual void Reconnect() = 0;
    //virtual void ReceivedPkg(String state) = 0;
    virtual bool SendPkg(String command) = 0;
    virtual bool IsReady() { return isReady; };
    virtual int IsReadAvailable() { return 0; };
    //virtual ~RmProtocol() = 0;

protected:
    bool isReady = false;
};

extern RmProtocol *rmProtocol;