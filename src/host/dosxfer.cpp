#include "dosxfer.h"

using namespace adt;
using std::string;

DosReceiver::DosReceiver()
{
}

void DosReceiver::execute(Serial & serial, AdtPlatform & platform)
{
    mReceivedWithErrors = false;
    mFileName = serial.receiveString();
    serial.sendByte(0x00);
    while (serial.receiveByte() != ACK)
    {
        // Limit number of retries?
    }
    int track;
    int sector;
    for (track = 0; track < DosDisk::TRACKS; track++)
    {
        for (sector = DosDisk::SECTORS - 1; sector >= 0; sector--)
        {
            string packet;
            serial.receiveDataPacket(packet);
            serial.sendByte(ACK);
            mDisk.writeSector(track, sector, packet);
        }
    }
}

string DosReceiver::getFileName() const
{
    return mFileName;
}

const DosDisk & DosReceiver::getDisk() const
{
    return mDisk;
}