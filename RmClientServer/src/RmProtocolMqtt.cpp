#include "RmProtocolMqtt.hpp"
#include "RmCommands.hpp"
#include <esp_event.h>
//#include <WiFi.h>

String RmProtocolMqtt::topic = "RadMobile/Command";
AsyncMqttClient RmProtocolMqtt::mqttClient;

void RmProtocolMqtt::Begin()
{
    Serial.println("RmProtocolMqtt::Begin()");
    mqttClient.setServer(MQTT_URL, MQTT_PORT);
    mqttClient.setClientId("ESP32");
    mqttClient.setCleanSession(true);
    mqttClient.setKeepAlive(15);

    mqttClient.onDisconnect([this](AsyncMqttClientDisconnectReason reason)
                            { 
                                Serial.println("Disconnected from MQTT."); 
                                connectToMqtt();
                            }
                            );
    mqttClient.onMessage(messageReceived);
    mqttClient.onConnect(_onConnect);
    connectToMqtt();
}

void RmProtocolMqtt::ReceivedCommand(String command)
{
    Serial.println("RmProtocolMqtt::ReceivedCommand()");
    Serial.println(command);
    RmCommandPkg cmd;
    cmd = RmCommands::StringToCommand(command);
    esp_event_post(RMPROTOCOL_EVENT, RMEVENT_CMD_RECEIVED, &cmd, sizeof(cmd), portMAX_DELAY);
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
        Serial.printf("[%s](%u):%s\n", topic, len, payload);
        rmProtocol->ReceivedCommand(String(payload));
    }
    else
    {
        Serial.printf("[NULL](%u):NULL\n", len);
    }
}
