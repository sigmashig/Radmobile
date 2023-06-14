#include "RmProtocolLora.hpp"
#include "SigmaLoger.hpp"
#include "RmConfiguration.hpp"
#include "RmCommands.hpp"
#include "RmSession.hpp"
#include "RmPackageValidator.hpp"

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
                //Log->Append(F("Lora timer. Queued package:")).Append(pkgForTransmit).Debug();
                radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());
                pkgForTransmit = "";
                isChannelFree = false;
            } else 
            {
                if (lastPkg != "") 
                {
                    //Log->Append(F("Lora timer. Resend last package:")).Append(lastPkg).Debug();
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
        Log->Info(F("LORA is initiated successfully!"));
    }
    else
    {
        Log->Append(F("LORA initialization failed, code ")).Append(state).Error();
    }
    radio->setDio0Action(loraISR, RISING);
    radio->startReceive();
}

RmProtocolLora::~RmProtocolLora()
{
    if (radio != NULL)
    {
        delete radio;
    }
    esp_event_handler_unregister(RMPROTOCOL_EVENT, RMEVENT_LORA_SOMETHING_HAPPENS, packageReceived);
    xTimerDelete(sendTimer, portMAX_DELAY);
}

void RmProtocolLora::Reconnect()
{
}
bool RmProtocolLora::SendPkg(String command)
{
    pkgForTransmit = String(rmSession->GetSessionId().id);
    pkgForTransmit += command;
    lastPkg = pkgForTransmit;
    if (isChannelFree)
    {
        Log->Append("Sending first:").Append(pkgForTransmit).Debug();
        isChannelFree = false;
        int state = radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());

        if (state != RADIOLIB_ERR_NONE)
        {
            Log->Append(F("LORA transmission failed, code ")).Append(state).Error();
            return false;
        }
    }
    else
    {
        Log->Append("Queued:").Append(pkgForTransmit).Debug();
    }
    return true;
}

void RmProtocolLora::packageReceived(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    uint status = radio->getIRQFlags();
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_RX_DONE)
    { // received a packet
        String str;
        radio->readData(str);
        Log->Append("Received: ").Append(str).Debug();
        if (str[0] != rmSession->GetSessionId().id)
        {
            Log->Info(F("Received for other device, ignoring."));
            return;
        }
        RmPackageValidator::Validate(str.substring(1));
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_TX_DONE)
    { // transfer completed
        radio->finishTransmit();
        xTimerReset(sendTimer, 0);
        pkgForTransmit = "";
        Log->Debug(F("Transfer completed."));
    }
    if (status & RADIOLIB_SX127X_CLEAR_IRQ_FLAG_VALID_HEADER)
    {
        Log->Debug(F("Valid header."));
    }
    if (pkgForTransmit.length() > 0)
    {
        Log->Debug(F("Sending queued..."));
        isChannelFree = false;
        radio->startTransmit(pkgForTransmit.c_str(), pkgForTransmit.length());
    }
    else
    {
        radio->startReceive();
        isChannelFree = true;
    }
}

//---------------------------------------------------

RmProtocolLora *rmProtocolLora = NULL;