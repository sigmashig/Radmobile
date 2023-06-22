#pragma once
#include <Arduino.h>
#include <esp_event.h>
#include "RmTypes.hpp"

#define CHARSET_BEGIN 0x20
#define CHARSET_END 0x7E

class RmConfiguration
{
public:
    // Application configuration
    uint64_t BoardId = ESP.getEfuseMac();
    IdConfig CommunicationId = {.id = 0, .key = {0}};
    // Hardware configuration
#if MODE == 1 // Server
    // ---------------------------------------------------------------------
    // SERVER
    // Exchange Protocol configuration
    // #define I2C_ADDRESS 0x27
    // #define I2C_BEGIN 50
    // #define I2C_END 65
    I2CParams i2cParms = {.address = 0x27, .pWire = NULL, .sda = 0, .scl = 0};
#define NUMBER_PORT_EXPANDERS 2
    PortExpander portExpanders[NUMBER_PORT_EXPANDERS] = {
        {.drvCode = SIGMAIO_GPIO, .beg = 0, .end = 40},
        {.drvCode = SIGMAIO_PCF8575, .beg = 50, .end = 65, .params = &i2cParms}};

    UARTConfig xchngConfig = {
        .baudrate = 115200,
        .rx = 17,
        .tx = 16};

    //  Joystick PS2

    PS2Config ps2Config{
        .pinData = 15,
        .pinCmd = (0 + 50),
        .pinAtt = (2 + 50),
        .pinClock = (1 + 50),
        .modeStick = PS2ModeStick::PS2_2x2};
    LoraConfig loraConfig = {
        .type = LORA_SX1278,
        .connection = {.LoraSx1278 = {
                           .nss = 32,
                           .dio0 = 33,
                           .nrst = 27,
                           .dio1 = 3}}};
    // Dummy GPS
    UARTConfig gpsConfig;
// Dummy PCF
#define NUMB_OF_CLIENT_PCF 1
    PcfSettings clientPcfs[NUMB_OF_CLIENT_PCF];
    // Dummy MPUSettings
    MPUSettings mpuSettings;
    // Dummy PID settings
    PidSettings pidSettings;
    // Dummy Vehicle
    VehicleConfig Vehicle;
//-----------------------------------------------------------------------
#elif MODE == 2
    // --------- CLIENT ---------------
    // dummy PS2
    PS2Config ps2Config;
    I2CParams i2cParms = {.address = 0x22, .pWire = NULL, .sda = 0, .scl = 0};
#define NUMBER_PORT_EXPANDERS 2
    PortExpander portExpanders[NUMBER_PORT_EXPANDERS] = {
        {.drvCode = SIGMAIO_GPIO, .beg = 0, .end = 40},
        {.drvCode = SIGMAIO_PCF8575, .beg = 50, .end = 65, .params = &i2cParms}};
    

    // #define I2C_ADDRESS 0x24
    // #define I2C_BEGIN 50
    // #define I2C_END 65
    UARTConfig xchngConfig = {
        .baudrate = 115200,
        .rx = 4, // crossed vs Server
        .tx = 35};

    LoraConfig loraConfig = {
        .type = LORA_SX1278,
        .connection = {.LoraSx1278 = {
                           .nss = 18,
                           .dio0 = 26,
                           .nrst = 23,
                           .dio1 = 33}}};

#define NUMB_OF_CLIENT_PCF 1
    PcfSettings clientPcfs[NUMB_OF_CLIENT_PCF] = {{.id = 0, .address = 0x24, .irqPin = 34}};

    PidSettings pidSettings = {.limitYaw = 10, .limitPitch = 10, .limitRoll = 10, .straightPeriod = 2000, .checkPeriod = 200};

