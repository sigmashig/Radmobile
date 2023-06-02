#include "RmTelemetry.hpp"
#include "RmGPS.hpp"

RmTelemetry::RmTelemetry()
{
    rmGPS = new RmGPS(10);
}

void RmTelemetry::Begin()
{
}

RmTelemetry *rmTelemetry;