#include <Arduino.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"

#include <RadioLib.h>

#if MODE == 1
#include "RmServer.hpp"
#elif MODE == 2
#include "RmClient.hpp"
#endif

bool isReady = false;

ESP_EVENT_DECLARE_BASE(RMPROTOCOL_EVENT);
#if MODE == 1
ESP_EVENT_DECLARE_BASE(RMRC_EVENT);
#elif MODE == 2
ESP_EVENT_DECLARE_BASE(RMPINS_DRIVER_EVENT);
ESP_EVENT_DECLARE_BASE(RMVEHICLE_EVENT);
#endif

static void totalEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
  // Serial.printf("MAIN loopEventHandler:%d\n", event_id);
  if (event_base == RMPROTOCOL_EVENT)
  {
    Log->Debug("MAIN RMPROTOCOL_EVENT");
  }
#if MODE == 1
  else if (event_base == RMRC_EVENT)
  {
    Log->Debug("MAIN RMRC_EVENT");
  }
#elif MODE == 2
  else if (event_base == RMPINS_DRIVER_EVENT)
  {
    Log->Debug("MAIN RM_PINS_DRIVER_EVENT");
  }
  else if (event_base == RMVEHICLE_EVENT)
  {
    Log->Debug("MAIN RMVEHICLE_EVENT");
  }
#endif
  else
  {
    Log->Append("MAIN event_base:").Append(event_base).Debug();
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");
  Log = new SigmaLoger(512);
  esp_event_loop_create_default();
  rmConfig = new RmConfiguration();
#if MODE == 1
  rmServer = new RmServer();
#elif MODE == 2
  rmClient = new RmClient();
#endif
  rmConfig->Id = ESP.getEfuseMac();
  Log->Printf("ID:%lx", rmConfig->Id).Info();
  // esp_event_handler_register(ESP_EVENT_ANY_BASE, ESP_EVENT_ANY_ID, totalEventHandler, NULL);
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
