#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"
#include <PCF8575.h>
#include <esp_event.h>

ESP_EVENT_DECLARE_BASE(RM_PINS_DRIVER_EVENT);
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
    /*
    typedef enum
    {
        PINDRV_NO,
        PINDRV_GPIO,
        PINDRV_PCF
    } PinDriver;
    typedef union
    {
        struct
        {
            byte controllerId;
            byte port;
        } PIN_I2C;
        struct
        {
            byte pin;
        } PIN_GPIO;
    } PinConfig;
    typedef struct
    {
        PinDriver pinDriver;
        PinType pinType;
        PinMode pinMode;
        PinConfig pinConfig;
    } PinDefinition;
*/
    RmPinsDriver(PcfSettings *pcfSettings, int numbPcf);
    void Begin();
    void SetPinMode(PinDefinition pinDefinition, PinMode pinMode);
    void SetPinType(PinDefinition pinDefinition, PinType pinType);

    void Write(PinDefinition pinDefinition, uint value);
    uint Read(PinDefinition pinDefinition);

private:
    int numbPcfs;
    PCF8575 **pcfs;

};