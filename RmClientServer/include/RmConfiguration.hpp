#pragma once
#include <Arduino.h>
#include <esp_event.h>

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
    typedef enum
    {
        PINDRV_GPIO,
        PINDRV_PCF
    } PinDriver;
    typedef struct {
        byte address;
        byte isrPin;
    } PcfController;
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
        PinConfig pinConfig;
    } PinDefinition;
    // SERVER
    //  Joystick PS2
    typedef struct
    {
        typedef enum
        {
            PS2_2x2, // left stick: Forward/Backward, Right stick: Left/Right
            PS2_4x4, // left stick: 4 direction, Right stick: 4 direction
            PS2_PAD  // pad only. No analog stick
        } PS2ModeStick;
        byte pinData;
        byte pinCmd;
        byte pinAtt;
        byte pinClock;
        PS2ModeStick modeStick;
    } PS2Config;
    PS2Config ps2Config{
        .pinData = 15,
        .pinCmd = 16,
        .pinAtt = 4,
        .pinClock = 2,
        .modeStick = PS2Config::PS2_2x2};
    // CLIENT
    typedef struct
    {
        typedef enum
        {
            JY01
        } EngineControllerType;
        typedef union
        {
            struct
            {
                PinDefinition vr;
                PinDefinition zf;
                PinDefinition signal;
                PinDefinition brake;
            } ControllerJY01;
        } EngineConnection;
        EngineControllerType controllerType;
        EngineConnection connection;
    } EngineConfig;
    typedef struct
    {
        EngineConfig frontLeft;
        EngineConfig frontRight;
        EngineConfig rearLeft;
        EngineConfig rearRight;
        PcfController controllers[1];

    } VehicleConfig;
    
#if VEHICLE == 1
    VehicleConfig Vehicle = {
        .frontLeft = {.controllerType = EngineConfig::JY01, .connection = {.ControllerJY01 = {.vr = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x00}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x01}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x02}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x03}}}}}},
        .frontRight = {.controllerType = EngineConfig::JY01, .connection = {.ControllerJY01 = {.vr = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x04}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x05}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x06}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x07}}}}}},
        .rearLeft = {.controllerType = EngineConfig::JY01, .connection = {.ControllerJY01 = {.vr = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x08}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x09}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0A}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0B}}}}}},
        .rearRight = {.controllerType = EngineConfig::JY01, .connection = {.ControllerJY01 = {.vr = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0C}}}, .zf = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0D}}}, .signal = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0E}}}, .brake = {.pinDriver = PINDRV_PCF, .pinConfig = {.PIN_I2C = {.controllerId = 0, .port = 0x0F}}}}}},
        .controllers = {{.address = 0x24, .isrPin = 4}}
    };
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