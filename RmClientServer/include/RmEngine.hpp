#pragma once
#include <Arduino.h>
#include "RmConfiguration.hpp"
#include "RmTypes.hpp"

class RmEngine
{
public:

    RmEngine(EngineConfig config);
    virtual void Begin() = 0;
    virtual void Run(Direction direction, EngineAction action, int power) = 0;
    virtual void GetSpeed() = 0;
    int GetPower() { return power; };
    Direction GetDirection() { return direction; };
protected:
    EngineConfig config;
    int power;
    Direction direction;
};