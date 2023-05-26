#include <Arduino.h>
#include <esp_event.h>
#include "RmConfiguration.hpp"
#include <RadioLib.h>

#if MODE == SERVER
#include "RmServer.hpp"
#elif MODE == CLIENT
#include "RmClient.hpp"
#endif

bool isReady = false;

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");

  rmConfig = new RmConfiguration();
#if MODE == SERVER
  rmServer = new RmServer();
#elif MODE == CLIENT
  rmClient = new RmClient();
#endif
}

void loop()
{
  if (!isReady)
  {
#if MODE == SERVER
    isReady = rmServer->IsReady();
#elif MODE == CLIENT
    isReady = rmClient->IsReady();
#endif
  }
  if (isReady)
  {
    rmConfig->Loop();
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}
