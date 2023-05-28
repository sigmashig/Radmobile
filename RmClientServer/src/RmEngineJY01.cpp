#include "RmEngineJY01.hpp"
#include "RmTypes.hpp"
#include "RmClient.hpp"
#include <esp_event.h>

RmEngineJY01::RmEngineJY01(EngineConfig config) : RmEngine(config)
{
    rmPinsDriver->RegisterPin(config.connection.controllerJY01.vr);
    rmPinsDriver->SetPinMode(config.connection.controllerJY01.vr, RmPinsDriver::PinMode::PIN_OUTPUT);

    rmPinsDriver->RegisterPin(config.connection.controllerJY01.brake);
    rmPinsDriver->SetPinMode(config.connection.controllerJY01.brake, RmPinsDriver::PinMode::PIN_OUTPUT);

    rmPinsDriver->RegisterPin(config.connection.controllerJY01.zf);
    rmPinsDriver->SetPinMode(config.connection.controllerJY01.zf, RmPinsDriver::PinMode::PIN_OUTPUT);

    rmPinsDriver->RegisterPin(config.connection.controllerJY01.signal);
    rmPinsDriver->SetPinMode(config.connection.controllerJY01.signal, RmPinsDriver::PinMode::PIN_INPUT);

    // esp_event_handler_register(RM_PINS_DRIVER_EVENT, RM_PINS_DRIVER_ISR, pcfEventHandler, NULL);
}

void RmEngineJY01::Begin()
{
    Run(Direction::DIRECTION_NODIRECTION, EngineAction::ACTION_STOP, 0);
}

void RmEngineJY01::Run(Direction direction, EngineAction action, int power)
{
    if (action == EngineAction::ACTION_STOP)
    {
        rmPinsDriver->Write(config.connection.controllerJY01.vr, 0);
        rmPinsDriver->Write(config.connection.controllerJY01.brake, 1);
        return;
    }
    else
    {
        if (direction == Direction::DIRECTION_FORWARD)
        {
            rmPinsDriver->Write(config.connection.controllerJY01.zf, 1);
        }
        else
        {
            rmPinsDriver->Write(config.connection.controllerJY01.zf, 0);
        }
        if (action == EngineAction::ACTION_RUN)
        {
            rmPinsDriver->Write(config.connection.controllerJY01.brake, 0);
            rmPinsDriver->Write(config.connection.controllerJY01.vr, power);
        }
        else
        {
            rmPinsDriver->Write(config.connection.controllerJY01.vr, 0);
            rmPinsDriver->Write(config.connection.controllerJY01.brake, 1);
        }
    }
}

void RmEngineJY01::GetSpeed()
{ // For Future use
}

void RmEngineJY01::pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{ // For Future use
}
