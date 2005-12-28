#include "TestSerial.h"
#include "crc.h"
#include "util.h"

using namespace adt;
using namespace std;

TestSerial::TestSerial(istream & inputStream, ostream & outputStream)
    : mInputStream(inputStream), mOutputStream(outputStream)
{
}

int TestSerial::receiveByte()
{
    char byte;
    mInputStream.read(&byte, 1);
    int len = mInputStream.gcount();
    if (len != 1)
    {
        return -1;
    }
    return byte;
}

void TestSerial::sendByte(Uint8 byte)
{
    mOutputStream.write((char *)&byte, 1);
}
