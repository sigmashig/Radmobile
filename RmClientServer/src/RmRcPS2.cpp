#include "RmRcPS2.hpp"
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"
#include <esp_event.h>
#include "RmTypes.hpp"
#include "RmCommands.hpp"

RmRcPS2::RmRcPS2()
{
    psx.setupPins(rmConfig->ps2Config.pinData, rmConfig->ps2Config.pinCmd,
                  rmConfig->ps2Config.pinAtt, rmConfig->ps2Config.pinClock, 10);
    psx.config(PSXMODE_ANALOG);
    timer = xTimerCreateStatic("PS2Timer", pdMS_TO_TICKS(100), pdTRUE, this, timerCallback, &timerPS2);
}

void RmRcPS2::timerCallback(TimerHandle_t xTimer)
{
    PSX::PSXDATA psData;
    if (PS2->psx.read(psData) == PSXERROR_SUCCESS)
    {
        PS2->cmdProcessing(psData);
    }
    else
    {
        Log->Error(F("PS2 joystick is not connected"));
    }
}

void RmRcPS2::Begin()
{
    psx.read(lastData);
    xTimerStart(timer, 0);
 }

int RmRcPS2::stickToDirection(byte x)
{
    int res = 0;

    if (x <= 0x7F)
    {
        res = -(MAX_COMMAND_VALUE * (0x7F - x)) / 0x7F;
    }
    else if (x >= 0x81)
    {
        res = (MAX_COMMAND_VALUE * (x - 0x7F)) / 0x7F;
    }

    return res;
}

