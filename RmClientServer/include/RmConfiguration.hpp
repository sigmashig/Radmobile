#pragma once
#include <Arduino.h>
#include <esp_event.h>

ESP_EVENT_DECLARE_BASE(CONFIG_EVENT);
typedef enum
{
    CONFIG_EVENT_LOOP,
    CONFIG_EVENT_1_SECOND,
    CONFIG_EVENT_3_SECONDS,
    CONFIG_EVENT_5_SECONDS,
    CONFIG_EVENT_10_SECONDS,
    CONFIG_EVENT_30_SECONDS,
    CONFIG_EVENT_1_MINUTE,
    CONFIG_EVENT_3_MINUTES,
    CONFIG_EVENT_5_MINUTES,
    CONFIG_EVENT_10_MINUTES,
    CONFIG_EVENT_30_MINUTES
} ConfigEvent;

class Configuration
{
public:
    // Hardware configuration
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

    Configuration();
    void Loop();
    void Begin();

private:
    static ulong loopCounter;
    static ulong loop1SecCounter;
    static ulong lastLoop;
};

extern Configuration *config;