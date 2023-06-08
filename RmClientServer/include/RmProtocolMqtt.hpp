#pragma once

#include <Arduino.h>
#include <RmProtocol.hpp>
#include <AsyncMqttClient.h>

class RmProtocolMqtt : public RmProtocol
{
public:
    void Begin();
    //void ReceivedPkg(String command);
    bool SendPkg(String command);
    void Reconnect();
    RmProtocolMqtt();
    void PublishLog(uint level, String payload);
    ~RmProtocolMqtt();

private:
    static AsyncMqttClient mqttClient;
    static String rootTopic;

    static bool _onConnect(bool sessionPresent);

    void connectToMqtt();
    String topicName(int mode, uint level = 0);
    static void messageReceived(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
};
extern RmProtocolMqtt *rmProtocolMqtt;
