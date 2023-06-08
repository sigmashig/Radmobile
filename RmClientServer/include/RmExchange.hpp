#pragma once
#include <Arduino.h>
#include "RmProtocol.hpp"
#include "RmId.hpp"

class RmExchange
{
public:
    RmExchange();
    IdConfig GenerateId();
    bool SendId(IdConfig idConfig);
    ~RmExchange();

private:
    RmProtocol *xchngProtocol;
};