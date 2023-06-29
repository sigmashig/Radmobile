#include "RmPID.hpp"
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <esp_event.h>
#include "RmTypes.hpp"
#include <SigmaLoger.hpp>
#include "RmConfiguration.hpp"

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);

void RmPID::stateHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    Log->Debug(F("RmPID::stateReceived"));
    CommandState *state = (CommandState *)event_data;

    if (!rmPID->isStarted)
    {
        if (state->straight == DIRECTION_START)
        {
            rmPID->isStarted = true;
        }
    }
    else
    { // started
        if (state->straight == DIRECTION_STOP)
        {
            rmPID->isStarted = false;
        }
        else
        {
            if (state->powerTurn > 0 || state->powerStraight == 0)
            {
                Log->Debug("RmPID: turn or pause detected. Timer Stop");
                xTimerStop(rmPID->pidTimerHandle, 0);
                rmPID->pidState = PID_STATE_IDLE;
            }
            else
            {
                // straight moving
                if (rmPID->pidState == PID_STATE_IDLE)
                { // waiting for straight stabilization
                    rmPID->pidState = PID_STATE_STRAIGHT_PERIOD;
                    Log->Append("RmPID: straight detected. Timer change period to Straight:").Append(rmPID->straightPeriod).Debug();
                    xTimerChangePeriod(rmPID->pidTimerHandle, pdMS_TO_TICKS(rmPID->straightPeriod), 0);
                }
                else
                {
                    // check/straight period - do nothing
                }
            }
        }
    }
}
void RmPID::getAngles()
{
    if (isMPUavailable)
    {
        uint8_t fifoBuffer[64];
        if (mpu->dmpGetCurrentFIFOPacket(fifoBuffer))
        {
            Quaternion q;
            VectorFloat gravity;
            float ypr[3];
            // расчёты
            mpu->dmpGetQuaternion(&q, fifoBuffer);
            mpu->dmpGetGravity(&gravity, &q);
            mpu->dmpGetYawPitchRoll(ypr, &q, &gravity);

            angles.pitch = ypr[1] * 180 / M_PI;
            angles.roll = ypr[2] * 180 / M_PI;
            angles.yaw = ypr[0] * 180 / M_PI;

            if (rmPID->angles.roll >= rmPID->roll || rmPID->angles.roll <= -rmPID->roll)
            {
                double x = rmPID->angles.roll;
                esp_event_post(RMPROTOCOL_EVENT, RMEVENT_PID_CORRECTION_ROLL, &(rmPID->angles.roll), sizeof(rmPID->angles.roll), 0);
            }
            if (rmPID->angles.pitch >= rmPID->pitch || rmPID->angles.pitch <= -rmPID->pitch)
            {
                double x = rmPID->angles.pitch;
                esp_event_post(RMPROTOCOL_EVENT, RMEVENT_PID_CORRECTION_PITCH, &(rmPID->angles.pitch), sizeof(rmPID->angles.pitch), 0);
            }
        }
    }
}

void RmPID::pidTimer(TimerHandle_t xTimer)
{
    Log->Append(F("RmPID::pidTimer")).Append(F(" pidState=")).Append(rmPID->pidState).Debug();
    rmPID->getAngles();
    if (rmPID->pidState == PID_STATE_STRAIGHT_PERIOD)
    {
        // straight moving stabilized
        rmPID->lastYaw = rmPID->angles.yaw;
        rmPID->pidState = PID_STATE_CHECK_PERIOD;
        Log->Append("RmPID: straight stabilized. Timer change period to Check:").Append(rmPID->checkPeriod).Debug();
        xTimerChangePeriod(xTimer, pdMS_TO_TICKS(rmPID->checkPeriod), 0);
    }
    else
    { // check period - check yaw
        double d = rmPID->angles.yaw - rmPID->lastYaw;
        // rmPID->lastYaw = rmPID->angles.yaw;
        if (abs(d) >= rmPID->yaw)
        {
            int intensive = (abs(d) / rmPID->lastYaw);
            Log->Printf(F("RmPID::pidTimer: d=%f, intensive=%d "), d, intensive);
            Log->Printf(F(" yaw=%f, abs(d)=%f\n"), rmPID->yaw, abs(d)).Debug();
            // turn detected
            if (d > 0)
            {
                esp_event_post(RMPROTOCOL_EVENT, RMEVENT_PID_CORRECTION_LEFT, &intensive, sizeof(intensive), 0);
            }
            else
            {
                esp_event_post(RMPROTOCOL_EVENT, RMEVENT_PID_CORRECTION_RIGHT, &intensive, sizeof(intensive), 0);
            }
        }
        Log->Debug("RmPID: idle or straight stabilized. Timer Reset");
        xTimerReset(rmPID->pidTimerHandle, 0);
    }
}

RmPID::RmPID(double pitch, double roll, double yaw, int straightPeriod, int checkPeriod)
    : pitch(pitch), roll(roll), yaw(yaw), straightPeriod(straightPeriod), checkPeriod(checkPeriod)
{
    Log->Debug(F("RmPID::RmPID"));
    isStarted = false;
    pidState = PID_STATE_IDLE;
    pidTimerHandle = xTimerCreateStatic("PIDTimer", pdMS_TO_TICKS(checkPeriod), pdFALSE, 0, pidTimer, &timerBuffer);
    if (pidTimerHandle == NULL)
    {
        Log->Error(F("RmPID::RmPID: xTimerCreateStatic failed"));
    }
    else
    {
        Log->Debug(F("RmPID::RmPID: xTimerCreateStatic success"));
    }
    if (xTimerStart(pidTimerHandle, 0) != pdPASS)
    {
        Log->Error(F("RmPID::RmPID: xTimerStart failed"));
    }

    mpu = new MPU6050(rmConfig->mpuSettings.address);

    mpu->initialize();
    isMPUavailable = mpu->testConnection();
    if (!isMPUavailable)
    {
        Log->Error(F("MPU6050 connection failed"));
    }
    else
    {
        Log->Info(F("MPU6050 connection successful"));
    }
    byte devStatus;

    devStatus = mpu->dmpInitialize();
    mpu->setXAccelOffset(rmConfig->mpuSettings.accX);
    mpu->setYAccelOffset(rmConfig->mpuSettings.accY);
    mpu->setZAccelOffset(rmConfig->mpuSettings.accZ);
    mpu->setXGyroOffset(rmConfig->mpuSettings.gyrX);
    mpu->setYGyroOffset(rmConfig->mpuSettings.gyrY);
    mpu->setZGyroOffset(rmConfig->mpuSettings.gyrZ);

    if (devStatus == 0)
    {
        // Calibration Time: generate offsets and calibrate our MPU6050
        // mpu.CalibrateAccel(6);
        // mpu.CalibrateGyro(6);
        // mpu.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        mpu->setDMPEnabled(true);

        // disable Arduino interrupt detection
        mpu->setIntEnabled(0x00);
        // mpuIntStatus = mpu->getIntStatus();
    }
    else
    {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Log->Append(F("DMP Initialization failed (code=")).Append(devStatus).Append(F(")")).Error();
        isMPUavailable = false;
    }
    lastYaw = 0;
    esp_event_handler_instance_register(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, stateHandler, NULL, NULL);
}
// -------------------------------------------------
RmPID *rmPID = NULL;