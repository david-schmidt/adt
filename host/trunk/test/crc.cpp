#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "test/testUtil.h"
#include "crc.h"

using namespace adt;
using namespace std;

#define CLASS Crc16Test

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testCrc);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testCrc();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testCrc()
{
    string data;
    int i;
    for (i = 0; i < 256; i++)
    {
        data.append(1, i);
    }
    ASSERT_EQUAL(Uint16(0x7E55), Crc16::calculateCrc(data));
}
