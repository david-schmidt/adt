#ifndef ADT_TEST_TEST_SERIAL_H
#define ADT_TEST_TEST_SERIAL_H

#include <iostream>
#include "adttypes.h"
#include "serial.h"

namespace adt {

class TestSerial : public Serial
{
  public:
    TestSerial(std::istream & inputStream, std::ostream & outputStream);
    
    int getc();
    
    void putc(Uint8 ch);
    
  private:
    std::istream & mInputStream;
    std::ostream & mOutputStream;
};

};

#endif