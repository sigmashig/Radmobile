#pragma once
#include <Arduino.h>

typedef enum
{
    CMD_J1_LEFT,
    CMD_J1_RIGHT,
    CMD_J1_FORWARD,
    CMD_J1_BACKWARD,
    CMD_J2_LEFT,
    CMD_J2_RIGHT,
    CMD_J2_FORWARD,
    CMD_J2_BACKWARD,
    CMD_PAD1_LEFT,
    CMD_PAD1_RIGHT,
    CMD_PAD1_FORWARD,
    CMD_PAD1_BACKWARD,
    CMD_PAD2_LEFT,
    CMD_PAD2_RIGHT,
    CMD_PAD2_FORWARD,
    CMD_PAD2_BACKWARD,
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
    CMD_NOCOMMAND
} CommandType;

typedef struct
{
    CommandType command;
    int value;
} CommandPkg;
