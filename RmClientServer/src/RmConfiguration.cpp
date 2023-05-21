#include "RmConfiguration.hpp"

RmConfiguration *config = NULL;
ESP_EVENT_DEFINE_BASE(RMCONFIG_EVENT);
RmConfiguration::PS2ModeStick RmConfiguration::ModeStick = RmConfiguration::PS2ModeStick::PS2_4x4;
ulong RmConfiguration::loopCounter;
ulong RmConfiguration::loop1SecCounter;
ulong RmConfiguration::lastLoop;

void RmConfiguration::Loop()
{
    esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, &loopCounter, sizeof(loopCounter), 1000 / portTICK_PERIOD_MS);
    if (millis() - lastLoop >= 1000)
    {
        esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_1_SECOND, &loop1SecCounter, sizeof(loop1SecCounter), 1000 / portTICK_PERIOD_MS);
        lastLoop = millis();
        ulong cnt;
        if (loop1SecCounter % 3 == 0)
        {
            cnt = loop1SecCounter / 3;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_3_SECONDS, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 5 == 0)
        {
            cnt = loop1SecCounter / 5;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_5_SECONDS, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 10 == 0)
        {
            cnt = loop1SecCounter / 10;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_10_SECONDS, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 30 == 0)
        {
            cnt = loop1SecCounter / 30;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_30_SECONDS, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 60 == 0)
        {
            cnt = loop1SecCounter / 60;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_1_MINUTE, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 180 == 0)
        {
            cnt = loop1SecCounter / 180;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_3_MINUTES, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 300 == 0)
        {
            cnt = loop1SecCounter / 300;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_5_MINUTES, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 600 == 0)
        {
            cnt = loop1SecCounter / 600;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_10_MINUTES, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        if (loop1SecCounter % 1800 == 0)
        {
            cnt = loop1SecCounter / 1800;
            esp_event_post(RMCONFIG_EVENT, RMCONFIG_EVENT_30_MINUTES, &cnt, sizeof(cnt), 1000 / portTICK_PERIOD_MS);
        }
        loop1SecCounter++;
    }
    loopCounter++;
}

void RmConfiguration::Begin()
{
}

RmConfiguration::RmConfiguration()
{
    loopCounter = 0;
    loop1SecCounter = 0;
    lastLoop = 0;
}
