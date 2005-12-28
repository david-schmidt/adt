#ifndef ADT_DOSDISK_H
#define ADT_DOSDISK_H

#include <iostream>
#include "adttypes.h"

namespace adt {
    
/**
 * A DOS 3.3 disk image.
 */
class DosDisk
{
  public:
    static const unsigned SECTORS = 16;
    static const unsigned TRACKS = 35;
    static const unsigned SECTOR_SIZE = 256;
    static const unsigned TRACK_SIZE = SECTORS * SECTOR_SIZE;
    static const unsigned TOTAL_BYTES = SECTORS * SECTOR_SIZE * TRACKS;
    
    DosDisk();
    
    void erase();
    
    /**
     * Writes this disk image to an output stream.
     */
    std::ostream & write(std::ostream & outputStream)  const;
    
    /**
     * Reads a disk image from an input stream, erasing the current image.
     */
    std::istream & read(std::istream & inputStream);

    void writeSector(unsigned track, unsigned sector, const std::string & data);
    
    void readSector(unsigned track, unsigned sector, std::string & data) const;
    
    std::string readSector(unsigned track, unsigned sector) const;
    
  private:
    /**
     * Returns a pointer to a 256-byte buffer for the specified track
     * and sector.
     */
    Uint8 * getSector(unsigned track, unsigned sector);

    const Uint8 * getSector(unsigned track, unsigned sector) const;

    Uint8 mImage[TOTAL_BYTES];
};

std::ostream & operator<<(std::ostream & s, const DosDisk & dosDisk);

std::istream & operator>>(std::istream & s, DosDisk & dosDisk);

};

#endif