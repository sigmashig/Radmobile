#pragma once

#include <Arduino.h>


class RmProtocol
{
    public:
        RmProtocol();
        virtual void Begin() = 0;
};

extern RmProtocol *rmProtocol;