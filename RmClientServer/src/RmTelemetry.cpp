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

String RmTelemetry::pkgGPS(GpsPosition position)
{
    Log->Printf("SENDING GPS position: %f, %f, %f, %f", position.latitude, position.longitude, position.altitude, position.speed);
    char buffer[30];
    sprintf(buffer, "{G#%lu#%lu#%u#%u}", position.latitude * 1000000, position.longitude * 1000000, position.altitude * 1000, position.speed * 10);
    String s = String(buffer);
    return s;
}

void RmTelemetry::rmEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

    switch (event_id)
    {
    case RMEVENT_GPS_POSITION:
    {
        rmProtocol->SendCommand(pkgGPS(*(GpsPosition *)event_data));
    }
    break;
    }
}

void RmTelemetry::SendAll()
{
    rmProtocol->SendCommand(pkgGPS(rmGPS->GetPosition()));
}
RmTelemetry *rmTelemetry;