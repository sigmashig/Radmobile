#pragma once
#include "RmRemoteControl.hpp"

class RmRcEmulator : public RmRemoteControl
{
public:
    RmRcEmulator();
    void Begin();

private:
    static void loopTask(void *params);
    static void loopEventHandle(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};