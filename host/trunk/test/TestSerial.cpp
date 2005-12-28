#include "TestSerial.h"
#include "crc.h"
#include "util.h"

using namespace adt;
using namespace std;

TestSerial::TestSerial(istream & inputStream, ostream & outputStream)
    : mInputStream(inputStream), mOutputStream(outputStream)
{
}

int TestSerial::getc()
{
    char ch;
    mInputStream.read(&ch, 1);
    int len = mInputStream.gcount();
    if (len != 1)
    {
        return -1;
    }
    return ch;
}

void TestSerial::putc(Uint8 ch)
{
    mOutputStream.write((char *)&ch, 1);
}
