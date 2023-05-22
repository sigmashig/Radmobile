#include "RmRelay.hpp"
#include "RmPinsDriver.hpp"

RmRelay::RmRelay(RelayConfig config)
{
    this->config = config;

    rmPinsDriver->RegisterPin(config.pin);
    rmPinsDriver->SetPinMode(config.pin, RmPinsDriver::PinMode::PIN_OUTPUT);

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
    rmPinsDriver->Write(config.pin, config.onLevel);
    isOn = true;
    if (config.type == RELAY_TRIGGER)
    {
        xTimerStart(timer, 0);
    }
}

void RmRelay::Off()
{
    rmPinsDriver->Write(config.pin, !config.onLevel);
    isOn = false;
}
