#ifndef CRC_H
#define CRC_H

#include <string.h>
#include <string>
#include "adttypes.h"

namespace adt {

class Crc16
{
  public:
    Crc16();
    
    Uint16 getCrc() const;
    
    void updateCrc(Uint8 value);

    void updateCrc(Uint8 * data, size_t length);
    
    void updateCrc(std::string data);

    void reset();
    
    static Uint16 calculateCrc(Uint8 * data, size_t length);
    
    static Uint16 calculateCrc(std::string data);
    
  private:
    static void initTable();
    
    static bool sTableInitialized;
    static Uint16 sCrcTable[256];
    Uint16 mCrc;
};

};

#endif