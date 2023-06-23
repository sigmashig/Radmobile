#pragma once
#include <Arduino.h>
#include "RmConfiguration.hpp"
#include "RmTypes.hpp"

class RmEngine
{
public:

    RmEngine(EngineConfig config);
    virtual void Begin() = 0;
    virtual void Run(Direction direction, EngineAction action, uint power) = 0;
    virtual uint GetSpeed() = 0;
    uint GetPower() { return power; };
    Direction GetDirection() { return direction; };
protected:
    EngineConfig config;
    uint power;
    Direction direction;
};