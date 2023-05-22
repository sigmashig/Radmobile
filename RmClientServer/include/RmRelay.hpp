#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"

class RmRelay
{
public:
    RmRelay(RelayConfig config);
    void Begin();
    void On();
    void Off();
    void Toggle();
    bool IsOn() { return isOn; };

private:
    RelayConfig config;
    bool isOn = false;
    TimerHandle_t timer;
    StaticTimer_t timerBuffer;
};