void RmRcPS2::cmdProcessing(PSX::PSXDATA psData)
{
    RmCommandPkg cmdPkg;
    if (memcmp(&lastData, &psData, sizeof(PSX::PSXDATA)) == 0)
    {
        return;
    }
    Log->Debug("PS2 Start");
    switch (rmConfig->ps2Config.modeStick)
    {
    case PS2ModeStick::PS2_2x2:
    {
        int res;
        res = stickToDirection(psData.JoyLeftY);
        if (lastData.JoyLeftY != psData.JoyLeftY)
        {
            if (res < 0)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = -res;
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_BACKWARD;
                cmdPkg.value = res;
            }
            else
            {
                cmdPkg.command = CMD_PAUSE;
                cmdPkg.value = 0;
            }

            CmdToServer(cmdPkg);
        }
        res = stickToDirection(psData.JoyRightX);
        if (lastData.JoyRightX != psData.JoyRightX)
        {
            if (res < 0)
            {
                cmdPkg.command = CMD_LEFT;
                cmdPkg.value = -res;
                CmdToServer(cmdPkg);
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_RIGHT;
                cmdPkg.value = res;
                CmdToServer(cmdPkg);
            }
            else
            {
                cmdPkg.command = CMD_RIGHT;
                cmdPkg.value = 0;
                CmdToServer(cmdPkg);
            }
        }
        break;
    }
    case PS2ModeStick::PS2_4x4:
    {
        int res;
        res = stickToDirection(psData.JoyLeftY);
        if (lastData.JoyLeftY != psData.JoyLeftY)
        {
            if (res < 0)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = -res;
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_BACKWARD;
                cmdPkg.value = res;
            }
            else
            {
                cmdPkg.command = CMD_PAUSE;
                cmdPkg.value = 0;
            }
            CmdToServer(cmdPkg);
        }
        res = stickToDirection(psData.JoyLeftX);
        if (lastData.JoyLeftX != psData.JoyLeftX)
        {
            if (res < 0)
            {
                cmdPkg.command = CMD_LEFT;
                cmdPkg.value = -res;
                CmdToServer(cmdPkg);
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_RIGHT;
                cmdPkg.value = res;
                CmdToServer(cmdPkg);
            }
            // No Sent STOP for left-right
        }
        break;
    }
    case PS2ModeStick::PS2_PAD:
    {
        if ((psData.buttons & PSXBTN_UP) != (lastData.buttons & PSXBTN_UP))
        {
            if (psData.buttons & PSXBTN_UP)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = MAX_COMMAND_VALUE;
            }
            else
            {
                cmdPkg.command = CMD_PAUSE;
                cmdPkg.value = 0;
            }
            CmdToServer(cmdPkg);
        }
        if ((psData.buttons & PSXBTN_DOWN) != (lastData.buttons & PSXBTN_DOWN))
        {
            if (psData.buttons & PSXBTN_DOWN)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = MAX_COMMAND_VALUE;
            }
            else
            {
                cmdPkg.command = CMD_PAUSE;
                cmdPkg.value = 0;
            }
            CmdToServer(cmdPkg);
        }
        if ((psData.buttons & PSXBTN_LEFT) != (lastData.buttons & PSXBTN_LEFT))
        {
            cmdPkg.command = CMD_LEFT;
            if (psData.buttons & PSXBTN_LEFT)
            {
                cmdPkg.value = MAX_COMMAND_VALUE;
            }
            else
            {
                cmdPkg.value = 0;
            }
            CmdToServer(cmdPkg);
        }
        if ((psData.buttons & PSXBTN_RIGHT) != (lastData.buttons & PSXBTN_RIGHT))
        {
            cmdPkg.command = CMD_RIGHT;
            if (psData.buttons & PSXBTN_RIGHT)
            {
                cmdPkg.value = MAX_COMMAND_VALUE;
            }
            else
            {
                cmdPkg.value = 0;
            }
            CmdToServer(cmdPkg);
        }

        break;
    }
    }
    if (rmConfig->ps2Config.modeStick != PS2ModeStick::PS2_PAD)
    {
        if (psData.buttons & PSXBTN_UP != lastData.buttons & PSXBTN_UP)
        {
            cmdPkg.command = CMD_BUTTON1;
            cmdPkg.value = psData.buttons & PSXBTN_UP ? MAX_COMMAND_VALUE : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_DOWN != lastData.buttons & PSXBTN_DOWN)
        {
            cmdPkg.command = CMD_BUTTON2;
            cmdPkg.value = psData.buttons & PSXBTN_DOWN ? MAX_COMMAND_VALUE : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_LEFT != lastData.buttons & PSXBTN_LEFT)
        {
            cmdPkg.command = CMD_BUTTON3;
            cmdPkg.value = psData.buttons & PSXBTN_LEFT ? MAX_COMMAND_VALUE : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_RIGHT != lastData.buttons & PSXBTN_RIGHT)
        {
            cmdPkg.command = CMD_BUTTON4;
            cmdPkg.value = psData.buttons & PSXBTN_RIGHT ? MAX_COMMAND_VALUE : 0;
            CmdToServer(cmdPkg);
        }
    }
    // -------------------------- Buttons --------------------------
    // Special command
    uint specialMask = (PSXBTN_TRIANGLE | PSXBTN_CIRCLE | PSXBTN_ACT_LEFT);

    if (((psData.buttons & specialMask) == specialMask) && ((lastData.buttons & specialMask) != specialMask))
    {
        Log->Debug("Special command");
        cmdPkg.command = CMD_SPECIAL1;
        cmdPkg.value = psData.buttons & PSXBTN_SELECT & PSXBTN_CIRCLE & PSXBTN_ACT_LEFT ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }

    if ((psData.buttons & PSXBTN_L1) != (lastData.buttons & PSXBTN_L1))
    {
        cmdPkg.command = CMD_BUTTON5;
        cmdPkg.value = psData.buttons & PSXBTN_L1 ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_L2) != (lastData.buttons & PSXBTN_L2))
    {
        cmdPkg.command = CMD_BUTTON6;
        cmdPkg.value = psData.buttons & PSXBTN_L2 ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_TRIANGLE) != (lastData.buttons & PSXBTN_TRIANGLE))
    {
        cmdPkg.command = CMD_BUTTON9;
        cmdPkg.value = psData.buttons & PSXBTN_TRIANGLE ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_CROSS) != (lastData.buttons & PSXBTN_CROSS))
    {
        cmdPkg.command = CMD_BUTTON10;
        cmdPkg.value = psData.buttons & PSXBTN_CROSS ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_SQUARE) != (lastData.buttons & PSXBTN_SQUARE))
    {
        cmdPkg.command = CMD_BUTTON11;
        cmdPkg.value = psData.buttons & PSXBTN_SQUARE ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_CIRCLE) != (lastData.buttons & PSXBTN_CIRCLE))
    {
        cmdPkg.command = CMD_BUTTON12;
        cmdPkg.value = psData.buttons & PSXBTN_CIRCLE ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_R1) != (lastData.buttons & PSXBTN_R1))
    {
        cmdPkg.command = CMD_BUTTON7;
        cmdPkg.value = psData.buttons & PSXBTN_R1 ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_R2) != (lastData.buttons & PSXBTN_R2))
    {
        cmdPkg.command = CMD_BUTTON8;
        cmdPkg.value = psData.buttons & PSXBTN_R2 ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_START) != (lastData.buttons & PSXBTN_START))
    {
        cmdPkg.command = CMD_START;
        cmdPkg.value = psData.buttons & PSXBTN_START ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_SELECT) != (lastData.buttons & PSXBTN_SELECT))
    {
        cmdPkg.command = CMD_STOP;
        cmdPkg.value = psData.buttons & PSXBTN_SELECT ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_ACT_RIGHT) != (lastData.buttons & PSXBTN_ACT_RIGHT))
    {
        cmdPkg.command = CMD_BUTTON15;
        cmdPkg.value = psData.buttons & PSXBTN_ACT_RIGHT ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_ACT_LEFT) != (lastData.buttons & PSXBTN_ACT_LEFT))
    {
        cmdPkg.command = CMD_BUTTON16;
        cmdPkg.value = psData.buttons & PSXBTN_ACT_LEFT ? MAX_COMMAND_VALUE : 0;
        CmdToServer(cmdPkg);
    }
    memcpy(&lastData, &psData, sizeof(psData));
    Log->Debug("PS2 Finsihed");
}

RmRcPS2 *PS2;