#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"
#include <PCF8575.h>
#include <esp_event.h>

ESP_EVENT_DECLARE_BASE(RMPINS_DRIVER_EVENT);
typedef enum
{
    RM_PINS_DRIVER_ISR
} RmPinsDriverEvent;

class RmPinsDriver
{
public:
    typedef enum
    {
        PIN_INPUT,
        PIN_OUTPUT
    } PinMode;
    RmPinsDriver(PcfSettings *pcfSettings, int numbPcf);
    void Begin();
    void SetPinMode(PinDefinition pinDefinition, PinMode pinMode);
    void RegisterPin(PinDefinition pinDefinition);
    void Write(PinDefinition pinDefinition, uint value);
    uint Read(PinDefinition pinDefinition);

private:
    int numbPcfs;
    PCF8575 **pcfs;
    uint normalizeValue(uint value, PinDefinition pinDefinition);
};

extern RmPinsDriver *rmPinsDriver;