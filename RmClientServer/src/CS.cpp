#include <Arduino.h>
#include <esp_event.h>
#if COMMUNICATIONS == 1
#include <WiFi.h>
#elif COMMUNICATIONS == 2
#endif

#if MODE == 1
#include "RmServer.hpp"
#elif MODE == 2
#include "RmClient.hpp"
#endif

#include "RmRemoteControl.hpp"
#if RC == 1
#include "RmRcEmulator.hpp"
#endif

#ifndef WIFI_SSID
#define WIFI_SSID "Please_define"
#endif

#ifndef WIFI_PWD
#define WIFI_PWD "Please_define"
#endif

bool isSetupReady = false;
bool isWifiConnected = false;

void onWiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
{
  switch (event)
  {
  case SYSTEM_EVENT_STA_GOT_IP:
  {
    isWifiConnected = true;
    Serial.println("WiFi connected");
    Serial.printf(F("IP address: %s\n"), WiFi.localIP().toString().c_str());
#if MODE == 1
    Serial.println("Starting RmServer");
    rmServer->Begin();
    remoteControl->Begin();
#elif MODE == 2
    Serial.println("Starting RmClient");
    rmClient->Begin();
#endif
    Serial.println("Setup ready\n");
    isSetupReady = true;
    break;
  }
  case SYSTEM_EVENT_STA_DISCONNECTED:
  {
    Serial.printf(F("WiFi connection error: %d\n"), info.wifi_sta_disconnected.reason);
    if (isWifiConnected) {
#if MODE == 1
    Serial.println("Reconnecting RmServer");
    rmServer->Reconnect();
#elif MODE == 2
    Serial.println("Reconnecting RmClient");
    rmClient->Reconnect();
#endif
    }
  }
  break;
  }
}

void startWiFi(String ssid, String password)
{

  Serial.printf(F("Connecting to WiFi network: %s(%s)\n"), ssid.c_str(), password.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(onWiFiEvent);
  WiFi.begin(ssid.c_str(), password.c_str());
}

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------");

#if COMMUNICATIONS == 1 // WiFi
#if MODE == 1
  rmServer = new RmServer();
#elif MODE == 2
  rmClient = new RmClient(String(SERVER_URL), SERVER_PORT);
#endif

#if RC == 1
  remoteControl = new RmRcEmulator();
#endif
  //WiFi.onEvent(onWiFiEvent);
  startWiFi(WIFI_SSID, WIFI_PWD);
  Serial.println("WiFi starting...");
#elif COMMUNICATIONS == 2
#endif
}

void loop()
{
  if (isSetupReady)
  {
#if MODE == 1
    if (rmServer != NULL)
    {
      rmServer->Loop();
    }
#elif MODE == 2
    if (rmClient != NULL)
    {
      rmClient->Loop();
    }
#endif
  }
  // vTaskDelete(NULL);
  // vTaskDelay(1000 / portTICK_PERIOD_MS);
}
