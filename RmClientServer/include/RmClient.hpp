#pragma once
#include <Arduino.h>
#include <esp_event.h>

class RmClient
{
public:
    RmClient(String host, uint16_t port);
    void Begin();
    //    void Reconnect();

private:
    String host;
    uint16_t port;

    static void commandReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
};

extern RmClient *rmClient;