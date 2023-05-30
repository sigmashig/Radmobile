#include "RmCommands.hpp"
#include "RmLoger.hpp"

String RmCommands::CommandToString(RmCommandPkg command)
{
    String commandString;
    // command string format: "{L2#546}" means: command=CMD_J2_LEFT, value=546
    commandString = "<";
    commandString += cmdTxt[command.command];
    commandString += "#";
    commandString += command.value;
    commandString += ">";
    return commandString;
}

String RmCommands::StateAsString(CommandState &state)
{
    String stateString;
    stateString += '(';
    switch (state.straight)
    {
    case DIRECTION_START:
        stateString += cmdTxt[CMD_START];
        break;
    case DIRECTION_FORWARD:
        stateString += cmdTxt[CMD_FORWARD];
        break;
    case DIRECTION_BACKWARD:
        stateString += cmdTxt[CMD_BACKWARD];
        break;
    case DIRECTION_STOP:
        stateString += cmdTxt[CMD_STOP];
        break;
    case DIRECTION_NODIRECTION:
        stateString += cmdTxt[CMD_PAUSE];
        break;
    }
    if (state.powerStraight < 10)
    {
        stateString += '0';
        stateString += String(state.powerStraight, DEC);
    }
    else
    {
        stateString += String(state.powerStraight, DEC);
    }
    stateString += '#';
    switch (state.turn)
    {
    case DIRECTION_LEFT:
        stateString += cmdTxt[CMD_LEFT];
        break;
    case DIRECTION_RIGHT:
        stateString += cmdTxt[CMD_RIGHT];
        break;
    case DIRECTION_NODIRECTION:
        stateString += cmdTxt[CMD_PAUSE];
        break;
    case DIRECTION_START:
        stateString += cmdTxt[CMD_START];
        break;
    case DIRECTION_STOP:
        stateString += cmdTxt[CMD_STOP];
        break;
    }
    if (state.powerTurn < 10)
    {
        stateString += '0';
        stateString += String(state.powerTurn, DEC);
    }
    else
    {
        stateString += String(state.powerTurn, DEC);
    }
    stateString += '#';
    for (int i = 0; i < 16; i++)
    {
        if ((state.buttons.buttonPacked & (1 << i)) != 0)
        {
            stateString += cmdTxt[CMD_BUTTON1 + i];
        }
    }
    stateString += ')';
    return stateString;
}

CommandState RmCommands::StringToState(String stateString)
{
    CommandState state;
    state.isValid = false;
    bool cont = true;
    if (stateString[0] == '(' && stateString[stateString.length() - 1] == ')')
    {
        if (stateString[1] == cmdTxt[CMD_START])
        {
            state.straight = DIRECTION_START;
            state.isValid = true;
        }
        else if (stateString[1] == cmdTxt[CMD_STOP])
        {
            state.straight = DIRECTION_STOP;
            state.isValid = true;
        }
        else if (stateString[1] == cmdTxt[CMD_FORWARD])
        {
            state.straight = DIRECTION_FORWARD;
            state.isValid = true;
        }
        else if (stateString[1] == cmdTxt[CMD_BACKWARD])
        {
            state.straight = DIRECTION_BACKWARD;
            state.isValid = true;
        }
        else if (stateString[1] == cmdTxt[CMD_PAUSE])
        {
            state.straight = DIRECTION_NODIRECTION;
            state.isValid = true;
        }
        if (state.isValid)
        {
            if (isDigit(stateString[2]) && isDigit(stateString[3]))
            {
                state.powerStraight = (stateString[2] - '0') * 10 + (stateString[3] - '0');
                state.isValid = true;
            }
            else
            {
                rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 2-3 not digits");
                state.isValid = false;
            }
            if (state.isValid)
            {
                if (stateString[4] == '#')
                {
                    if (stateString[5] == cmdTxt[CMD_START])
                    {
                        state.turn = DIRECTION_START;
                        state.isValid = true;
                    }
                    else if (stateString[5] == cmdTxt[CMD_STOP])
                    {
                        state.turn = DIRECTION_STOP;
                        state.isValid = true;
                    }
                    else if (stateString[5] == cmdTxt[CMD_LEFT])
                    {
                        state.turn = DIRECTION_LEFT;
                        state.isValid = true;
                    }
                    else if (stateString[5] == cmdTxt[CMD_RIGHT])
                    {
                        state.turn = DIRECTION_RIGHT;
                        state.isValid = true;
                    }
                    else if (stateString[5] == cmdTxt[CMD_PAUSE])
                    {
                        state.turn = DIRECTION_NODIRECTION;
                        state.isValid = true;
                    }
                    else
                    {
                        rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 5 unknown turn");
                        state.isValid = false;
                    }
                    if (state.isValid)
                    {
                        if (isDigit(stateString[6]) && isDigit(stateString[7]))
                        {
                            state.powerTurn = (stateString[6] - '0') * 10 + (stateString[7] - '0');
                            state.isValid = true;
                        }
                        else
                        {
                            rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 6-7 not digits");
                            state.isValid = false;
                        }
                        if (state.isValid)
                        {
                            if (stateString[8] == '#')
                            {
                                state.buttons.buttonPacked = 0;
                                for (int i = 9; i < stateString.length() - 2; i++)
                                {
                                    for (int j = 0; j < 16; j++)
                                    {
                                        if (stateString[i] == cmdTxt[CMD_BUTTON1 + j])
                                        {
                                            state.buttons.buttonPacked |= (1 << j);
                                        }
                                    }
                                }
                                state.isValid = true;
                            }
                            else
                            {
                                rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 8 is not #");
                                state.isValid = false;
                            }
                        }
                    }
                }
                else
                {
                    rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 4 is not #");
                    state.isValid = false;
                }
            }
        }
    }
    else
    {
        rmLoger->append("StateString: ").append(stateString).append(" is not valid.").Error("ERROR 0-last not ()");
        state.isValid = false;
    }
    return state;
}

char RmCommands::cmdTxt[CMD_NOCOMMAND + 1];
RmCommands::RmCommands()
{
    cmdTxt[CMD_START] = 'S';
    cmdTxt[CMD_STOP] = 'X';
    cmdTxt[CMD_FORWARD] = 'F';
    cmdTxt[CMD_BACKWARD] = 'B';
    cmdTxt[CMD_PAUSE] = 'P';
    cmdTxt[CMD_LEFT] = 'L';
    cmdTxt[CMD_RIGHT] = 'R';
    cmdTxt[CMD_BUTTON1] = 'a';
    cmdTxt[CMD_BUTTON2] = 'b';
    cmdTxt[CMD_BUTTON3] = 'c';
    cmdTxt[CMD_BUTTON4] = 'd';
    cmdTxt[CMD_BUTTON5] = 'e';
    cmdTxt[CMD_BUTTON6] = 'f';
    cmdTxt[CMD_BUTTON7] = 'g';
    cmdTxt[CMD_BUTTON8] = 'h';
    cmdTxt[CMD_BUTTON9] = 'i';
    cmdTxt[CMD_BUTTON10] = 'j';
    cmdTxt[CMD_BUTTON11] = 'k';
    cmdTxt[CMD_BUTTON12] = 'l';
    cmdTxt[CMD_BUTTON13] = 'm';
    cmdTxt[CMD_BUTTON14] = 'n';
    cmdTxt[CMD_BUTTON15] = 'o';
    cmdTxt[CMD_BUTTON16] = 'p';
    cmdTxt[CMD_NOCOMMAND] = '0';
}
/*

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
*/
//----------------------------------------------------
RmCommands *rmCommands;