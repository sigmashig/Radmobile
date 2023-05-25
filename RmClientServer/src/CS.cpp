#include <Arduino.h>
#include <esp_event.h>
#include "RmConfiguration.hpp"
#include <RadioLib.h>

#if MODE == 1
#include "RmServer.hpp"
#elif MODE == 2
#include "RmClient.hpp"
#endif

bool isReady = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");

  rmConfig = new RmConfiguration();
#if MODE == 1
  rmServer = new RmServer();
#elif MODE == 2
  rmClient = new RmClient();
#endif
}

void loop()
{
  if (!isReady)
  {
#if MODE == 1
    isReady = rmServer->IsReady();
#elif MODE == 2
    isReady = rmClient->IsReady();
#endif
  }
  if (isReady)
  {
    rmConfig->Loop();
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}
