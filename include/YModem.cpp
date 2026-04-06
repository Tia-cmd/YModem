#include "YModem.h"

#define SOH  0x01
#define STX  0x02
#define EOT  0x04
#define ACK  0x06
#define NAK  0x15
#define CAN  0x18
#define CRC  'C'

YModem::YModem(Stream& serial, uint32_t timeoutMs) 
    : _serial(serial), _timeout(timeoutMs) {}

uint16_t YModem::crc16(const uint8_t* buf, uint16_t len) {
    uint16_t crc = 0;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)buf[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void YModem::flushInput() {
    while (_serial.available()) _serial.read();
}

int YModem::readByte(uint32_t timeoutMs) {
    uint32_t start = millis();
    while (millis() - start < timeoutMs) {
        if (_serial.available()) return _serial.read();
    }
    return -1;
}

bool YModem::sendByte(uint8_t c) {
    _serial.write(c);
    return true;
}

bool YModem::waitForChar(uint8_t expected, uint32_t timeoutMs) {
    int c = readByte(timeoutMs);
    return (c == expected);
}

// ====================== RECEIVER ======================

bool YModem::receiveFile(uint8_t* buffer, uint32_t maxSize, uint32_t& receivedSize, String& outFilename) {
    receivedSize = 0;
    outFilename = "";
    flushInput();

    uint8_t packet[1030];  // STX + blk + ~blk + 1024 data + 2 CRC
    uint32_t fileSize = 0;
    uint8_t blockNum = 1;
    bool firstBlock = true;

    // Küldjük a kezdeményező 'C'-t (CRC mód)
    sendByte(CRC);
    delay(100);

    while (true) {
        int header = readByte(_timeout);
        if (header < 0) {
            sendByte(NAK);
            continue;
        }

        if (header == CAN) return false;  // cancel

        if (header == EOT) {
            sendByte(ACK);
            return true;  // sikeres befejezés
        }

        uint16_t packetLen = (header == STX) ? 1029 : 133;  // 1K vagy 128 byte

        if (!receivePacket(packet, packetLen)) {
            sendByte(NAK);
            continue;
        }

        uint8_t recvBlock = packet[1];
        uint8_t recvBlockInv = packet[2];

        if (recvBlock != (uint8_t)~recvBlockInv) {
            sendByte(NAK);
            continue;
        }

        // 0. blokk = fájlnév + méret
        if (firstBlock && recvBlock == 0) {
            if (handleFileInfoBlock(packet + 3, outFilename, fileSize)) {
                sendByte(ACK);
                firstBlock = false;
                sendByte(CRC);  // következő blokkhoz CRC-t kérünk
                continue;
            } else {
                sendByte(NAK);
                continue;
            }
        }

        // Normál adatblokk
        if (recvBlock != blockNum) {
            sendByte(NAK);
            continue;
        }

        uint16_t dataLen = (header == STX) ? 1024 : 128;
        uint16_t crcReceived = (packet[3 + dataLen] << 8) | packet[3 + dataLen + 1];
        uint16_t crcCalc = crc16(packet + 3, dataLen);

        if (crcReceived != crcCalc) {
            sendByte(NAK);
            continue;
        }

        // Adat másolása a bufferbe
        uint32_t toCopy = min((uint32_t)dataLen, maxSize - receivedSize);
        memcpy(buffer + receivedSize, packet + 3, toCopy);
        receivedSize += toCopy;

        sendByte(ACK);
        blockNum++;
        if (blockNum == 0) blockNum = 1;
    }
}

bool YModem::receivePacket(uint8_t* packet, uint16_t expectedLen) {
    for (uint16_t i = 0; i < expectedLen; i++) {
        int c = readByte(_timeout);
        if (c < 0) return false;
        packet[i] = (uint8_t)c;
    }
    return true;
}

bool YModem::handleFileInfoBlock(uint8_t* data, String& filename, uint32_t& fileSize) {
    filename = "";
    fileSize = 0;

    // Fájlnév (0-val végződik)
    for (int i = 0; i < 128 && data[i] != 0; i++) {
        if (data[i] >= 32) filename += (char)data[i];
    }

    // Fájlméret (space után)
    String sizeStr = "";
    bool inSize = false;
    for (int i = 0; i < 128; i++) {
        if (data[i] == ' ') inSize = true;
        else if (inSize && isdigit(data[i])) sizeStr += (char)data[i];
        else if (inSize) break;
    }

    if (sizeStr.length() > 0) fileSize = sizeStr.toInt();
    return (filename.length() > 0);
}

// Egyszerűsített verzió csak adatokhoz (fájlnév nélkül)
bool YModem::receiveData(uint8_t* buffer, uint32_t maxSize, uint32_t& receivedSize) {
    String dummy;
    return receiveFile(buffer, maxSize, receivedSize, dummy);
}