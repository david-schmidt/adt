#include <string.h>
#include "dosdisk.h"
#include "util.h"

using namespace adt;
using std::string;
using std::ostream;
using std::istream;

DosDisk::DosDisk()
{
    erase();
}

void DosDisk::erase()
{
    memset(mImage, 0, TOTAL_BYTES);
}

Uint8 * DosDisk::getSector(unsigned track, unsigned sector)
{
    return &mImage[TRACK_SIZE * track + SECTOR_SIZE * sector];
}

const Uint8 * DosDisk::getSector(unsigned track, unsigned sector) const
{
    return &mImage[TRACK_SIZE * track + SECTOR_SIZE * sector];
}

void DosDisk::writeSector(unsigned track, unsigned sector, const string & data)
{
    CXX_ASSERT(data.length() == SECTOR_SIZE);
    Uint8 * sectorBuffer = getSector(track, sector);
    memcpy(sectorBuffer, data.data(), data.length());
}

void DosDisk::readSector(unsigned track, unsigned sector, string & data) const
{
    const Uint8 * sectorBuffer = getSector(track, sector);
    data.assign((const char *) sectorBuffer, SECTOR_SIZE);
    CXX_ASSERT(data.length() == SECTOR_SIZE);
}

string DosDisk::readSector(unsigned track, unsigned sector) const
{
    string data;
    readSector(track, sector, data);
    return data;
}

ostream & DosDisk::write(ostream & outputStream) const
{
    outputStream.write((const char *) mImage, TOTAL_BYTES);
    return outputStream;
}

ostream & adt::operator<<(ostream & outputStream, const DosDisk & dosDisk)
{
    return dosDisk.write(outputStream);
}

istream & DosDisk::read(istream & inputStream)
{
    inputStream.read((char *) mImage, TOTAL_BYTES);
    return inputStream;
}

istream & adt::operator>>(istream & inputStream, DosDisk & dosDisk)
{
    return dosDisk.read(inputStream);
}
