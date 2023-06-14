#pragma once
#include <Arduino.h>
#include "RmProtocol.hpp"
#include "RmId.hpp"

class RmExchange
{
public:
    RmExchange();
    void Begin();
    IdConfig GenerateId();
    bool SendId();
    bool SendAck(bool result);
    bool ReceiveCmd(String pkg);
    bool SendEnd();
    ~RmExchange();
    bool IsReadAvailable() { return xchngProtocol->IsReadAvailable(); }
    const ulong ExchangeDuration = 5 * 60 * 1000; // 5 minutes
    bool IsIdReceived = false;
    bool IsAckReceived = false;
    bool IsAckGood = false;
    bool IsEndReceived = false;

private:
    RmProtocol *xchngProtocol;
    IdConfig idConfig;
};