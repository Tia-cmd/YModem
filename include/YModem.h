#pragma once
#include <Arduino.h>

class YModem {
public:
    YModem(Stream& serial = Serial, uint32_t timeoutMs = 5000);

    // Sender: fájl küldése PC-nek
    bool sendFile(const uint8_t* data, uint32_t size, const char* filename = "eprom.bin");

    // Receiver: fájl fogadása + fájlnév visszaadása
    bool receiveFile(uint8_t* buffer, uint32_t maxSize, uint32_t& receivedSize, String& outFilename);

    // Egyszerűbb: csak adat fogadása (fájlnév nélkül)
    bool receiveData(uint8_t* buffer, uint32_t maxSize, uint32_t& receivedSize);

private:
    Stream& _serial;
    uint32_t _timeout;

    uint16_t crc16(const uint8_t* buf, uint16_t len);
    void flushInput();
    int readByte(uint32_t timeoutMs);
    bool sendByte(uint8_t c);
    bool waitForChar(uint8_t expected, uint32_t timeoutMs);

    // Receiver segédfüggvények
    bool receivePacket(uint8_t* packet, uint16_t expectedLen);
    bool handleFileInfoBlock(uint8_t* packet, String& filename, uint32_t& fileSize);
};