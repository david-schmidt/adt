#include <string.h>
#include "serial.h"
#include "crc.h"
#include "util.h"

using namespace adt;
using std::string;

Serial::~Serial()
{
    
}

bool Serial::receiveDataPacket(string & packet)
{
    packet.clear();
    packet.reserve(PACKET_SIZE);

    Uint8 previous = 0;
    string::size_type byte;
    for (byte = 0; byte < PACKET_SIZE;)
    {
        Uint8 data = receiveByte();
        if (data != 0)
        {
            previous += data;
            packet.append(1, previous);
            byte++;
        }
        else
        {
            data = receiveByte();
            do
            {
                packet.append(1, previous);
                byte++;
            } while ((byte < PACKET_SIZE) && byte != data);
        }
    }
    CXX_ASSERT(packet.length() == PACKET_SIZE);
    Uint8 crcLo = receiveByte();
    Uint8 crcHi = receiveByte();
    Uint16 receivedCrc = crcHi << 8 | crcLo;
    Uint16 computedCrc = Crc16::calculateCrc(packet);
    return (receivedCrc == computedCrc);
}

string Serial::receiveString()
{
    string aString;
    while(true)
    {
        // Set high bit to low
        Uint8 data = receiveByte() & 0x7F;
        if (data == 0x00)
            break;
        aString.append(1, data);
    }
    return aString;
}

void Serial::sendDataPacket(const string & packet)
{
    CXX_ASSERT(packet.length() == PACKET_SIZE);
    Uint8 previous = 0;
    string::size_type byte;
    for (byte = 0; byte < PACKET_SIZE;)
    {
        Uint8 newPrevious = packet[byte];
        Uint8 data = newPrevious - previous;
        previous = newPrevious;
        sendByte(data);
        if (data != 0)
            byte++;
        else
        {
            while ((byte < PACKET_SIZE) && (packet[byte] == newPrevious))
                byte++;
            sendByte(byte & 0xFF);  // 256 becomes 0
        }
    }
    Uint16 crc = Crc16::calculateCrc(packet);
    sendByte(crc & 0xFF);
    sendByte(crc >> 8);
}
