#include "crc.h"

using namespace adt;
using std::string;

bool Crc16::sTableInitialized = false;

Uint16 Crc16::sCrcTable[];

Crc16::Crc16()
    : mCrc(0)
{
    if (!sTableInitialized)
    {
        initTable();
        sTableInitialized = true;
    }
}

void Crc16::initTable()
{
    int byte;
    for(byte = 0; byte < 256; byte++)
    {
        unsigned crc = byte << 8;
        int bit;
        for(bit = 0; bit < 8; bit++)
            crc = (crc & 0x8000 ? (crc <<1 )^0x1021 : crc << 1);
        sCrcTable[byte] = crc;
    }    
}

Uint16 Crc16::getCrc() const
{
    return mCrc;
}

void Crc16::updateCrc(Uint8 value)
{
    mCrc = (mCrc << 8) ^ sCrcTable[(mCrc >> 8) ^ value];
}

void Crc16::updateCrc(Uint8 * data, size_t length)
{
    size_t i;
    for (i = 0; i < length; i++)
    {
        updateCrc(data[i]);
    }
}

void Crc16::updateCrc(string data)
{
    updateCrc((Uint8 *) data.data(), data.length());
}

void Crc16::reset()
{
    mCrc = 0;
}

Uint16 Crc16::calculateCrc(Uint8 * data, size_t length)
{
    Crc16 crc;
    crc.updateCrc(data, length);
    return crc.getCrc();
}

Uint16 Crc16::calculateCrc(string data)
{
    return calculateCrc((Uint8 *) data.data(), data.length());
}