#pragma once
#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <esp_event.h>

#define PID_TIMER_PERIOD 2000

class RmPID
{
public:
    RmPID(double pitch, double roll, double yaw, int checkPeriod = PID_TIMER_PERIOD);

private:
    double pitch;
    double roll;
    double yaw;
    int checkPeriod;
    bool isMPUavailable;
    bool isStarted;       // the START command was received
    bool isWorkingCircle; // PID is working and ready to send corrections. Will be reseted during turn
    TimerHandle_t timer;
    StaticTimer_t timerBuffer;
    struct
    {
        double yaw;
        double pitch;
        double roll;
    } angles;
    double lastYaw;
    MPU6050 *mpu;
    static void cmdEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
    void getAngles();
    static void pidTimer(TimerHandle_t xTimer);
};

extern RmPID *rmPID;
