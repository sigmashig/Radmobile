#include "RmEngineQS909.hpp"
#include "RmTypes.hpp"
#include "RmClient.hpp"
#include <esp_event.h>
#include <SigmaIO.hpp>

RmEngineQS909::RmEngineQS909(EngineConfig config) : RmEngine(config)
{
    IOError err;
    sigmaIO->PinMode(config.connection.controllerQS909.vr, OUTPUT);
    sigmaIO->PinMode(config.connection.controllerQS909.zf, OUTPUT);
    sigmaIO->PinMode(config.connection.controllerQS909.enable, OUTPUT);
    sigmaIO->PinMode(config.connection.controllerQS909.signal, INPUT);
    err=sigmaIO->RegisterPwmPin(config.connection.controllerQS909.vr, 5000, 7, config.minPower, config.maxPower);
    if (err != IOError::SIGMAIO_SUCCESS)
    {
        Log->Append("RmEngineQS909 PWM registration:error=").Append((uint)err).Debug();
    }
    power = 0;
    direction = Direction::DIRECTION_NODIRECTION;
}

void RmEngineQS909::Begin()
{
    Run(Direction::DIRECTION_NODIRECTION, EngineAction::ACTION_STOP, 0);
}

void RmEngineQS909::Run(Direction direction, EngineAction action, uint power)
{
    IOError err;
    byte vr=0, en=0, zf=0;
    if ((direction != DIRECTION_FORWARD && direction != DIRECTION_BACKWARD) || action != ACTION_RUN || power <= config.minPower || power > config.maxPower)
    {
        action = EngineAction::ACTION_STOP;
    }
    this->power = power;
    if (action == EngineAction::ACTION_STOP)
    {
        this->power = 0;
        direction = Direction::DIRECTION_NODIRECTION;
        vr = 0;
        en = 0;
        zf = 1;
    }
    else
    {
        if (direction == Direction::DIRECTION_FORWARD)
        {
            zf = 1;
        }
        else
        {
            zf = 0;
        }
        if (action == EngineAction::ACTION_RUN)
        {
            vr = power;
            en = 1;
        }
    }
    err=sigmaIO->SetPwm(config.connection.controllerQS909.vr, vr);
    if (err != IOError::SIGMAIO_SUCCESS)
    {
        Log->Append("RmEngineQS909 VR write:error=").Append((uint)err).Debug();
    }
    sigmaIO->DigitalWrite(config.connection.controllerQS909.zf, zf);
    sigmaIO->DigitalWrite(config.connection.controllerQS909.enable, en);
    Log->Append("Eng:").Append(GetId()).Append("; en=").Append(en).Append("; vr=").Append(vr).Append("; zf=").Append(zf).Debug();
}

uint RmEngineQS909::GetSpeed()
{ // For Future use
    return 0;
}

/*
void RmEngineJY01::pcfEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{ // For Future use
}
*/
