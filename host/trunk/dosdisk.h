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
    static const int SECTORS = 16;
    static const int TRACKS = 35;
    static const int SECTOR_SIZE = 256;
    static const int TRACK_SIZE = SECTORS * SECTOR_SIZE;
    static const int TOTAL_SECTORS = SECTORS * TRACKS;
    static const int TOTAL_BYTES = TOTAL_SECTORS * SECTOR_SIZE;
    
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

    void writeSector(int track, int sector, const std::string & data);
    
    void readSector(int track, int sector, std::string & data) const;
    
    std::string readSector(int track, int sector) const;
    
  private:
    /**
     * Returns a pointer to a 256-byte buffer for the specified track
     * and sector.
     */
    Uint8 * getSector(int track, int sector);

    const Uint8 * getSector(int track, int sector) const;

    Uint8 mImage[TOTAL_BYTES];
};

std::ostream & operator<<(std::ostream & s, const DosDisk & dosDisk);

std::istream & operator>>(std::istream & s, DosDisk & dosDisk);

};

#endif