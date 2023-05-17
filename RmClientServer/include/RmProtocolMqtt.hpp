#pragma once

#include <Arduino.h>
#include <RmProtocol.hpp>
#include <AsyncMqttClient.h>

class RmProtocolMqtt : public RmProtocol
{
public:
    void Begin();
    void ReceivedCommand(String command);
    bool SendCommand(String command);
    //    void Reconnect();

private:
    static AsyncMqttClient mqttClient;
    static String topic;

    static bool _onConnect(bool sessionPresent);

    void connectToMqtt();

    static void messageReceived(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
};
