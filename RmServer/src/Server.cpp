#include <Arduino.h>
#include <esp_event.h>
#include "RmServer.hpp"

#ifndef WIFI_SSID
#define WIFI_SSID "Please_define"
#endif

#ifndef WIFI_PWD
#define WIFI_PWD "Please_define"
#endif

void responseEventHandler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{

}

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");

  esp_event_handler_instance_register(RMSERVER_EVENT,RMEVENT_RESPONSE,
                                      responseEventHandler, NULL, NULL);
  
#if COMMUNICATIONS == 1 
  rmServer = new RmServer(WIFI_SSID, WIFI_PWD);
#elif COMMUNICATIONS == 2
#endif  
  rmServer->Begin();
}

void loop()
{
 // rmServer->loop();
 // vTaskDelete(NULL);
 vTaskDelay(1000 / portTICK_PERIOD_MS);
}
