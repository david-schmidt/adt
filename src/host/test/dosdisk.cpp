#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include <stdexcept>
#include "test/testUtil.h"
#include "dosdisk.h"
#include "util.h"

using namespace adt;
using namespace std;

#define CLASS DosDiskTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testWriteEmpty);
    CPPUNIT_TEST(testWritePattern);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testWriteSectorTooLong);
    CPPUNIT_TEST(testWriteSectorTooShort);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testWriteEmpty();
    void testWritePattern();
    void testRead();
    void testWriteSectorTooLong();
    void testWriteSectorTooShort();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testWriteEmpty()
{
    DosDisk disk;
    stringstream stream;
    stream << disk;
    string image = stream.str();
    ASSERT_EQUAL((size_t) DosDisk::TOTAL_BYTES, image.length());

    string expected(DosDisk::TOTAL_BYTES, '\0');
    ASSERT_DATA_EQUAL(expected, image);
}

void CLASS::testWritePattern()
{
    DosDisk disk;
    int track;
    int sector;
    for (track = 0; track < DosDisk::TRACKS; track++)
    {
        for (sector = 0; sector < DosDisk::SECTORS; sector++)
        {
            char value = (track << 4) | sector;
            string data(DosDisk::SECTOR_SIZE, value);
            disk.writeSector(track, sector, data);
        }
    }
    
    stringstream stream;
    stream << disk;
    string expected;
    for (track = 0; track < DosDisk::TRACKS; track++)
    {
        for (sector = 0; sector < DosDisk::SECTORS; sector++)
        {
            Uint8 value = (track << 4) | sector;
            expected.append(DosDisk::SECTOR_SIZE, value) ;
        }
    }
    ASSERT_DATA_EQUAL(expected, stream.str());
}

void CLASS::testRead()
{
    int track;
    int sector;
    stringstream expected;
    for (track = 0; track < DosDisk::TRACKS; track++)
    {
        for (sector = 0; sector < DosDisk::SECTORS; sector++)
        {
            Uint8 value = (track << 4) | sector;
            string data(DosDisk::SECTOR_SIZE, value);
            expected << data;
        }
    }
    DosDisk disk;
    expected >> disk;

    for (track = 0; track < DosDisk::TRACKS; track++)
    {
        for (sector = 0; sector < DosDisk::SECTORS; sector++)
        {
            Uint8 value = (track << 4) | sector;
            string expected(DosDisk::SECTOR_SIZE, value);
            string actual = disk.readSector(track, sector);
            ASSERT_DATA_EQUAL_MESSAGE(
                str_stream() << "Track: " << track << ", sector: " << sector,
                expected, actual);
        }
    }
}

void CLASS::testWriteSectorTooLong()
{
    DosDisk disk;
    string data(DosDisk::SECTOR_SIZE + 1, 'a');
    try
    {
        disk.writeSector(0, 0, data);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (runtime_error &)
    {
        // Expected
    }
}

void CLASS::testWriteSectorTooShort()
{
    DosDisk disk;
    string data(DosDisk::SECTOR_SIZE - 1, 'a');
    try
    {
        disk.writeSector(0, 0, data);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (runtime_error &)
    {
        // Expected
    }
}
