#pragma once
#include "RmRemoteControl.hpp"

class RmRcEmulator : public RmRemoteControl
{
public:
    RmRcEmulator();
    void Begin();

private:
    StaticTimer_t loopTimerBuffer;
};