#include "RmEngineYellow.hpp"
#include "RmTypes.hpp"
#include "RmClient.hpp"
#include <esp_event.h>

RmEngineYellow::RmEngineYellow(EngineConfig config) : RmEngine(config)
{
    rmPinsDriver->RegisterPin(config.connection.controllerL298N.in1);
    rmPinsDriver->SetPinMode(config.connection.controllerL298N.in1, RmPinsDriver::PinMode::PIN_OUTPUT);

    rmPinsDriver->RegisterPin(config.connection.controllerL298N.in2);
    rmPinsDriver->SetPinMode(config.connection.controllerL298N.in2, RmPinsDriver::PinMode::PIN_OUTPUT);

    rmPinsDriver->RegisterPin(config.connection.controllerL298N.en);
    rmPinsDriver->SetPinMode(config.connection.controllerL298N.en, RmPinsDriver::PinMode::PIN_OUTPUT);
    power = 0;
    direction = EngineDirection::ENGINE_NODIRECTION;
    // esp_event_handler_register(RM_PINS_DRIVER_EVENT, RM_PINS_DRIVER_ISR, pcfEventHandler, NULL);
}

void RmEngineYellow::Begin()
{
    Run(EngineDirection::ENGINE_NODIRECTION, EngineAction::ACTION_STOP, 0);
}

void RmEngineYellow::Run(EngineDirection direction, EngineAction action, int power)
{
    this->power = power;
    this->direction = direction;
    if (action == EngineAction::ACTION_STOP)
    {
        this->power = 0;
        direction = EngineDirection::ENGINE_NODIRECTION;
        rmPinsDriver->Write(config.connection.controllerL298N.en, 0);
        rmPinsDriver->Write(config.connection.controllerL298N.in1, 0);
        rmPinsDriver->Write(config.connection.controllerL298N.in2, 0);
        return;
    }
    else
    {
        if (direction == EngineDirection::ENGINE_FORWARD)
        {
            rmPinsDriver->Write(config.connection.controllerL298N.in1, 1);
            rmPinsDriver->Write(config.connection.controllerL298N.in2, 0);
        }
        else
        {
            rmPinsDriver->Write(config.connection.controllerL298N.in1, 0);
            rmPinsDriver->Write(config.connection.controllerL298N.in2, 1);
        }
        if (action == EngineAction::ACTION_RUN)
        {
            rmPinsDriver->Write(config.connection.controllerL298N.en, power);
        }
        else
        {
            rmPinsDriver->Write(config.connection.controllerL298N.in1, 0);
            rmPinsDriver->Write(config.connection.controllerL298N.in2, 0);
            rmPinsDriver->Write(config.connection.controllerL298N.en, 0);
        }
    }
}

void RmEngineYellow::GetSpeed()
{ // For Future use
}

void RmEngineYellow::pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{ // For Future use
}
