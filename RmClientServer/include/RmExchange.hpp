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
    bool SendId(IdConfig idConfig);
    IdConfig ReceiveId(String pkg);
    ~RmExchange();

private:
    RmProtocol *xchngProtocol;
};