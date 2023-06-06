#include "SigmaCrypt.hpp"

SigmaCrypt::SigmaCrypt(const byte *key, int keyLength) : keyLength(keyLength)
{
    this->key = new byte[keyLength];
    memcpy(this->key, key, keyLength);
}

void SigmaCrypt::Encrypt(char *cipherOutput, const char *clearInput)
{

    for (int i = 0; i < strlen(clearInput); i++)
    {
        uint c = clearInput[i] - blockBeg;
        uint r = (c + key[i % keyLength]) % blockLen;
        cipherOutput[i] = r + blockBeg;
        //Serial.printf("input:%c, c=0x%x,key=0x%x,r=0x%x:%c\n", clearInput[i], c, key[i % keyLength], r, cipherOutput[i]);
    }
    cipherOutput[strlen(clearInput)] = '\0';
}

void SigmaCrypt::Decrypt(char *clearOutput, const char *cipherInput)
{
    //Serial.println("Decrypting:");
    for (int i = 0; i < strlen(cipherInput); i++)
    {
        uint c = cipherInput[i] - blockBeg;
        uint r = (c + blockLen * 4 - key[i % keyLength]) % blockLen;
        clearOutput[i] = r + blockBeg;
        //Serial.printf("input:%c, c=0x%x,key=0x%x,r=0x%x:%c\n", cipherInput[i], c, key[i % keyLength], r, clearOutput[i]);
    }
    clearOutput[strlen(cipherInput)] = '\0';
    //Serial.println("\nDone");
}

//------------------------------------------------------------------------------
SigmaCrypt *sigmaCrypt;
