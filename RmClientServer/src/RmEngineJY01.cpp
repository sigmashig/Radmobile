#include "RmEngineJY01.hpp"
#include "RmTypes.hpp"
#include "RmClient.hpp"
#include <esp_event.h>

RmEngineJY01::RmEngineJY01(EngineConfig config) : RmEngine(config)
{

    rmClient->PinsDriver->SetPinMode(config.connection.controllerJY01.vr, RmPinsDriver::PinMode::PIN_OUTPUT);
    rmClient->PinsDriver->SetPinType(config.connection.controllerJY01.vr, PinType::PIN_PWM);

    rmClient->PinsDriver->SetPinMode(config.connection.controllerJY01.brake, RmPinsDriver::PinMode::PIN_OUTPUT);
    rmClient->PinsDriver->SetPinType(config.connection.controllerJY01.brake, PinType::PIN_DIGITAL);

    rmClient->PinsDriver->SetPinMode(config.connection.controllerJY01.zf, RmPinsDriver::PinMode::PIN_OUTPUT);
    rmClient->PinsDriver->SetPinType(config.connection.controllerJY01.zf, PinType::PIN_DIGITAL);

    rmClient->PinsDriver->SetPinMode(config.connection.controllerJY01.signal, RmPinsDriver::PinMode::PIN_INPUT);
    rmClient->PinsDriver->SetPinType(config.connection.controllerJY01.signal, PinType::PIN_DIGITAL);

    // esp_event_handler_register(RM_PINS_DRIVER_EVENT, RM_PINS_DRIVER_ISR, pcfEventHandler, NULL);
}

void RmEngineJY01::Begin()
{
    Run(EngineDirection::ENGINE_NODIRECTION, EngineAction::ACTION_STOP, 0);
}

void RmEngineJY01::Run(EngineDirection direction, EngineAction action, int power)
{
    if (action == EngineAction::ACTION_STOP)
    {
        rmClient->PinsDriver->Write(config.connection.controllerJY01.vr, 0);
        rmClient->PinsDriver->Write(config.connection.controllerJY01.brake, 1);
        return;
    }
    else
    {
        if (direction == EngineDirection::ENGINE_FORWARD)
        {
            rmClient->PinsDriver->Write(config.connection.controllerJY01.zf, 1);
        }
        else
        {
            rmClient->PinsDriver->Write(config.connection.controllerJY01.zf, 0);
        }
        if (action == EngineAction::ACTION_RUN)
        {
            rmClient->PinsDriver->Write(config.connection.controllerJY01.brake, 0);
            rmClient->PinsDriver->Write(config.connection.controllerJY01.vr, power);
        }
        else
        {
            rmClient->PinsDriver->Write(config.connection.controllerJY01.vr, 0);
            rmClient->PinsDriver->Write(config.connection.controllerJY01.brake, 1);
        }
    }
}

void RmEngineJY01::GetSpeed()
{ // For Future use
}

void RmEngineJY01::pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{ // For Future use
}
