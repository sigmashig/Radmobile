#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "RmClient.hpp"

void setup()
{
  Serial.begin(115200);
  Serial.println("---------------------");

#if COMMUNICATIONS == 1
  rmClient = new RmClient(String(SERVER),SERVER_PORT, WIFI_SSID, WIFI_PWD);
#elif COMMUNICATIONS == 2
#endif

  rmClient->Begin();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void loop()
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // vTaskDelete(NULL);
}
