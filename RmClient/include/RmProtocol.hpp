#pragma once

#include <Arduino.h>

class RmProtocol
{
public:
    RmProtocol();
    virtual void Begin() = 0;
protected:
    String host;
    uint16_t port;
};
extern RmProtocol *rmProtocol;