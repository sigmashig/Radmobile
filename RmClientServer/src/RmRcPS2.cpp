#include "RmRcPS2.hpp"
#include "RmConfiguration.hpp"
#include <esp_event.h>
#include "RmTypes.hpp"

RmRcPS2::RmRcPS2()
{
    psx.setupPins(rmConfig->ps2Config.pinData, rmConfig->ps2Config.pinCmd,
                  rmConfig->ps2Config.pinAtt, rmConfig->ps2Config.pinClock, 10);
    psx.config(PSXMODE_ANALOG);
}

void RmRcPS2::Begin()
{
    Serial.println("RmRcPS2::Begin");
    psx.read(lastData);
    esp_event_handler_register(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, loopEventHandler, NULL);
}

void RmRcPS2::loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    PSX::PSXDATA psData;
    // Serial.println("RmRcPS2::loopEventHandler");
    if (PS2->psx.read(psData) == PSXERROR_SUCCESS)
    {
        // Serial.println("PS2 joystick is connected");
        PS2->cmdProcessing(psData);
    }
    else
    {
        Serial.println("PS2 joystick is not connected");
    }
}

int RmRcPS2::stickToDirection(byte x)
{
    int res = 0;

    if (x < 0x7F)
    {
        res = (100 * (0x7F - x)) / 0x7F;
    }
    else if (x > 0x81)
    {
        res = -(100 * (x - 0x7F)) / 0x7F;
    }

    return res;
}

void RmRcPS2::cmdProcessing(PSX::PSXDATA psData)
{
    RmCommandPkg cmdPkg;
    // Serial.println("RmRcPS2::cmdProcessing");
    if (memcmp(&lastData, &psData, sizeof(PSX::PSXDATA)) == 0)
    {
        return;
    }
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
                cmdPkg.command = CMD_BACKWARD;
                cmdPkg.value = -res;
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = res;
            }
            else
            {
                cmdPkg.command = CMD_STOP;
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
            // No Sent STOP for left-right
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
                cmdPkg.command = CMD_BACKWARD;
                cmdPkg.value = -res;
            }
            else if (res > 0)
            {
                cmdPkg.command = CMD_FORWARD;
                cmdPkg.value = res;
            }
            else
            {
                cmdPkg.command = CMD_STOP;
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

        if (psData.buttons & PSXBTN_UP || psData.buttons & PSXBTN_DOWN)
        {
            if (lastData.buttons & PSXBTN_UP != psData.buttons & PSXBTN_UP || lastData.buttons & PSXBTN_DOWN != psData.buttons & PSXBTN_DOWN)
            {
                psData.buttons &PSXBTN_UP ? cmdPkg.command = CMD_FORWARD : cmdPkg.command = CMD_STOP;
                psData.buttons &PSXBTN_DOWN ? cmdPkg.command = CMD_BACKWARD : cmdPkg.command = CMD_STOP;
                cmdPkg.value = 100;
                CmdToServer(cmdPkg);
            }
        }
        if (psData.buttons & PSXBTN_LEFT || psData.buttons & PSXBTN_RIGHT)
        {
            if (lastData.buttons & PSXBTN_LEFT != psData.buttons & PSXBTN_LEFT || lastData.buttons & PSXBTN_RIGHT != psData.buttons & PSXBTN_RIGHT)
            {
                psData.buttons &PSXBTN_LEFT ? cmdPkg.command = CMD_LEFT : cmdPkg.command = CMD_RIGHT;
                cmdPkg.value = 100;
                CmdToServer(cmdPkg);
            }
        }
        break;
    }
    }
    if (rmConfig->ps2Config.modeStick != PS2ModeStick::PS2_PAD)
    {
        if (psData.buttons & PSXBTN_UP != lastData.buttons & PSXBTN_UP)
        {
            cmdPkg.command = CMD_BUTTON1;
            cmdPkg.value = psData.buttons & PSXBTN_UP ? 100 : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_DOWN != lastData.buttons & PSXBTN_DOWN)
        {
            cmdPkg.command = CMD_BUTTON2;
            cmdPkg.value = psData.buttons & PSXBTN_DOWN ? 100 : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_LEFT != lastData.buttons & PSXBTN_LEFT)
        {
            cmdPkg.command = CMD_BUTTON3;
            cmdPkg.value = psData.buttons & PSXBTN_LEFT ? 100 : 0;
            CmdToServer(cmdPkg);
        }
        if (psData.buttons & PSXBTN_RIGHT != lastData.buttons & PSXBTN_RIGHT)
        {
            cmdPkg.command = CMD_BUTTON4;
            cmdPkg.value = psData.buttons & PSXBTN_RIGHT ? 100 : 0;
            CmdToServer(cmdPkg);
        }
    }
    if ((psData.buttons & PSXBTN_L1) != (lastData.buttons & PSXBTN_L1))
    {
        Serial.println("L1");
        cmdPkg.command = CMD_BUTTON5;
        cmdPkg.value = psData.buttons & PSXBTN_L1 ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_L2) != (lastData.buttons & PSXBTN_L2))
    {
        Serial.println("L2");
        cmdPkg.command = CMD_BUTTON6;
        cmdPkg.value = psData.buttons & PSXBTN_L2 ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_TRIANGLE) != (lastData.buttons & PSXBTN_TRIANGLE))
    {
        cmdPkg.command = CMD_BUTTON9;
        cmdPkg.value = psData.buttons & PSXBTN_TRIANGLE ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_CROSS) != (lastData.buttons & PSXBTN_CROSS))
    {
        cmdPkg.command = CMD_BUTTON10;
        cmdPkg.value = psData.buttons & PSXBTN_CROSS ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_SQUARE) != (lastData.buttons & PSXBTN_SQUARE))
    {
        cmdPkg.command = CMD_BUTTON11;
        cmdPkg.value = psData.buttons & PSXBTN_SQUARE ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_CIRCLE) != (lastData.buttons & PSXBTN_CIRCLE))
    {
        cmdPkg.command = CMD_BUTTON12;
        cmdPkg.value = psData.buttons & PSXBTN_CIRCLE ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_R1) != (lastData.buttons & PSXBTN_R1))
    {
        cmdPkg.command = CMD_BUTTON7;
        cmdPkg.value = psData.buttons & PSXBTN_R1 ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_R2) != (lastData.buttons & PSXBTN_R2))
    {
        cmdPkg.command = CMD_BUTTON8;
        cmdPkg.value = psData.buttons & PSXBTN_R2 ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_START) != (lastData.buttons & PSXBTN_START))
    {
        cmdPkg.command = CMD_START;
        cmdPkg.value = psData.buttons & PSXBTN_START ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_ACT_RIGHT) != (lastData.buttons & PSXBTN_ACT_RIGHT))
    {
        cmdPkg.command = CMD_ACTR;
        cmdPkg.value = psData.buttons & PSXBTN_ACT_RIGHT ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    if ((psData.buttons & PSXBTN_ACT_LEFT) != (lastData.buttons & PSXBTN_ACT_LEFT))
    {
        cmdPkg.command = CMD_ACTL;
        cmdPkg.value = psData.buttons & PSXBTN_ACT_LEFT ? 100 : 0;
        CmdToServer(cmdPkg);
    }
    memcpy(&lastData, &psData, sizeof(psData));
}

RmRcPS2 *PS2;