#pragma once
#include <Arduino.h>
#include "RmPinsDriver.hpp"

class RmPinsDriverPcf : public RmPinsDriver
{
    public:
        RmPinsDriverPcf(byte id, byte address, byte irqPin);
};