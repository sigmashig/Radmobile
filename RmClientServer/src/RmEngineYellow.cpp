#include "RmEngineYellow.hpp"
#include "RmTypes.hpp"
#include "RmClient.hpp"
#include <esp_event.h>
#include <SigmaIO.hpp>

RmEngineYellow::RmEngineYellow(EngineConfig config) : RmEngine(config)
{
    sigmaIO->PinMode(config.connection.controllerL298N.in1, OUTPUT);
    sigmaIO->PinMode(config.connection.controllerL298N.in2, OUTPUT);
    sigmaIO->PinMode(config.connection.controllerL298N.en, OUTPUT);
    sigmaIO->RegisterPwmPin(config.connection.controllerL298N.en, 5000, 7, config.minPower, config.maxPower);

    power = 0;
    direction = Direction::DIRECTION_NODIRECTION;
}

void RmEngineYellow::Begin()
{
    Run(Direction::DIRECTION_NODIRECTION, EngineAction::ACTION_STOP, 0);
}

void RmEngineYellow::Run(Direction direction, EngineAction action, uint power)
{
    if ((direction != DIRECTION_FORWARD && direction != DIRECTION_BACKWARD) || action != ACTION_RUN)
    {
        action = EngineAction::ACTION_STOP;
    }
    this->power = power;
    if (action == EngineAction::ACTION_STOP)
    {
        this->power = 0;
        direction = Direction::DIRECTION_NODIRECTION;
        sigmaIO->SetPwm(config.connection.controllerL298N.en, 0);
        sigmaIO->DigitalWrite(config.connection.controllerL298N.in1, 0);
        sigmaIO->DigitalWrite(config.connection.controllerL298N.in2, 0);
        return;
    }
    else
    {
        if (direction == Direction::DIRECTION_FORWARD)
        {
            sigmaIO->DigitalWrite(config.connection.controllerL298N.in1, 1);
            sigmaIO->DigitalWrite(config.connection.controllerL298N.in2, 0);
        }
        else
        {
            sigmaIO->DigitalWrite(config.connection.controllerL298N.in1, 0);
            sigmaIO->DigitalWrite(config.connection.controllerL298N.in2, 1);
        }
        if (action == EngineAction::ACTION_RUN)
        {
            sigmaIO->SetPwm(config.connection.controllerL298N.en, power);
        }
    }
}

uint RmEngineYellow::GetSpeed()
{ // For Future use
    return 0;
}

// void RmEngineYellow::pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
//{ // For Future use
// }
