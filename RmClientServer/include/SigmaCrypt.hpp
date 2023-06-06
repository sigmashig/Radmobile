#pragma once
#include <Arduino.h>

class SigmaCrypt
{
public:
    SigmaCrypt(const byte *key, int keyLength);
    void Encrypt(char *cipherOutput, const char *clearInput);
    void Decrypt(char *clearOutput, const char *cipherInput);
    

private:
    byte *key;
    int keyLength;
    const byte blockBeg = 0x20;
    const byte blockLen = 0x7e - blockBeg + 1;
};
extern SigmaCrypt *sigmaCrypt;