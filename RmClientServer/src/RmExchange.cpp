#include "RmExchange.hpp"
#include "RmConfiguration.hpp"
#include "RmProtocolUart.hpp"
#include <SafeString.h>
#include <SPI.h>

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
    idConfig.id = random(CHARSET_BEGIN, CHARSET_END);
    idConfig.isValid = true;
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        idConfig.key[i] = random(CHARSET_BEGIN, CHARSET_END);
    }
    return idConfig;
}

bool RmExchange::SendId()
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

bool RmExchange::SendAck(bool result)
{
    createSafeString(buffer, 10);
    buffer = "(A#";
    buffer += result ? "1" : "0";
    buffer += ")";
    String s = String(buffer.c_str());
    return xchngProtocol->SendPkg(s);
}

bool RmExchange::ReceiveCmd(String pkg)
{
    if (pkg.length() > 0)
    {
        if (pkg[0] == '(' && pkg[pkg.length() - 1] == ')')
        {
            switch (pkg[1])
            {
            case 'I':
            { // id
                String s = pkg.substring(3);
                int index = s.indexOf("#");
                if (index == -1)
                {
                    idConfig.isValid = false;
                    return false;
                }
                idConfig.id = s.substring(0, index).toInt();
                s = s.substring(index + 1, s.length() - 2);
                if (s.length() != KEY_LENGTH)
                {
                    idConfig.isValid = false;
                    return false;
                }
                for (int i = 0; i < KEY_LENGTH; i++)
                {
                    idConfig.key[i] = s[i];
                }
                idConfig.isValid = true;
                IsIdReceived = true;
                return true;
            }
            case 'A':
            { // ack
                if (pkg[4] == '#' && pkg[5] == '1')
                {
                    IsAckReceived = true;
                    IsAckGood = true;
                    return true;
                }
                else if (pkg[4] == '#' && pkg[5] == '0')
                {
                    IsAckReceived = true;
                    IsAckGood = false;
                    return true;
                }
                else
                {
                    IsAckReceived = false;
                    IsAckGood = false;
                    return false;
                }
            }
            case 'E':
            { // end
                IsEndReceived = true;
                return true;
            }
            }
        }
    }
    return false;
}

bool RmExchange::SendEnd()
{
    return xchngProtocol->SendPkg("(E)");
}

RmExchange::~RmExchange()
{
    Serial2.end();
}
