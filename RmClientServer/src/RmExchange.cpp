#include "RmExchange.hpp"
#include "RmConfiguration.hpp"
#include "RmProtocolUart.hpp"
#include <SafeString.h>

RmExchange::RmExchange()
{
    Serial2.begin(rmConfig->xchngConfig.baudrate, SERIAL_8N1, rmConfig->xchngConfig.rx, rmConfig->xchngConfig.tx);
    xchngProtocol = new RmProtocolUart(&Serial2);
}

void RmExchange::Begin()
{
    xchngProtocol->Begin();
}

IdConfig RmExchange::GenerateId()
{
    IdConfig idConfig;
    idConfig.id = random(CHARSET_BEGIN, CHARSET_END);
    idConfig.isValid = true;
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        idConfig.key[i] = random(CHARSET_BEGIN, CHARSET_END);
    }
    return idConfig;
}

bool RmExchange::SendId(IdConfig idConfig)
{
    createSafeString(buffer, 200);
    buffer = "(I#";
    buffer += idConfig.id;
    buffer += "#";
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        buffer += idConfig.key[i];
    }
    buffer += ")";
    String s = String(buffer.c_str());
    return xchngProtocol->SendPkg(s);
}

IdConfig RmExchange::ReceiveId(String pkg)
{
    IdConfig idConfig;
    idConfig.isValid = false;
    if (pkg.length() > 0)
    {
        if (pkg[0] == '(' && pkg[pkg.length() - 1] == ')' && pkg.indexOf("I#") == 1)
        {
            String s = pkg.substring(3);
            int index = s.indexOf("#");
            if (index == -1)
            {
                idConfig.isValid = false;
                return idConfig;
            }
            idConfig.id = s.substring(0, index).toInt();
            s = s.substring(index + 1, s.length() - 2);
            if (s.length() != KEY_LENGTH)
            {
                idConfig.isValid = false;
                return idConfig;
            }
            for (int i = 0; i < KEY_LENGTH; i++)
            {
                idConfig.key[i] = s[i];
            }
            idConfig.isValid = true;
        }
    }
    return idConfig;
}

RmExchange::~RmExchange()
{
    Serial2.end();
}
