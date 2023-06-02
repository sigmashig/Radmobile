#pragma once
#include <Arduino.h>
#include <RmTypes.hpp>
#include "RmConfiguration.hpp"

class RmTelemetry
{
    public:
        RmTelemetry();
        void Begin();
};
extern RmTelemetry *rmTelemetry;
