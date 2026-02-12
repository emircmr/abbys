#include "com.h"

SerialParser::SerialParser() : currentState(WAIT_HEAD1), bufferIndex(0) {
    memset(&validPacket, 0, sizeof(ControlPacket));
    validPacket.surge = 0; 
}

bool SerialParser::parseByte(uint8_t byte) {
    switch (currentState) {
        case WAIT_HEAD1:
            if (byte == 0xFA) {
                bufferIndex = 0;
                buffer[bufferIndex++] = byte;
                currentState = WAIT_HEAD2;
            }
            break;

        case WAIT_HEAD2:
            if (byte == 0xFB) {
                buffer[bufferIndex++] = byte;
                currentState = READ_PAYLOAD;
            } else {
                currentState = WAIT_HEAD1; 
            }
            break;

        case READ_PAYLOAD:
            buffer[bufferIndex++] = byte;

            if (bufferIndex >= sizeof(ControlPacket) - 1) {
                currentState = CHECK_CRC;
            }
            break;

        case CHECK_CRC:
            buffer[bufferIndex] = byte; 
            

            uint8_t calcSum = 0;
            for (int i = 0; i < sizeof(ControlPacket) - 1; i++) {
                calcSum += buffer[i];
            }

            currentState = WAIT_HEAD1; 

            if (calcSum == byte) {

                memcpy(&validPacket, buffer, sizeof(ControlPacket));
                return true; 
            }
            break;
    }
    return false;
}
const ControlPacket& SerialParser::getPacket() const {
    return validPacket;
}