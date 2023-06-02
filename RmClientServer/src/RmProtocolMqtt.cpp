#include "RmProtocolMqtt.hpp"
#include <WiFi.h>
#include <esp_event.h>
#include "SigmaLoger.hpp"
#include "RmCommands.hpp"
#include "RmConfiguration.hpp"

String RmProtocolMqtt::rootTopic = "RadMobile";
AsyncMqttClient RmProtocolMqtt::mqttClient;

void RmProtocolMqtt::Begin()
{
    connectToMqtt();
}

void RmProtocolMqtt::ReceivedState(String stateString)
{
    CommandState state;
    state = RmCommands::StringToState(stateString);
    if (state.isValid)
    {
        esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &state, sizeof(state), portMAX_DELAY);
    }
}

bool RmProtocolMqtt::SendCommand(String command)
{
    uint res = mqttClient.publish(topicName(0).c_str(), 0, false, command.c_str());
    return res != 0;
}

void RmProtocolMqtt::Reconnect()
{
    Log->Warn("Mqtt Reconnect");
    connectToMqtt();
}

RmProtocolMqtt::RmProtocolMqtt()
{
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                 { 
                     Log->Append("WiFi event:(MQTT) ").Append((uint)event).Debug();
                     if ((uint)event == (uint)SYSTEM_EVENT_STA_GOT_IP)
                     {
                         Reconnect();
                     } });
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    Log->Append("MQTT_URL=").Append(MQTT_URL).Append(":").Append(MQTT_PORT).Debug();
    String clientId = "RM_" + String(rmConfig->Id, HEX);
    Log->Append("clientId=").Append(clientId).Debug();
    mqttClient.setClientId(clientId.c_str());
    mqttClient.setCleanSession(true);
    mqttClient.setKeepAlive(15);

    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { 
                                Log->Append("Disconnected from MQTT. Reason=").Append((uint)reason).Error(); 
                                connectToMqtt(); });
    mqttClient.onMessage(messageReceived);
    mqttClient.onConnect(_onConnect);
}

void RmProtocolMqtt::PublishLog(uint level, String payload)
{
    mqttClient.publish(topicName(2, level).c_str(), 0, false, payload.c_str());
}

bool RmProtocolMqtt::_onConnect(bool sessionPresent)
{
    rmProtocolMqtt->isReady = true;
    Log->Info(F("Connected to MQTT."));
    // mqttClient.subscribe(topic.c_str(), 0);
    return true;
}

void RmProtocolMqtt::connectToMqtt()
{
    mqttClient.connect();
}

String RmProtocolMqtt::topicName(int mode, uint level)
{
    if (mode == 0)
    { // Command
        return rootTopic + "/Command/";
    }
    else if (mode == 1)
    { // Log
        String topic = rootTopic + "/Logs/RM_" + String(rmConfig->Id, HEX);

        switch ((SigmaLogLevel)level)
        {
        case SIGMALOG_OFF:
            return topic + "/Off";
        case SIGMALOG_ERROR:
            return topic + "/Error";
        case SIGMALOG_WARN:
            return topic + "/Warn";
        case SIGMALOG_INFO:
            return topic + "/Info";
        case SIGMALOG_DEBUG:
            return topic + "/Debug";
        case SIGMALOG_FATAL:
            return topic + "/Fatal";
        case SIGMALOG_INTERNAL:
            return topic + "/Internal";
        case SIGMALOG_ALL:
            return topic + "/All";
        }
        return topic;
    }
    else if (mode == 2)
    { // State
        return rootTopic + "/Logs/RM_" + String(rmConfig->Id, HEX) + "/All";
    }
    return "";
}

void RmProtocolMqtt::messageReceived(char *topic, char *payload,
                                     AsyncMqttClientMessageProperties properties, size_t len,
                                     size_t index, size_t total)
{
    if (len > 0)
    {
        payload[len] = '\0';
        Log->Append(F("[")).Append(topic).Append(F("]:")).Append(payload).Debug();
        rmProtocolMqtt->ReceivedState(String(payload));
    }
    else
    {
        Log->Error(F("MQTT:Empty message received!"));
    }
}

RmProtocolMqtt *rmProtocolMqtt = NULL;
