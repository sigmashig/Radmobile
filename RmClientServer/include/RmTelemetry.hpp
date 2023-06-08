#pragma once
#include <Arduino.h>
#include <RmTypes.hpp>
#include "RmConfiguration.hpp"

class RmTelemetry
{
    public:
        RmTelemetry();
        void Begin();
        void SendAll();

    private:
        static void rmEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};
extern RmTelemetry *rmTelemetry;