    // Initial calibration values
    MPUSettings mpuSettings = {
        .accX = -775, .accY = -28, .accZ = 1095, .gyrX = 145, .gyrY = -42, .gyrZ = -7, .address = 0x68};
    // GPS Settings
    UARTConfig gpsConfig = {
        .baudrate = 9600,
        .rx = 12,
        .tx = 15};
    // byte gpsRx = 12;
    // byte gpsTx = 15;
    //  ------Vehicle configuration---------
//    const uint pwmFreq = 5000;
//    const uint pwmResolution = 7;
#if VEHICLE == 0
    VehicleConfig Vehicle;
#elif VEHICLE == 1
#define ENGINE_MIN_VALUE 0
#define ENGINE_MAX_VALUE 99
EngineConfig engineFL = {
        .minPower = ENGINE_MIN_VALUE,
        .maxPower = ENGINE_MAX_VALUE,
        .controllerType = EngineControllerType::QS909,
        .connection = {.controllerQS909 = {.vr = 35, .zf = 54, .signal = 65, .enable = 50}}};
EngineConfig engineFR = {
    .minPower = ENGINE_MIN_VALUE,
    .maxPower = ENGINE_MAX_VALUE,
    .controllerType = EngineControllerType::QS909,
    .connection = {.controllerQS909 = {.vr = 14, .zf = 55, .signal = 65, .enable = 51}}};
EngineConfig engineRL = {
    .minPower = ENGINE_MIN_VALUE,
    .maxPower = ENGINE_MAX_VALUE,
    .controllerType = EngineControllerType::QS909,
    .connection = {.controllerQS909 = {.vr = 13, .zf = 56, .signal = 65, .enable = 52}}};
EngineConfig engineRR = {
    .minPower = ENGINE_MIN_VALUE,
    .maxPower = ENGINE_MAX_VALUE,
    .controllerType = EngineControllerType::QS909,
    .connection = {.controllerQS909 = {.vr = 25, .zf = 57, .signal = 65, .enable = 53}}};

VehicleConfig Vehicle = {
    .limitSlowRun = 20,
    .frontLeft = engineFL,
    .frontRight = engineFR,
    .rearLeft = engineRL,
    .rearRight = engineRR};

/*
    VehicleConfig VehicleXX = {
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
*/
#elif VEHICLE == 2
#define ENGINE_MIN_VALUE 60
#define ENGINE_MAX_VALUE 99
    EngineConfig engineFL = {
        .minPower = ENGINE_MIN_VALUE,
        .maxPower = ENGINE_MAX_VALUE,
        .controllerType = EngineControllerType::QS909,
        .connection = {.controllerL298N = {.in1 = 58, .in2 = 62, .en = 26}}};

    EngineConfig engineFR = {
        .minPower = ENGINE_MIN_VALUE,
        .maxPower = ENGINE_MAX_VALUE,
        .controllerType = EngineControllerType::QS909,
        .connection = {.controllerL298N = {.in1 = 59, .in2 = 63, .en = 14}}};
    EngineConfig engineRL = {
        .minPower = ENGINE_MIN_VALUE,
        .maxPower = ENGINE_MAX_VALUE,
        .controllerType = EngineControllerType::QS909,
        .connection = {.controllerL298N = {.in1 = 60, .in2 = 64, .en = 12}}};
    EngineConfig engineRR = {
        .minPower = ENGINE_MIN_VALUE,
        .maxPower = ENGINE_MAX_VALUE,
        .controllerType = EngineControllerType::QS909,
        .connection = {.controllerL298N = {.in1 = 61, .in2 = 65, .en = 13}}};

    VehicleConfig Vehicle = {
        .limitSlowRun = 20,
        .frontLeft = engineFL,
        .frontRight = engineFR,
        .rearLeft = engineRL,
        .rearRight = engineRR};
/*
    VehicleConfig Vehicle = {
        .limitSlowRun = 20,
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
*/
#endif
#endif
    // Functionality
    RmConfiguration();
    void Begin();

private:
};

extern RmConfiguration *rmConfig;