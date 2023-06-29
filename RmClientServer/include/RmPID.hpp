#pragma once
#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <esp_event.h>

#define PID_STRAIGHT_PERIOD 1000
#define PID_CHECK_PERIOD 500

class RmPID
{
public:
    RmPID(double pitch, double roll, double yaw, int straightPeriod = PID_STRAIGHT_PERIOD, int checkPeriod = PID_CHECK_PERIOD);

private:
    double pitch;
    double roll;
    double yaw;
    int checkPeriod;
    int straightPeriod;
    bool isMPUavailable;
    bool isStarted; // the START command was received
    // bool isWorkingCircle; // PID is working and ready to send corrections. Will be reseted during turn
    TimerHandle_t pidTimerHandle;
    StaticTimer_t timerBuffer;
    enum
    {
        PID_STATE_IDLE,
        PID_STATE_CHECK_PERIOD,
        PID_STATE_STRAIGHT_PERIOD
    } pidState;
    struct
    {
        double yaw;
        double pitch;
        double roll;
    } angles;
    double lastYaw;
    MPU6050 *mpu;
    static void stateHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void getAngles();
    static void pidTimer(TimerHandle_t xTimer);
};

extern RmPID *rmPID;
