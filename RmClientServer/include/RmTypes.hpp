#pragma once
#include <Arduino.h>

typedef enum
{
    PINDRV_NO,
    PINDRV_GPIO,
    PINDRV_PCF
} PinDriver;

typedef enum
{
    PIN_DIGITAL,
    PIN_ANALOG,
    PIN_PWM
} PinType;

typedef struct
{
    PinType pinType;
    struct {
        uint frequency;
        uint resolution;
        uint channel;
    } pwmSettings;
    union
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
    } pinAddress;
} PinConfig;
typedef struct
{
    PinDriver pinDriver;
    PinConfig pinConfig;
} PinDefinition;

typedef enum
{
    PS2_2x2, // left stick: Forward/Backward, Right stick: Left/Right
    PS2_4x4, // left stick: 4 direction, Right stick: 4 direction
    PS2_PAD  // pad only. No analog stick
} PS2ModeStick;

typedef struct
{
    byte pinData;
    byte pinCmd;
    byte pinAtt;
    byte pinClock;
    PS2ModeStick modeStick;
} PS2Config;

typedef enum
{
    ENGINE_NODIRECTION,
    ENGINE_FORWARD,
    ENGINE_BACKWARD
} EngineDirection;
typedef enum
{
    ACTION_STOP,
    ACTION_RUN
} EngineAction;

typedef struct
{
    typedef enum
    {
        JY01
    } EngineControllerType;
    union
    {
        struct
        {
            PinDefinition vr;
            PinDefinition zf;
            PinDefinition signal;
            PinDefinition brake;
        } controllerJY01;
    } connection;
    EngineControllerType controllerType;
} EngineConfig;

typedef struct
{
    EngineConfig frontLeft;
    EngineConfig frontRight;
    EngineConfig rearLeft;
    EngineConfig rearRight;
} VehicleConfig;

typedef struct
{
    byte id;
    byte address;
    byte irqPin;
} PcfSettings;