#include "RmProtocolLora.hpp"
#include "RmLoger.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"

LORA *RmProtocolLora::radio;
bool RmProtocolLora::isChannelFree = true;
String RmProtocolLora::pkgForTransmit = "";
String RmProtocolLora::lastPkg = "";
TimerHandle_t RmProtocolLora::sendTimer;
StaticTimer_t RmProtocolLora::sendTimerBuffer;

RmProtocolLora::RmProtocolLora()
{
    LoraConfig *cfg;
#if MODE == 1
    cfg = &rmConfig->srvLoraConfig;
#else
    cfg = &rmConfig->clientLoraConfig;
#endif
#if LORA == SX1278
    radio = new SX1278(new Module(cfg->connection.LoraSx1278.nss,
                                  cfg->connection.LoraSx1278.dio0,
                                  cfg->connection.LoraSx1278.nrst,
                                  cfg->connection.LoraSx1278.dio1));
#endif
    esp_event_handler_register(RMPROTOCOL_EVENT, RMEVENT_LORA_SOMETHING_HAPPENS, packageReceived, NULL);
    sendTimer = xTimerCreateStatic(
        "sendTimer", pdMS_TO_TICKS(1000), pdTRUE, NULL, [](TimerHandle_t xTimer)
        {
        if (isChannelFree) {
            if (pkgForTransmit != "")
            {
                rmLoger->append(F("Lora timer. Queued package:")).append(pkgForTransmit).Debug();
                radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());
                pkgForTransmit = "";
                isChannelFree = false;
            } else 
            {
                if (lastPkg != "") 
                {
                    rmLoger->append(F("Lora timer. Resend last package:")).append(lastPkg).Debug();
                    radio->startTransmit(lastPkg.c_str(), lastPkg.length());
                    isChannelFree = false;
                }
            }
        } },
        &sendTimerBuffer);
}

ICACHE_RAM_ATTR void RmProtocolLora::loraISR()
{
    esp_event_isr_post(RMPROTOCOL_EVENT, RMEVENT_LORA_SOMETHING_HAPPENS, NULL, 0, NULL);
}

void RmProtocolLora::Begin()
{
    int state = radio->begin();
    if (state == RADIOLIB_ERR_NONE)
    {
        rmLoger->Info(F("LORA is initiated successfully!"));
    }
    else
    {
        rmLoger->append(F("LORA initialization failed, code ")).append(state).Error();
    }
    radio->setDio0Action(loraISR, RISING);
    radio->startReceive();
}

void RmProtocolLora::Reconnect()
{
}

void RmProtocolLora::ReceivedState(String state)
{
    rmLoger->append(F("LORA ReceivedCommand: ")).append(state).Debug();
    CommandState st = RmCommands::StringToState(state);
    if (st.isValid)
    {
        esp_event_post(RMPROTOCOL_EVENT, RMEVENT_STATE_RECEIVED, &st, sizeof(st), portMAX_DELAY);
    }
    else
    {
        rmLoger->Error(F("Invalid command!"));
    }
}

bool RmProtocolLora::SendCommand(String command)
{
    pkgForTransmit = String(rmSession->GetSessionId());
    pkgForTransmit += command;
    lastPkg = pkgForTransmit;
    if (isChannelFree)
    {
        rmLoger->append("Sending first:").append(pkgForTransmit).Debug();
        isChannelFree = false;
        int state = radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());

        if (state != RADIOLIB_ERR_NONE)
        {
            rmLoger->append(F("LORA transmission failed, code ")).append(state).Error();
            return false;
        }
    }
    else
    {
        rmLoger->append("Queued:").append(pkgForTransmit).Debug();}
    return true;
}

void RmProtocolLora::packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    uint status = radio->getIRQFlags();
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_DONE)
    { // received a packet
        String str;
        radio->readData(str);
        rmLoger->append("Received: ").append(str).Debug();
        if (str[0] != rmSession->GetSessionId())
        {
            rmLoger->Info(F("Received for other device, ignoring."));
            return;
        }
        rmProtocol->ReceivedState(str.substring(1));
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_TX_DONE)
    { // transfer completed
        radio->finishTransmit();
        xTimerReset(sendTimer, 0);
        pkgForTransmit = "";
        rmLoger->Debug(F("Transfer completed."));
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_VALID_HEADER)
    {
        rmLoger->Debug(F("Valid header."));
    }
    if (pkgForTransmit.length() > 0)
    {
        rmLoger->Debug(F("Sending queued..."));
        isChannelFree = false;
        radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());
    }
    else
    {
        radio->startReceive();
        isChannelFree = true;
    }
}