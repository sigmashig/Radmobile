#include "RmPinsDriver.hpp"

RmPinsDriver::RmPinsDriver(PcfSettings *pcfSettings, int numbPcf)
{
    this->numbPcfs = numbPcf;
    pcfs = new PCF8575 *[numbPcf];
    for (int i = 0; i < numbPcf; i++)
    {
        pcfs[i] = new PCF8575(pcfSettings[i].address);
    }
}

void RmPinsDriver::Begin()
{
}

void RmPinsDriver::SetPinMode(PinDefinition pinDefinition, PinMode pMode)
{
    if (pinDefinition.pinDriver == PINDRV_PCF)
    {
        pcfs[pinDefinition.pinConfig.pinAddress.PIN_I2C.controllerId]
            ->pinMode(pinDefinition.pinConfig.pinAddress.PIN_I2C.port, pMode == PIN_INPUT ? INPUT : OUTPUT);
    }
    else
    {
        pinMode(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, pMode == PIN_INPUT ? INPUT : OUTPUT);
    }
}

void RmPinsDriver::SetPinType(PinDefinition pinDefinition, PinType pinType)
{
    if (pinDefinition.pinConfig.pinType == PIN_PWM)
    {
        if (pinDefinition.pinDriver == PINDRV_GPIO)
        {
            ledcSetup(pinDefinition.pinConfig.pwmSettings.channel,
                      pinDefinition.pinConfig.pwmSettings.frequency,
                      pinDefinition.pinConfig.pwmSettings.resolution);
            ledcAttachPin(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, pinDefinition.pinConfig.pwmSettings.channel);
        }
    }
}

void RmPinsDriver::Write(PinDefinition pinDefinition, uint value)
{
    if (pinDefinition.pinDriver == PINDRV_PCF)
    {
        pcfs[pinDefinition.pinConfig.pinAddress.PIN_I2C.controllerId]
            ->digitalWrite(pinDefinition.pinConfig.pinAddress.PIN_I2C.port, value);
    }
    else
    {
        if (pinDefinition.pinConfig.pinType == PIN_PWM)
        {
            ledcWrite(pinDefinition.pinConfig.pwmSettings.channel, value);
        }
        else if (pinDefinition.pinConfig.pinType == PIN_DIGITAL)
        {
            digitalWrite(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, value);
        } else {
            analogWrite(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, value);
        }
    }
}

uint RmPinsDriver::Read(PinDefinition pinDefinition)
{
    uint res;
    if (pinDefinition.pinDriver == PINDRV_PCF)
    {
        res = pcfs[pinDefinition.pinConfig.pinAddress.PIN_I2C.controllerId]
            ->digitalRead(pinDefinition.pinConfig.pinAddress.PIN_I2C.port);
    }
    else
    {
        if (pinDefinition.pinConfig.pinType == PIN_PWM)
        {
            res = ledcRead(pinDefinition.pinConfig.pwmSettings.channel);
        }
        else if (pinDefinition.pinConfig.pinType == PIN_DIGITAL)
        {
            res = digitalRead(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin);
        }
        else
        {
            res = analogRead(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin);
        }
    }

    return res;
}
