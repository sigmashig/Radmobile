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
    struct
    {
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
typedef enum
{
    JY01,
    L298N
} EngineControllerType;

typedef struct
{
    EngineControllerType controllerType;
    union
    {
        struct
        {
            PinDefinition vr;
            PinDefinition zf;
            PinDefinition signal;
            PinDefinition brake;
        } controllerJY01;
        struct
        {
            PinDefinition in1;
            PinDefinition in2;
            PinDefinition en;
        } controllerL298N;
    } connection;
} EngineConfig;
typedef enum
{
    RELAY_PULSE,
    RELAY_TRIGGER
} RelayType;

typedef struct
{
    RelayType type;
    PinDefinition pin;
    bool onLevel;
} RelayConfig;
typedef struct
{
    EngineConfig frontLeft;
    EngineConfig frontRight;
    EngineConfig rearLeft;
    EngineConfig rearRight;
    RelayConfig r1;
    RelayConfig r2;
} VehicleConfig;

typedef struct
{
    byte id;
    byte address;
    byte irqPin;
} PcfSettings;

typedef enum
{
    LORA_NO_LORA,
    LORA_SX1278
} LoraType;

typedef struct
{
    LoraType type;
    union
    {
        struct
        {
            // SX1278 has the following connections:
            // NSS pin:   10
            // DIO0 pin:  2
            // NRST pin:  9
            // DIO1 pin:  3
            byte nss;
            byte dio0;
            byte nrst;
            byte dio1;
        } LoraSx1278;
        struct
        {
            // SX1262 has the following connections:
            // NSS pin:   10
            // DIO1 pin:  2
            // NRST pin:  3
            // BUSY pin:  9
            byte nss;
            byte dio1;
            byte nrst;
            byte busy;
        } LoraSx1268;
    } connection;
} LoraConfig;