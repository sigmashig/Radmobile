#include "RmId.hpp"
#include "RmTypes.hpp"

String RmId::IdToString(char id, byte key[])
{
    String idString;
    idString += '(';
    idString += id;
    idString += '#';
    for (int i = 0; i < KEY_LENGTH; i++)
    {
        idString += String(key[i], HEX);
    }
    idString += ')';
    return idString;
}

IdConfig RmId::StringToId(String idString)
{
    IdConfig id;
    if (idString[0]=='(' && idString[idString.length()-1]==')' && idString[2]=='#')
    {
        id.id = idString[1];
        for (int i = 0; i< KEY_LENGTH; i++)
        {
            String hex = idString.substring(3 + i * 2, 4 + i * 2);
            id.key[i] = strtol(hex.c_str(), NULL, 16);
        }
        id.isValid = true;
    }
    else
    {
        id.isValid = false;
    }
    return id;
}
