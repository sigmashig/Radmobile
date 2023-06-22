#include "RmRelay.hpp"
#include <SigmaIO.hpp>

RmRelay::RmRelay(RelayConfig config)
{
    this->config = config;

    sigmaIO->PinMode(config.pin, OUTPUT);
    
    timer = xTimerCreateStatic(
        "RmRelayTimer",
        pdMS_TO_TICKS(1000),
        pdFALSE,
        this,
        [](TimerHandle_t xTimer)
        {
            RmRelay *relay = (RmRelay *)pvTimerGetTimerID(xTimer);
            relay->Off();
        },
        &timerBuffer);

}

void RmRelay::Begin()
{
    Off();
}

void RmRelay::On()
{
    sigmaIO->DigitalWrite(config.pin, config.onLevel);
    isOn = true;
    if (config.type == RELAY_TRIGGER)
    {
        xTimerStart(timer, 0);
    }
}

void RmRelay::Off()
{
    sigmaIO->DigitalWrite(config.pin, !config.onLevel);
    isOn = false;
}
