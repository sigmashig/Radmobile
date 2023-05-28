#include "RmCommands.hpp"

String RmCommands::cmdTxt[CMD_NOCOMMAND + 1];
RmCommands::RmCommands()
{
    cmdTxt[CMD_START] = "00";
    cmdTxt[CMD_FORWARD] = "F1";
    cmdTxt[CMD_BACKWARD] = "B1";
    cmdTxt[CMD_LEFT] = "L1";
    cmdTxt[CMD_RIGHT] = "R1";
    cmdTxt[CMD_STOP] = "XX";
    cmdTxt[CMD_PAUSE] = "PP";
    cmdTxt[CMD_BUTTON1] = "T1";
    cmdTxt[CMD_BUTTON2] = "T2";
    cmdTxt[CMD_BUTTON3] = "T3";
    cmdTxt[CMD_BUTTON4] = "T4";
    cmdTxt[CMD_BUTTON5] = "T5";
    cmdTxt[CMD_BUTTON6] = "T6";
    cmdTxt[CMD_BUTTON7] = "T7";
    cmdTxt[CMD_BUTTON8] = "T8";
    cmdTxt[CMD_BUTTON9] = "T9";
    cmdTxt[CMD_BUTTON10] = "TA";
    cmdTxt[CMD_BUTTON11] = "TB";
    cmdTxt[CMD_BUTTON12] = "TC";
    cmdTxt[CMD_NOCOMMAND] = "YY";
    cmdTxt[CMD_ACTR] = "AR";
    cmdTxt[CMD_ACTL] = "AL";
}

String RmCommands::CommandToString(RmCommandPkg command)
{
    String commandString;
    // command string format: "{L2#546}" means: command=CMD_J2_LEFT, value=546
    commandString = "{";
    commandString += cmdTxt[command.command];
    commandString += "#";
    commandString += command.value;
    commandString += "}";
    return commandString;
}
RmCommandPkg RmCommands::StringToCommand(String commandString)
{
    RmCommandPkg command;
    command.command = CMD_NOCOMMAND;
    if (commandString[0] == '{' && commandString[commandString.length() - 1] == '}' && commandString[cmdTxt[CMD_NOCOMMAND].length() + 1] == '#')
    {
        String cmd = commandString.substring(1, 3);
        for (int i = 0; i <= CMD_NOCOMMAND; i++)
        {
            if (cmdTxt[i] == cmd)
            {
                String strValue = commandString.substring(4, commandString.length() - 1);
                if (strValue.length() > 0 && strValue.length() <= 3)
                {
                    byte val = 0;
                    for (int j = 0; j < strValue.length(); j++)
                    {
                        if (strValue[j] >= '0' && strValue[j] <= '9')
                        {
                            val = val * 10 + (strValue[j] - '0');
                        }
                        else
                        {
                            // bad value,
                            return command;
                        }
                    }
                    if (val > 100)
                    { // bad value
                        return command;
                    }

                    command.command = (RmCommandType)i;
                    command.value = val;
                    break;
                }
            }
        }
    }
    return command;
}

//----------------------------------------------------
RmCommands *rmCommands;