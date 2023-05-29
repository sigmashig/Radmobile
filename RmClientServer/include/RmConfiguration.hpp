#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmPinsDriver.hpp"
#include "RmTypes.hpp"

ESP_EVENT_DECLARE_BASE(RMCONFIG_EVENT);
typedef enum
{
    RMCONFIG_EVENT_LOOP,
    RMCONFIG_EVENT_1_SECOND,
    RMCONFIG_EVENT_3_SECONDS,
    RMCONFIG_EVENT_5_SECONDS,
    RMCONFIG_EVENT_10_SECONDS,
    RMCONFIG_EVENT_30_SECONDS,
    RMCONFIG_EVENT_1_MINUTE,
    RMCONFIG_EVENT_3_MINUTES,
    RMCONFIG_EVENT_5_MINUTES,
    RMCONFIG_EVENT_10_MINUTES,
    RMCONFIG_EVENT_30_MINUTES
} RmConfigEvent;

class RmConfiguration
{
public:
    // Hardware configuration
    // SERVER
    //  Joystick PS2
    PS2Config ps2Config{
        .pinData = 15,
        .pinCmd = 16,
        .pinAtt = 4,
        .pinClock = 2,
        .modeStick = PS2ModeStick::PS2_2x2};
    LoraConfig srvLoraConfig = {
        .type = LORA_SX1278,
        .connection = {.LoraSx1278 = {
                           .nss = 32,
                           .dio0 = 33,
                           .nrst = 27,
                           .dio1 = 3}}};
    //-----------------------------------------------------------------------
    // CLIENT
    LoraConfig clientLoraConfig = {
        .type = LORA_SX1278,
        .connection = {.LoraSx1278 = {
                           .nss = 18,
                           .dio0 = 26,
                           .nrst = 23,
                           .dio1 = 33}}};

#define NUMB_OF_CLIENT_PCF 1
    PcfSettings clientPcfs[NUMB_OF_CLIENT_PCF] = {{.id = 0, .address = 0x24, .irqPin = 34}};
    const uint pwmFreq = 5000;
    const uint pwmResolution = 7;
#if VEHICLE == 0
    VehicleConfig Vehicle;
#elif VEHICLE == 1

    VehicleConfig Vehicle = {
        .frontLeft = {
            .controllerType = EngineControllerType::JY01,
            .connection = {
                .controllerJY01 = {
                    .vr = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 0}, .pinAddress = {.PIN_GPIO = {.pin = 2}}}},
                    .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x01}}}},
                    .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x02}}}},
                    .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x03}}}}}}},
        .frontRight = {.controllerType = EngineControllerType::JY01, .connection = {.controllerJY01 = {.vr = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 1}, .pinAddress = {.PIN_GPIO = {.pin = 13}}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x05}}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x06}}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x07}}}}}}},
        .rearLeft = {.controllerType = EngineControllerType::JY01, .connection = {.controllerJY01 = {.vr = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 2}, .pinAddress = {.PIN_GPIO = {.pin = 14}}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x09}}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x0A}}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x0B}}}}}}},
        .rearRight = {.controllerType = EngineControllerType::JY01, .connection = {.controllerJY01 = {.vr = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 3}, .pinAddress = {.PIN_GPIO = {.pin = 25}}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x0D}}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x0E}}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x0F}}}}}}},
        .r1 = {.type = RELAY_PULSE, .pin = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x08}}}}, .onLevel = LOW},
        .r2 = {.type = RELAY_TRIGGER, .pin = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x09}}}}, .onLevel = LOW}};
#elif VEHICLE == 2
#define ENGINE_MIN_VALUE 60
    VehicleConfig Vehicle = {
        .frontLeft = {
            .controllerType = EngineControllerType::L298N,
            .connection = {
                .controllerL298N = {
                    .in1 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x8}}}},
                    .in2 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0x9}}}},
                    .en = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 0, .minvalue = ENGINE_MIN_VALUE}, .pinAddress = {.PIN_GPIO = {.pin = 14}}}}}}},
        .frontRight = {.controllerType = EngineControllerType::L298N, .connection = {.controllerL298N = {.in1 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xA}}}}, .in2 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xB}}}}, .en = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 0, .minvalue = ENGINE_MIN_VALUE}, .pinAddress = {.PIN_GPIO = {.pin = 25}}}}}}},
        .rearLeft = {.controllerType = EngineControllerType::L298N, .connection = {.controllerL298N = {.in1 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xE}}}}, .in2 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xF}}}}, .en = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 0, .minvalue = ENGINE_MIN_VALUE}, .pinAddress = {.PIN_GPIO = {.pin = 2}}}}}}},
        .rearRight = {.controllerType = EngineControllerType::L298N, .connection = {.controllerL298N = {.in1 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xC}}}}, .in2 = {.pinDriver = PINDRV_PCF, .pinConfig = {.pinType = PIN_DIGITAL, .pinAddress = {.PIN_I2C = {.controllerId = 0, .port = 0xD}}}}, .en = {.pinDriver = PINDRV_GPIO, .pinConfig = {.pinType = PIN_PWM, .pwmSettings = {.frequency = pwmFreq, .resolution = pwmResolution, .channel = 0, .minvalue = ENGINE_MIN_VALUE}, .pinAddress = {.PIN_GPIO = {.pin = 13}}}}}}}};
#endif
    // Functionality
    RmConfiguration();
    void Loop();
    void Begin();

private:
    static ulong loopCounter;
    static ulong loop1SecCounter;
    static ulong lastLoop;
};

extern RmConfiguration *rmConfig;