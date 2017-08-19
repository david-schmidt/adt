#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "test/testUtil.h"
#include "TestSerial.h"
#include "dosxfer.h"
#include "dosdisk.h"

using namespace adt;
using namespace std;

#define CLASS DosReceiverTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testReceiveDosDisk);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testReceiveDosDisk();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testReceiveDosDisk()
{
    istreamAPtr input(getResource("ascending_in.bin"));
    ostringstream output;
    TestSerial serial(*input, output);

    AdtPlatform platform;
    DosReceiver receiver;
    receiver.execute(serial, platform);
    ASSERT_STRING_EQUAL("file.dsk", receiver.getFileName());
    const DosDisk & actualDisk = receiver.getDisk();
    
    istreamAPtr inputDisk(getResource("ascending.dsk"));
    DosDisk expectedDisk;
    *inputDisk >> expectedDisk;
    ASSERT_DISK_EQUAL(expectedDisk, actualDisk);

    string expectedOutput;
    // One null character
    expectedOutput.append(1, 0x00);
    // Plus one ACK for every sector
    expectedOutput.append(DosDisk::TOTAL_SECTORS, DiskTransfer::ACK);
    ASSERT_DATA_EQUAL(expectedOutput, output.str());
}
