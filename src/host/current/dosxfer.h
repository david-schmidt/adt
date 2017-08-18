#ifndef ADT_DOSXFER_H
#define ADT_DOSXFER_H

#include "adttypes.h"
#include "dosdisk.h"
#include "serial.h"

namespace adt {

class DiskTransfer
{
  public:
    static const Uint8 ACK = 0x06;
    static const Uint8 NAK = 0x15;
    
    static const Uint8 CANNOT_OPEN = 26;
    static const Uint8 FILE_EXISTS = 28;
    static const Uint8 DISK_FULL = 30;
};

class AdtPlatform
{
};

class DosReceiver : public DiskTransfer
{
  public:
    DosReceiver();

    void execute(Serial & serial, AdtPlatform & platform);

    std::string getFileName() const;

    const DosDisk & getDisk() const;
    
    bool receivedWithErrors() const; 
    
  private:
    std::string mFileName;
    DosDisk mDisk;
    bool mReceivedWithErrors;
};

class DosSender
{
  public:
    void execute(AdtPlatform & platform);
};

};

#endif