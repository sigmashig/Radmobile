#pragma once
#include "RmRemoteControl.hpp"

#define EMULATOR_LOOP_STACK_SIZE 2000
class RmRcEmulator : public RmRemoteControl
{
public:
    RmRcEmulator();
    void Begin();

private:
    StaticTask_t loopTaskBuffer;
    StackType_t loopTaskStack[EMULATOR_LOOP_STACK_SIZE];
    TaskHandle_t loopTaskHandle;
    static void loopTask(void *params);
};