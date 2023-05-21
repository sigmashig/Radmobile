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

    //SERVER
    // Joystick PS2
    static const byte PIN_PS2_DATA = 15;
    static const byte PIN_PS2_CMD = 16;
    static const byte PIN_PS2_ATT = 4;
    static const byte PIN_PS2_CLOCK = 2;
    typedef enum
    {
        PS2_2x2, // left stick: Forward/Backward, Right stick: Left/Right
        PS2_4x4, // left stick: 4 direction, Right stick: 4 direction
        PS2_PAD  // pad only. No analog stick
    } PS2ModeStick;
    static PS2ModeStick ModeStick;

    // CLIENT

    RmConfiguration();
    void Loop();
    void Begin();

private:
    static ulong loopCounter;
    static ulong loop1SecCounter;
    static ulong lastLoop;
};

extern RmConfiguration *config;