#ifndef ADT_SERIAL_H
#define ADT_SERIAL_H

#include <string>
#include "adttypes.h"

namespace adt {

class Serial
{
  public:
    virtual ~Serial();
    
    virtual void sendByte(Uint8 byte) = 0;
    
    virtual int receiveByte() = 0;
    
    static const unsigned PACKET_SIZE = 256;

    /**
     * Receives one data packet that is encoded using RLE compression.
     * It is validating using a CRC.
     *
     * @return <code>true</code> if the CRC matches.
     */
    bool receiveDataPacket(std::string & packet);
    
    /**
     * Receive a null terminated ASCII string.
     */
    std::string receiveString();
    
    /**
     * Sends one data packet using RLE compression.  The data is
     * protected with a CRC.
     */
    void sendDataPacket(const std::string & packet);
};
  
};

#endif