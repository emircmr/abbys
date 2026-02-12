#ifndef COM_H
#define COM_H

#include <cstdint>
#include <cstring>

//12 byte olacak şekilde paket
#pragma pack(push, 1) // Bellekte boşluk bırakmayı engelleyen kod
struct ControlPacket {
    uint8_t head1;      // 0xFA
    uint8_t head2;      // 0xFB
    int16_t surge;      // İleri/Geri
    int16_t sway;       // Sağ/Sol
    int16_t heave;      // Dalma/Çıkma
    int16_t yaw;        // Dönme
    uint8_t buttons;    // Butonlar
    uint8_t checksum;   // Doğrulama
};
#pragma pack(pop)

class SerialParser {
public:
    SerialParser();
    
    // Interrupt içinden Gelen her byte'ı besleyen fonk.
    bool parseByte(uint8_t byte);

    // Geçerli son paketi almak için
    const ControlPacket& getPacket() const;

private:
    enum State { WAIT_HEAD1, WAIT_HEAD2, READ_PAYLOAD, CHECK_CRC };
    State currentState;
    
    uint8_t buffer[sizeof(ControlPacket)]; // Geçici buffer
    uint8_t bufferIndex;
    
    ControlPacket validPacket; // valid olan son paket
};

#endif