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
    for (int i = 0; i < numbPcfs; i++)
    {
        pcfs[i]->begin();
    }
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

void RmPinsDriver::RegisterPin(PinDefinition pinDefinition)
{
    if (pinDefinition.pinConfig.pinType == PIN_PWM)
    {
        if (pinDefinition.pinDriver == PINDRV_GPIO)
        {
            ledcSetup(pinDefinition.pinConfig.pwmSettings.channel,
                      pinDefinition.pinConfig.pwmSettings.frequency,
                      pinDefinition.pinConfig.pwmSettings.resolution);
            Serial.printf("Setup ledc channel=%d, freq=%d res=%d\n", pinDefinition.pinConfig.pwmSettings.channel, pinDefinition.pinConfig.pwmSettings.frequency, pinDefinition.pinConfig.pwmSettings.resolution);
            ledcAttachPin(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, pinDefinition.pinConfig.pwmSettings.channel);
            Serial.printf("Register pin %d as PWM channel %d\n", pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, pinDefinition.pinConfig.pwmSettings.channel);
        }
    }
}
uint RmPinsDriver::normalizeValue(uint value, PinDefinition pinDefinition)
{
    uint res;
    if (pinDefinition.pinConfig.pinType == PIN_PWM)
    {
        res = map(value > 100 ? 100 : value, 0, 100, 0, pow(2, pinDefinition.pinConfig.pwmSettings.resolution) - 1);
        Serial.printf("Norm: %d ->%d: %f\n", value, pinDefinition.pinConfig.pwmSettings.resolution, pow(2, pinDefinition.pinConfig.pwmSettings.resolution) - 1);
    }
    else if (pinDefinition.pinConfig.pinType == PIN_DIGITAL)
    {
        res = value > 0 ? HIGH : LOW;
    }
    else
    {
        res = value;
    }
    return res;
}

void RmPinsDriver::Write(PinDefinition pinDefinition, uint value)
{
    // Serial.printf("Write: %d\n", value);
    uint normValue = normalizeValue(value, pinDefinition);
    // Serial.printf("Normalized: %d\n", normValue);
    if (pinDefinition.pinDriver == PINDRV_PCF)
    { // PCF driver can process just HIGH or LOW
        pcfs[pinDefinition.pinConfig.pinAddress.PIN_I2C.controllerId]
            ->digitalWrite(pinDefinition.pinConfig.pinAddress.PIN_I2C.port, normValue);
    }
    else
    {
        if (pinDefinition.pinConfig.pinType == PIN_PWM)
        {
            Serial.printf("PWM ch=%d, value=%d\n", pinDefinition.pinConfig.pwmSettings.channel, normValue);
            // I have no idea why the AttachPin is needed here, but without it the PWM does not work
            ledcAttachPin(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, pinDefinition.pinConfig.pwmSettings.channel);

            ledcWrite(pinDefinition.pinConfig.pwmSettings.channel, normValue);
        }
        else if (pinDefinition.pinConfig.pinType == PIN_DIGITAL)
        {
            Serial.printf("Digital pin=%d, value=%d\n", pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, normValue);
            digitalWrite(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, normValue);
        }
        else
        {
            Serial.printf("Analog pin=%d, value=%d\n", pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, normValue);
            analogWrite(pinDefinition.pinConfig.pinAddress.PIN_GPIO.pin, normValue);
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

//------------------------------------------------------------------
RmPinsDriver *rmPinsDriver;