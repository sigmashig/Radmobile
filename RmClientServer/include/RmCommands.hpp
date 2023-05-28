#pragma once
#include <Arduino.h>

typedef enum
{
    CMD_START,
    CMD_FORWARD,
    CMD_BACKWARD,
    CMD_LEFT,
    CMD_RIGHT,
    CMD_STOP,
    CMD_PAUSE,
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
    CMD_ACTR,
    CMD_ACTL,
    CMD_NOCOMMAND
} RmCommandType;

typedef struct
{
    RmCommandType command;
    int value;
} RmCommandPkg;

class RmCommands
{
public:
    RmCommands();
    static String CommandToString(RmCommandPkg command);
    static String cmdTxt[CMD_NOCOMMAND + 1];
    static RmCommandPkg StringToCommand(String commandString);
};

extern RmCommands *rmCommands;