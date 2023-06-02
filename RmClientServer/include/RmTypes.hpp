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
        uint minvalue;
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
    DIRECTION_FORWARD,
    DIRECTION_BACKWARD,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_NODIRECTION,
    DIRECTION_START,
    DIRECTION_STOP
} Direction;
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

typedef enum
{
    CMD_START,
    CMD_STOP,
    CMD_FORWARD,
    CMD_BACKWARD,
    CMD_PAUSE,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_BUTTON1,
    CMD_BUTTON2,
    CMD_BUTTON3,
    CMD_BUTTON4,
    CMD_BUTTON5,
    CMD_BUTTON6,
    CMD_BUTTON7,
    CMD_BUTTON8,
    CMD_BUTTON9,
    CMD_BUTTON10,
    CMD_BUTTON11,
    CMD_BUTTON12,
    CMD_BUTTON13,
    CMD_BUTTON14,
    CMD_BUTTON15,
    CMD_BUTTON16,
    CMD_NOCOMMAND
} RmCommandType;

typedef struct
{
    RmCommandType command;
    int value;
} RmCommandPkg;

typedef union
{
    struct
    {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
        unsigned b9 : 1;
        unsigned b10 : 1;
        unsigned b11 : 1;
        unsigned b12 : 1;
        unsigned b13 : 1;
        unsigned b14 : 1;
        unsigned b15 : 1;
        unsigned b16 : 1;
    } button;
    uint16_t buttonPacked;
} ButtonsSet;

typedef struct
{
    bool isValid = true;
    Direction straight;
    int powerStraight;
    Direction turn;
    int powerTurn;
    ButtonsSet buttons;
} CommandState;

typedef struct
{
    int accX;
    int accY;
    int accZ;
    int gyrX;
    int gyrY;
    int gyrZ;
    byte address;
    byte irqPin;
} MPUSettings;

typedef enum
{
    RMEVENT_NONE,
    RMEVENT_STATE_RECEIVED,
    RMEVENT_LORA_SOMETHING_HAPPENS,
    RMEVENT_PID_CORRECTION_ROLL,
    RMEVENT_PID_CORRECTION_PITCH,
    RMEVENT_PID_CORRECTION_LEFT,
    RMEVENT_PID_CORRECTION_RIGHT,
    RMEVENT_GPS_POSITION

} RmProtocolEvent;

typedef struct
{
    double latitude;
    double longitude;
    double altitude;
    double speed;
} GpsPosition;