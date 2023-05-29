#pragma once
#include <Arduino.h>
#include "RmTypes.hpp"

#define MAX_COMMAND_VALUE 99
class RmCommands
{
public:
    RmCommands();
    static String CommandToString(RmCommandPkg command);
    static char cmdTxt[CMD_NOCOMMAND + 1];
    //static RmCommandPkg StringToCommand(String commandString);
    static String StateAsString(CommandState &state);
    static CommandState StringToState(String stateString);
};

extern RmCommands *rmCommands;