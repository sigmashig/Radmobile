#pragma once
#include <Arduino.h>
#include "RmEngine.hpp"
#include "RmTypes.hpp"

class RmEngineYellow : public RmEngine
{
public:
    RmEngineYellow(EngineConfig config);
    void Begin();
    void Run(Direction direction, EngineAction action, uint power);
    uint GetSpeed();

private:
    static void pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};