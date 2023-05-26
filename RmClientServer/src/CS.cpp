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

static void loopEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
  Serial.printf("MAIN loopEventHandler:%d\n", event_id);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");
  esp_event_loop_create_default();
  rmConfig = new RmConfiguration();
#if MODE == 1
  rmServer = new RmServer();
#elif MODE == 2
  rmClient = new RmClient();
#endif
  //esp_event_handler_register(RMCONFIG_EVENT, RMCONFIG_EVENT_LOOP, loopEventHandler, NULL);
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
    //Serial.println("MAIN loop");
    rmConfig->Loop();
  }
  vTaskDelay(50 / portTICK_PERIOD_MS);
}
