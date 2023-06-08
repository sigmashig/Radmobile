#include "RmTelemetry.hpp"
#include "RmGPS.hpp"
#include "SigmaLoger.hpp"
#include <esp_event.h>
#include "rmProtocol.hpp"

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

RmTelemetry::RmTelemetry()
{
    esp_event_handler_register(RMPROTOCOL_EVENT, ESP_EVENT_ANY_ID, rmEventHandler, this);
}

void RmTelemetry::Begin()
{
}

void RmTelemetry::rmEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    switch (event_id)
    {
    case RMEVENT_GPS_POSITION:
    {
        rmProtocol->SendPkg(RmGPS::GpsAsString(*(GpsPosition *)event_data));
    }
    break;
    }
}

void RmTelemetry::SendAll()
{
    rmProtocol->SendPkg(RmGPS::GpsAsString(rmGPS->GetPosition()));
}
RmTelemetry *rmTelemetry;