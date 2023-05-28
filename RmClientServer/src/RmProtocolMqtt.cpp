#include "RmProtocolMqtt.hpp"
#include <esp_event.h>
#include "RmCommands.hpp"
// #include <WiFi.h>

String RmProtocolMqtt::topic = "RadMobile/Command";
AsyncMqttClient RmProtocolMqtt::mqttClient;

void RmProtocolMqtt::Begin()
{
    Serial.println("RmProtocolMqtt::Begin()");
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    String clientId = "RM_" + String(ESP.getEfuseMac(), HEX);
    Serial.println("Client ID: " + clientId);
    mqttClient.setClientId(clientId.c_str());
    mqttClient.setCleanSession(true);
    mqttClient.setKeepAlive(15);

    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { 
                                Serial.println("Disconnected from MQTT."); 
                                connectToMqtt(); });
    mqttClient.onMessage(messageReceived);
    mqttClient.onConnect(_onConnect);
    connectToMqtt();
}

void RmProtocolMqtt::ReceivedState(String stateString)
{
    Serial.println("RmProtocolMqtt::ReceivedCommand()");
    CommandState state;
    state = RmCommands::StringToState(stateString);
    if (state.isValid)
    {
        esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &state, sizeof(state), portMAX_DELAY);
    }   
}

bool RmProtocolMqtt::SendCommand(String command)
{
    Serial.println("RmProtocolMqtt::SendCommand()");
    uint res = mqttClient.publish(topic.c_str(), 0, false, command.c_str());
    return res != 0;
}

void RmProtocolMqtt::Reconnect()
{
    Serial.println("RmProtocolMqtt::Reconnect()");
    connectToMqtt();
}

bool RmProtocolMqtt::_onConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    mqttClient.subscribe(topic.c_str(), 0);
    return true;
}

void RmProtocolMqtt::connectToMqtt()
{
    Serial.println("MAIN Connecting to MQTT...");
    mqttClient.connect();
}

void RmProtocolMqtt::messageReceived(char *topic, char *payload,
                                     AsyncMqttClientMessageProperties properties, size_t len,
                                     size_t index, size_t total)
{
    Serial.println("RmProtocolMqtt::MessageReceived()");
    if (len > 0)
    {
        payload[len] = '\0';
        // Serial.printf("[%s](%u):%s\n", topic, len, payload);
        rmProtocol->ReceivedState(String(payload));
    }
    else
    {
        Serial.printf("[NULL](%u):NULL\n", len);
    }
}
